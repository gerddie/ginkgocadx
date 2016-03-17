/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "storedqueriescontroller.h"
#include <limits>
#include <iomanip>
#include <iostream>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/dicommanager.h>
#include <api/dicom/dicomdataset.h>
#include <api/dicom/dcmdictionary.h>
#include <api/internationalization/internationalization.h>
#include <main/entorno.h>

#include <wx/xml/xml.h>
#include <wx/mstream.h>
#include <wx/sstream.h>
#include <wx/regex.h>
#include <wx/sizer.h>
#include <wx/display.h>

#define SQ_DOCUMENT_ROOT wxT("stored-queries")

#define SQ_ROOT wxT("stored-query")
#define SQ_ATT_NAME wxT("name")
#define SQ_ATT_PACSID wxT("pacs-id")
#define SQ_ATT_ACTIVE wxT("active")

#define COS_ROOT wxT("conditions")
#define CO_ROOT wxT("condition")
#define CO_ATT_TAG wxT("tag")
#define CO_ATT_VALUE wxT("value")
#define CO_ATT_EVAL wxT("eval")

#define LOGGER "StoredQueriesController"
/**
Defines a specific layout, is recursive to allow splitted cells
*/
GNC::GCS::Condition::Condition(const std::string& tag, const std::string& value, bool eval)
{
        this->tag = tag;
        this->value = value;
        this->eval = eval;
}


GNC::GCS::Condition::Condition():eval(false)
{
}

GNC::GCS::Condition::Condition(wxXmlNode* conditionRoot)
{
        wxString tmp;
        if (conditionRoot->GetAttribute(CO_ATT_TAG, &tmp)) {
                tag = tmp.ToUTF8();
        }
        if (conditionRoot->GetAttribute(CO_ATT_VALUE, &tmp)) {
                value = tmp.ToUTF8();
        }
        eval = conditionRoot->GetAttribute(CO_ATT_EVAL, wxT("true")).CmpNoCase(wxT("true")) == 0;
}

GNC::GCS::Condition::Condition(const Condition& other)
{
        this->tag = other.tag;
        this->value = other.value;
        this->eval = other.eval;
}

GNC::GCS::Condition::~Condition()
{
}

wxXmlNode* GNC::GCS::Condition::serialize()
{
        wxXmlNode* conditionNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, CO_ROOT);
        conditionNode->AddAttribute(CO_ATT_TAG, wxString::FromUTF8(tag.c_str()));
        conditionNode->AddAttribute(CO_ATT_VALUE, wxString::FromUTF8(value.c_str()));
        conditionNode->AddAttribute(CO_ATT_EVAL, eval?wxT("true"):wxT("false"));
        return conditionNode;
}

/**
getters and setters
*/
std::string GNC::GCS::Condition::getTag()
{
        return tag;
}

void GNC::GCS::Condition::setTag(std::string tag)
{
        this->tag = tag;
}

std::string GNC::GCS::Condition::getValue()
{
        return value;
}

void GNC::GCS::Condition::setValue(std::string value)
{
        this->value = value;
}

bool GNC::GCS::Condition::hasToEval()
{
        return eval;
}

void GNC::GCS::Condition::setEval(bool eval)
{
        this->eval = eval;
}

//evaluate condition and insert in query
void GNC::GCS::Condition::addConditionToQuery(GIL::DICOM::DicomDataset& query)
{
        if (eval) {
                //we are going to process syntax relDate{y,m,w,d} --> y = years, m = months, w = weeks, d = days => integer (could be positive or negative)
                wxString wxValue = wxString::FromUTF8(value.c_str());
                wxRegEx regExDate(wxT("relDate\\{([-]?[0123456789]+),([-]?[0123456789]+),([-]?[0123456789]+),([-]?[0123456789]+)\\}"));
                if (regExDate.Matches(wxValue)) {
                        bool isOk = true;
                        long y,m,w,d;
                        isOk &= regExDate.GetMatch(wxValue, 1).ToLong(&y);
                        isOk &= regExDate.GetMatch(wxValue, 2).ToLong(&m);
                        isOk &= regExDate.GetMatch(wxValue, 3).ToLong(&w);
                        isOk &= regExDate.GetMatch(wxValue, 4).ToLong(&d);
                        if (isOk) {
                                query.tags[tag] = wxDateTime::Now().Add(wxDateSpan(y, m, w, d)).Format(wxT("%Y%m%d-")).ToUTF8();
                        } else {
                                LOG_ERROR(LOGGER, "Unknown syntax to eval condition " << value);
                        }
                } else {
                        LOG_ERROR(LOGGER, "Unknown syntax to eval condition " << value);
                }
        } else {
                query.tags[tag] = value;
        }
}

/////////////////////////////////////////////////////
GNC::GCS::StoredQuery::StoredQuery():active(false)
{
}

GNC::GCS::StoredQuery::StoredQuery(wxXmlNode* queryNode)
{
        this->name = queryNode->GetAttribute(SQ_ATT_NAME, wxEmptyString).ToUTF8();
        this->pacsId = queryNode->GetAttribute(SQ_ATT_PACSID, wxEmptyString).ToUTF8();
        this->active = queryNode->GetAttribute(SQ_ATT_ACTIVE, wxT("true")).CmpNoCase(wxT("true")) == 0;

        for (wxXmlNode* child = queryNode->GetChildren(); child != NULL; child = child->GetNext()) {
                if (child->GetName().CmpNoCase(COS_ROOT) == 0) {
                        //storedConditions...
                        for (wxXmlNode* conditionChild = child->GetChildren(); conditionChild != NULL; conditionChild = conditionChild->GetNext()) {
                                if (conditionChild->GetName().CmpNoCase(CO_ROOT) == 0) {
                                        GNC::GCS::Ptr<Condition> pCondition(new Condition(conditionChild));
                                        this->addCondition(pCondition);
                                }
                        }
                } else {
                        LOG_ERROR(LOGGER, "error deserializing stored condition, unknown node " << child->GetName().ToUTF8());
                }
        }
}

GNC::GCS::StoredQuery::StoredQuery(const GNC::GCS::StoredQuery& other)
{
        this->name = other.name;
        this->pacsId = other.pacsId;
        this->active = other.active;

        for (GNC::GCS::StoredQuery::TMapOfConditions::const_iterator it = other.conditions.begin(); it != other.conditions.end(); ++it) {
                if ((*it).second.IsValid()) {
                        GNC::GCS::Ptr<GNC::GCS::Condition> condition(new Condition(*(*it).second));
                        this->conditions[(*it).first] = condition;
                }
        }
}

GNC::GCS::StoredQuery::~StoredQuery()
{
}

wxXmlNode* GNC::GCS::StoredQuery::serialize()
{
        wxXmlNode* storedQueryNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, SQ_ROOT);
        storedQueryNode->AddAttribute(SQ_ATT_NAME, wxString::FromUTF8(this->name.c_str()));
        storedQueryNode->AddAttribute(SQ_ATT_PACSID, wxString::FromUTF8(this->pacsId.c_str()));
        storedQueryNode->AddAttribute(SQ_ATT_ACTIVE, active?wxT("true"):wxT("false"));

        wxXmlNode* conditionsNode = new wxXmlNode(storedQueryNode, wxXML_ELEMENT_NODE, COS_ROOT);
        for (GNC::GCS::StoredQuery::TMapOfConditions::const_iterator it = conditions.begin(); it != conditions.end(); ++it) {
                if ((*it).second.IsValid()) {
                        conditionsNode->AddChild((*it).second->serialize());
                }
        }

        return storedQueryNode;
}

/**
inserts conditions into query
*/
void GNC::GCS::StoredQuery::buildQuery(GIL::DICOM::DicomDataset& query)
{
        for (GNC::GCS::StoredQuery::TMapOfConditions::const_iterator it = conditions.begin(); it != conditions.end(); ++it) {
                (*it).second->addConditionToQuery(query);
        }
}

/**
Getters and setters
*/
const std::string& GNC::GCS::StoredQuery::getName()
{
        return this->name;
}

void GNC::GCS::StoredQuery::setName(const std::string& name)
{
        this->name = name;
}

const std::string& GNC::GCS::StoredQuery::getPacsId()
{
        return this->pacsId;
}

void GNC::GCS::StoredQuery::setPacsId(const std::string& pacsId)
{
        this->pacsId = pacsId;
}

bool GNC::GCS::StoredQuery::isActive()
{
        return this->active;
}

void GNC::GCS::StoredQuery::setActive(bool active)
{
        this->active = active;
}

void GNC::GCS::StoredQuery::addCondition(const GNC::GCS::Ptr<Condition>& condition)
{
        if (condition.IsValid()) {
                this->conditions[condition->getTag()] = condition;
        }
}

GNC::GCS::Ptr<GNC::GCS::Condition>  GNC::GCS::StoredQuery::getCondition(const std::string& tagValue)
{
        GNC::GCS::Ptr<GNC::GCS::Condition> condition;
        if (conditions.find(tagValue) != conditions.end()) {
                condition = conditions[tagValue];
        }
        return condition;
}

bool GNC::GCS::StoredQuery::hasCondition(const std::string& tagValue)
{
        if (conditions.find(tagValue) != conditions.end()) {
                return true;
        } else {
                return false;
        }
}

void GNC::GCS::StoredQuery::clearConditions()
{
        conditions.clear();
}

void GNC::GCS::StoredQuery::addCondition(const std::string& tag, const std::string& value)
{
        GNC::GCS::Ptr<GNC::GCS::Condition> condition(new Condition(tag, value, false));
        this->conditions[tag] = condition;
}

void GNC::GCS::StoredQuery::addConditionIfNotExists(const std::string& tag, const std::string& value)
{
        if (conditions.find(tag) == conditions.end()) {
                GNC::GCS::Ptr<GNC::GCS::Condition> condition(new Condition(tag, value, false));
                this->conditions[tag] = condition;
        }
}

void GNC::GCS::StoredQuery::addSpecialCondition(TSpecialConditions condition)
{
        //delete study date and time...
        if (conditions.find(GKDCM_StudyDate) != conditions.end()) {
                conditions.erase(conditions.find(GKDCM_StudyDate));
        }
        if (conditions.find(GKDCM_StudyTime) != conditions.end()) {
                conditions.erase(conditions.find(GKDCM_StudyTime));
        }

        GNC::GCS::Ptr<GNC::GCS::Condition> dateCondition(new Condition(GKDCM_StudyDate, "", true));
        GNC::GCS::Ptr<GNC::GCS::Condition> timeCondition(new Condition(GKDCM_StudyTime, "", false));
        switch (condition) {
        case TSC_TODAY_MORNING: {
                dateCondition->setValue("relDate{0,0,0,0}");
                timeCondition->setValue("000000-115959");
        }
        break;
        case TSC_TODAY_AFTERNOON: {
                dateCondition->setValue("relDate{0,0,0,0}");
                timeCondition->setValue("120000-235959");
        }
        break;
        case TSC_TODAY: {
                dateCondition->setValue("relDate{0,0,0,0}");
        }
        break;
        case TSC_YESTERDAY: {
                dateCondition->setValue("relDate{0,0,0,-1}");
        }
        break;
        case TSC_LAST_7_DAYS: {
                dateCondition->setValue("relDate{0,0,0,-7}");
        }
        break;
        case TSC_LAST_30_DAYS: {
                dateCondition->setValue("relDate{0,0,0,-30}");
        }
        break;
        case TSC_LAST_3_MONTHS: {
                dateCondition->setValue("relDate{0,-3,0,0}");
        }
        break;
        }
        conditions[GKDCM_StudyDate] = dateCondition;
        if (!timeCondition->getValue().empty()) {
                conditions[GKDCM_StudyTime] = timeCondition;
        }
}

bool GNC::GCS::StoredQuery::containsSpecialCondition(TSpecialConditions condition)
{
        GNC::GCS::Ptr<GNC::GCS::Condition> dateCondition;
        GNC::GCS::Ptr<GNC::GCS::Condition> timeCondition;
        if (conditions.find(GKDCM_StudyDate) != conditions.end()) {
                dateCondition = conditions[GKDCM_StudyDate];
        }
        if (conditions.find(GKDCM_StudyTime) != conditions.end()) {
                timeCondition = conditions[GKDCM_StudyTime];
        }

        bool contains = false;
        switch (condition) {
        case TSC_TODAY_MORNING: {
                contains = dateCondition.IsValid() && timeCondition.IsValid() &&
                           dateCondition->getValue() == "relDate{0,0,0,0}" &&
                           timeCondition->getValue() == "000000-115959";
        }
        break;
        case TSC_TODAY_AFTERNOON: {
                contains = dateCondition.IsValid() && timeCondition.IsValid() &&
                           dateCondition->getValue() == "relDate{0,0,0,0}" &&
                           timeCondition->getValue() == "120000-235959";
        }
        break;
        case TSC_TODAY: {
                contains = dateCondition.IsValid() && !timeCondition.IsValid() &&
                           dateCondition->getValue() == "relDate{0,0,0,0}";
        }
        break;
        case TSC_YESTERDAY: {
                contains = dateCondition.IsValid() && !timeCondition.IsValid() &&
                           dateCondition->getValue() == "relDate{0,0,0,-1}";
        }
        break;
        case TSC_LAST_7_DAYS: {
                contains = dateCondition.IsValid() && !timeCondition.IsValid() &&
                           dateCondition->getValue() == "relDate{0,0,0,-7}";
        }
        break;
        case TSC_LAST_30_DAYS: {
                contains = dateCondition.IsValid() && !timeCondition.IsValid() &&
                           dateCondition->getValue() == "relDate{0,0,0,-30}";
        }
        break;
        case TSC_LAST_3_MONTHS: {
                contains = dateCondition.IsValid() && !timeCondition.IsValid() &&
                           dateCondition->getValue() == "relDate{0,-3,0,0}";
        }
        break;
        }
        return contains;
}

bool GNC::GCS::StoredQuery::isEmpty()
{
        return conditions.empty();
}

/**
Controller for hanging protocols
*/
GNC::GCS::StoredQueryController* GNC::GCS::StoredQueryController::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new StoredQueryController();
        }
        return m_pInstance;
}

void GNC::GCS::StoredQueryController::FreeInstance()
{
        if (m_pInstance != NULL) {
                delete m_pInstance;
                m_pInstance = NULL;
        }
}

GNC::GCS::StoredQueryController::StoredQueryController()
{
        LoadFromConfiguration();
}

GNC::GCS::StoredQueryController::~StoredQueryController()
{
}

GNC::GCS::StoredQueryController* GNC::GCS::StoredQueryController::m_pInstance = NULL;


void GNC::GCS::StoredQueryController::StoreStoredQueriesConfiguration()
{
        //serialization...
        wxXmlDocument doc;
        wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,SQ_DOCUMENT_ROOT);
        doc.SetRoot(root);
        {
                GNC::GCS::ILocker lockQueries(this);
                for (TStoredQueriesMap::const_iterator it = this->mapOfQueries.begin(); it != this->mapOfQueries.end(); ++it) {
                        root->AddChild((*it).second->serialize());
                }
        }
        wxMemoryOutputStream out;
        doc.SetFileEncoding(wxT("UTF-8"));
        doc.Save(out,0);
        char* vectorWidgets = new char[out.GetLength()];
        out.CopyTo(vectorWidgets,out.GetLength());
        GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/StoredQueries","QueriesList", std::string(vectorWidgets, out.GetLength()));
        delete[] vectorWidgets;
}

GNC::GCS::StoredQueryController::TStoredQueriesList GNC::GCS::StoredQueryController::getStoredQueries()
{
        TStoredQueriesList  returningList;
        getActiveQueries(returningList, false);
        return returningList;
}

GNC::GCS::StoredQueryController::TStoredQueriesList GNC::GCS::StoredQueryController::getActiveQueries()
{
        TStoredQueriesList  returningList;
        getActiveQueries(returningList, true);
        return returningList;
}

GNC::GCS::Ptr<GNC::GCS::StoredQuery> GNC::GCS::StoredQueryController::getQuery(const std::string& queryId)
{
        GNC::GCS::Ptr<GNC::GCS::StoredQuery>pReturn;
        {
                GNC::GCS::ILocker lockQueries(this);
                if (mapOfQueries.find(queryId) != mapOfQueries.end()) {
                        pReturn = new StoredQuery(*mapOfQueries[queryId]);
                }
        }
        return pReturn;
}

void GNC::GCS::StoredQueryController::removeQuery(const std::string& queryId)
{
        {
                GNC::GCS::ILocker lockQueries(this);
                if (mapOfQueries.find(queryId) != mapOfQueries.end()) {
                        mapOfQueries.erase(mapOfQueries.find(queryId));
                }
        }
        StoreStoredQueriesConfiguration();
}

void GNC::GCS::StoredQueryController::storeQuery(const GNC::GCS::Ptr<StoredQuery>& pQuery)
{
        {
                GNC::GCS::ILocker lockQueries(this);
                if (pQuery.IsValid()) {
                        mapOfQueries[pQuery->getName()] = pQuery;
                }
        }
        StoreStoredQueriesConfiguration();
}

void GNC::GCS::StoredQueryController::storeQueries(const TStoredQueriesList& queries)
{
        {
                GNC::GCS::ILocker lockQueries(this);
                for (TStoredQueriesList::const_iterator it = queries.begin(); it != queries.end(); ++it) {
                        mapOfQueries[(*it)->getName()] = (*it);
                }
        }
        StoreStoredQueriesConfiguration();
}

void GNC::GCS::StoredQueryController::clearQueries()
{
        GNC::GCS::ILocker lockQueries(this);
        mapOfQueries.clear();
}

void GNC::GCS::StoredQueryController::getActiveQueries(TStoredQueriesList& list, bool onlyActive)
{
        GNC::GCS::ILocker lockQueries(this);
        for (TStoredQueriesMap::const_iterator it = this->mapOfQueries.begin(); it != this->mapOfQueries.end(); ++it) {
                if (!onlyActive || (*it).second->isActive()) {
                        GNC::GCS::Ptr<StoredQuery> pProtocol(new StoredQuery(*(*it).second));
                        list.push_back(pProtocol);
                }
        }
}

void GNC::GCS::StoredQueryController::LoadFromConfiguration()
{
        GNC::GCS::ILocker lockQueries(this);
        //Deserialize...
        this->mapOfQueries.clear();
        std::string serializedValue;
        GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/StoredQueries","QueriesList", serializedValue);
        if (!serializedValue.empty()) {
                wxString wxSerializedValue = wxString::FromUTF8(serializedValue.c_str());
                wxStringInputStream istream(wxSerializedValue);
                wxXmlDocument doc;
                if (doc.Load(istream)) {
                        wxXmlNode* root = doc.GetRoot();
                        if (root != NULL && root->GetName().CmpNoCase(SQ_DOCUMENT_ROOT) == 0) {
                                for (wxXmlNode* sqNode = root->GetChildren(); sqNode != NULL; sqNode = sqNode->GetNext()) {
                                        if (sqNode->GetName().CmpNoCase(SQ_ROOT) == 0) {
                                                GNC::GCS::Ptr<StoredQuery> storedQuery(new StoredQuery(sqNode));
                                                this->mapOfQueries[storedQuery->getName()] = storedQuery;
                                        }
                                }
                        }
                } else {
                        LOG_ERROR(LOGGER, "Error deserializing storedQuery");
                }
        }
}




