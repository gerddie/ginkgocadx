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


#include "hangingprotocolcontroller.h"
#include <limits> 
#include <iomanip>
#include <iostream>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/dicommanager.h>
#include <api/dicom/dicomdataset.h>
#include <api/dicom/dcmdictionary.h>
#include <api/ivista.h>
#include <api/internationalization/internationalization.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/entorno.h>

#include <wx/xml/xml.h>
#include <wx/mstream.h>
#include <wx/sstream.h>
#include <wx/regex.h>
#include <wx/sizer.h>
#include <wx/display.h>

#define HP_DOCUMENT_ROOT wxT("hanging-protocols")

#define PROPERTY_NODE wxT("property")
#define PROPERTY_KEY wxT("key")
#define PROPERTY_VALUE wxT("value")

#define HP_ROOT wxT("hanging-protocol")
#define HP_ATT_NAME wxT("name")
#define HP_ATT_DESCRIPTION wxT("description")
#define HP_ATT_MODULE_SID wxT("module-sid")
#define HP_ATT_MODE_ID wxT("mode")
#define HP_ATT_ACTIVE wxT("active")

#define LOS_ROOT wxT("layouts")
#define LO_ROOT wxT("layout")
#define LO_ATT_ROWS wxT("rows")
#define LO_ATT_COLUMNS wxT("columns")
#define LO_ATT_DETACHED wxT("detached")
#define LO_ATT_MAXIMIZED wxT("maximized")
#define LO_ATT_X_POS wxT("x-pos")
#define LO_ATT_Y_POS wxT("y-pos")
#define LO_ATT_WIDTH wxT("width")
#define LO_ATT_HEIGHT wxT("height")

#define MC_ROOT wxT("matcher")
#define MC_ATT_CRITERIA wxT("criteria")

/**
Class that evaluates study and series matching, depending on matching criteria over tags
*/
GNC::GCS::Matcher::Matcher()
{
}

GNC::GCS::Matcher::Matcher(wxXmlNode* matcherNode)
{
	for (wxXmlNode* child = matcherNode->GetChildren(); child != NULL; child = child->GetNext())
	{
		if (child->GetName().CmpNoCase(MC_ATT_CRITERIA) == 0) {
			for (wxXmlNode* prop = child->GetChildren(); prop != NULL; prop = prop->GetNext()) 
			{
				if (prop->GetName().CmpNoCase(PROPERTY_NODE) == 0) {
					const wxString key = prop->GetAttribute(PROPERTY_KEY, wxEmptyString);
					const wxString value = prop->GetAttribute(PROPERTY_VALUE, wxEmptyString);
					if (!key.IsEmpty()) {
						this->matchingCriteria[std::string(key.ToUTF8())] = value.ToUTF8();
					}
				} else {
					LOG_ERROR("Matcher", "error deserializing matching criteria of Matcher");
				}
			}
		} else {
			LOG_ERROR("Matcher", "error deserializing Matcher, unknown node " << child->GetName().ToUTF8());
		}
	}	
}

GNC::GCS::Matcher::Matcher(const GNC::GCS::Matcher& other)
{
	this->matchingCriteria = other.matchingCriteria;
}

GNC::GCS::Matcher::~Matcher()
{
}

bool GNC::GCS::Matcher::MatchesStudy(long idStudy)
{	
	//tries with study tags from database...
	GIL::DICOM::DicomDataset databaseDataset;
	GNC::GCS::HistoryController::Instance()->GetDatabaseDatasetFromStudy(idStudy, databaseDataset);

	std::string tmpValue;
	for (std::map<std::string, std::string>::const_iterator it = this->matchingCriteria.begin(); it != this->matchingCriteria.end(); ++it)
	{
		if (databaseDataset.getTag((*it).first, tmpValue)) {
			wxRegEx regularExpresion(wxString::FromUTF8((*it).second.c_str()), wxRE_ICASE);
			if (!regularExpresion.Matches(wxString::FromUTF8(tmpValue.c_str()))) {
				return false;
			}
		} else {
			//this attribute isn't in database, so we have to parse a file from the study....
			GNC::GCS::IHistoryController::SeriesModelList seriesList;
			GNC::GCS::HistoryController::Instance()->GetSeriesFromStudy(idStudy, seriesList);
			if (!seriesList.empty()) {
				GNC::GCS::IHistoryController::FileModel file = GNC::GCS::HistoryController::Instance()->GetFrameOfReference(seriesList.front().pk);
				return matches(file.real_path);
			}				
		}
	}
	return true;
}
bool GNC::GCS::Matcher::MatchesSeries(long idSeries)
{
	//tries with study tags from database...
	GIL::DICOM::DicomDataset databaseDataset;
	GNC::GCS::HistoryController::Instance()->GetDatabaseDatasetFromSeries(idSeries, databaseDataset);

	std::string tmpValue;
	for (std::map<std::string, std::string>::const_iterator it = this->matchingCriteria.begin(); it != this->matchingCriteria.end(); ++it)
	{
		if (databaseDataset.getTag((*it).first, tmpValue)) {
			wxRegEx regularExpresion(wxString::FromUTF8((*it).second.c_str()), wxRE_ICASE);
			if (!regularExpresion.Matches(wxString::FromUTF8(tmpValue.c_str()))) {
				return false;
			}
		} else {
			//this attribute isn't in database, so we have to parse a file from the study....
			GNC::GCS::IHistoryController::FileModel file = GNC::GCS::HistoryController::Instance()->GetFrameOfReference(idSeries);
			return matches(file.real_path);				
		}
	}
	return true;
}

bool GNC::GCS::Matcher::matches(const std::string& pathOfFile)
{
	GIL::DICOM::DICOMManager manager;
	GIL::DICOM::DicomDataset base;
	manager.CargarFichero(pathOfFile, base);
	std::string tmpValue;
	for (std::map<std::string, std::string>::const_iterator it = this->matchingCriteria.begin(); it != this->matchingCriteria.end(); ++it)
	{
		if (base.getTag((*it).first, tmpValue)) {
			wxRegEx regularExpresion(wxString::FromUTF8((*it).second.c_str()));
			if (!regularExpresion.Matches(wxString::FromUTF8(tmpValue.c_str()))) {
				return false;
			}
		} else {
			LOG_DEBUG("Matcher", "study doesn't contains " << (*it).first << " attribute");
			return false;
		}
	}
	return true;
}

wxXmlNode* GNC::GCS::Matcher::serialize()
{
	wxXmlNode* matcherNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, MC_ROOT);
	
	wxXmlNode* matchingCriteria = new wxXmlNode(matcherNode, wxXML_ELEMENT_NODE, MC_ATT_CRITERIA);
	for (std::map<std::string, std::string>::const_iterator it = this->matchingCriteria.begin(); it != this->matchingCriteria.end(); ++it)
	{
		wxXmlNode* propertyNode = new wxXmlNode(matchingCriteria, wxXML_ELEMENT_NODE, PROPERTY_NODE);
		propertyNode->AddAttribute(PROPERTY_KEY, wxString::FromUTF8((*it).first.c_str()));
		propertyNode->AddAttribute(PROPERTY_VALUE, wxString::FromUTF8((*it).second.c_str()));
	}

	return matcherNode;
}

std::map<std::string, std::string>& GNC::GCS::Matcher::getMatchingCriteria()
{
	return this->matchingCriteria;
}

void GNC::GCS::Matcher::setMatchingCriteria(const std::map<std::string, std::string>& criteria)
{
	this->matchingCriteria = criteria;
}


/**
Defines a specific layout, is recursive to allow splitted cells
*/
GNC::GCS::Layout::Layout():rows(1),
    columns(1),
    pParent(nullptr),
    isDetached(false),
    isMaximized(false),
    xPos(0), yPos(0),
    width(1), height(1)
{
}

GNC::GCS::Layout::Layout(wxXmlNode* layoutRoot):Layout()
{
	long tmp;
	if (layoutRoot->GetAttribute(LO_ATT_ROWS, wxT("1")).ToLong(&tmp)) {
		rows = tmp;
	}
	if (layoutRoot->GetAttribute(LO_ATT_COLUMNS, wxT("1")).ToLong(&tmp)) {
		columns = tmp;
	}
	wxString foo;
	if (layoutRoot->GetAttribute(LO_ATT_DETACHED, &foo)) {
		this->isDetached = true;
		isMaximized = layoutRoot->GetAttribute(LO_ATT_MAXIMIZED, &foo);
		if (layoutRoot->GetAttribute(LO_ATT_X_POS, wxT("-1")).ToLong(&tmp)) {
			xPos = tmp;
		}
		if (layoutRoot->GetAttribute(LO_ATT_Y_POS, wxT("-1")).ToLong(&tmp)) {
			yPos = tmp;
		}
		if (layoutRoot->GetAttribute(LO_ATT_WIDTH, wxT("-1")).ToLong(&tmp)) {
			width = tmp;
		}
		if (layoutRoot->GetAttribute(LO_ATT_HEIGHT, wxT("-1")).ToLong(&tmp)) {
			height = tmp;
		}
	} else {
		this->isDetached = false;
	}

	for (wxXmlNode* child = layoutRoot->GetChildren(); child != NULL; child = child->GetNext())
	{
		if (child->GetName().CmpNoCase(LO_ROOT) == 0) {
			GNC::GCS::Ptr<Layout> childrenLayout(new Layout(child));
			cellList.push_back(childrenLayout);
		} else if (child->GetName().CmpNoCase(MC_ROOT) == 0) {
			seriesMatcher = new Matcher(child);
		} else if (child->GetName().CmpNoCase(PROPERTY_NODE) == 0) {
			const wxString key = child->GetAttribute(PROPERTY_KEY, wxEmptyString);
			const wxString value = child->GetAttribute(PROPERTY_VALUE, wxEmptyString);
			if (!key.IsEmpty()) {
				this->properties[std::string(key.ToUTF8())] = value.ToUTF8();
			}			
		} else {
			LOG_ERROR("Layout", "error deserializing layout, unknown node " << child->GetName().ToUTF8());
		}
	}	
}

GNC::GCS::Layout::Layout(const Layout& other)
{
	for (std::list<GNC::GCS::Ptr<GNC::GCS::Layout> >::const_iterator it = other.cellList.begin(); it != other.cellList.end(); it++)
	{
		GNC::GCS::Ptr<GNC::GCS::Layout> layout(new Layout(*(*it)));
		this->cellList.push_back(layout);
	}
	if (other.seriesMatcher.IsValid()) {
		GNC::GCS::Ptr<GNC::GCS::Matcher> matcher(new Matcher(*other.seriesMatcher));
		this->seriesMatcher = matcher;
	}
	this->properties = other.properties;
	this->columns = other.columns;
	this->rows = other.rows;
	this->isDetached = other.isDetached;
	this->isMaximized = other.isMaximized;
	this->xPos = other.xPos;
	this->yPos = other.yPos;
	this->width = other.width;
	this->height = other.height;
    pParent = other.pParent;
}

GNC::GCS::Layout::~Layout()
{
}
/**
Layout with a row and a column is a cell and cellList is empty
*/
bool GNC::GCS::Layout::isACell()
{
	return rows == 1 && columns == 1;
}

wxXmlNode* GNC::GCS::Layout::serialize()
{
	wxXmlNode* layoutNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, LO_ROOT);
	layoutNode->AddAttribute(LO_ATT_ROWS, wxString::Format(wxT("%d"), (int)(this->rows)));
	layoutNode->AddAttribute(LO_ATT_COLUMNS, wxString::Format(wxT("%d"), (int)(this->columns)));
	for (std::list<GNC::GCS::Ptr<Layout> >::const_iterator it = this->cellList.begin(); it != this->cellList.end(); ++it)
	{
		if ((*it).IsValid()) {
			layoutNode->AddChild((*it)->serialize());
		}
	}

	if (seriesMatcher.IsValid()) {
		layoutNode->AddChild(seriesMatcher->serialize());
	}

	for (std::map<std::string, std::string>::const_iterator it = this->properties.begin(); it != this->properties.end(); ++it)
	{
		wxXmlNode* propertyNode = new wxXmlNode(layoutNode, wxXML_ELEMENT_NODE, PROPERTY_NODE);
		propertyNode->AddAttribute(PROPERTY_KEY, wxString::FromUTF8((*it).first.c_str()));
		propertyNode->AddAttribute(PROPERTY_VALUE, wxString::FromUTF8((*it).second.c_str()));
	}

	if (isDetached) {
		layoutNode->AddAttribute(LO_ATT_DETACHED, wxEmptyString);
		if (isMaximized) {
			layoutNode->AddAttribute(LO_ATT_MAXIMIZED, wxEmptyString);
		}
		layoutNode->AddAttribute(LO_ATT_X_POS, wxString::Format(wxT("%d"), (int)(this->xPos)));
		layoutNode->AddAttribute(LO_ATT_Y_POS, wxString::Format(wxT("%d"), (int)(this->yPos)));
		layoutNode->AddAttribute(LO_ATT_WIDTH, wxString::Format(wxT("%d"), (int)(this->width)));
		layoutNode->AddAttribute(LO_ATT_HEIGHT, wxString::Format(wxT("%d"), (int)(this->height)));
	}

	return layoutNode;
}

void GNC::GCS::Layout::doLayout(IHangingProtocolContract* pContract, wxSizer* parentSizer)
{
	if (isACell()) {
		//create cell...
		long pk = -1;
		if (seriesMatcher.IsValid()){
			const std::list<long>& seriesId = pContract->getOpeningSeries();
			for (std::list<long>::const_iterator it = seriesId.begin(); it != seriesId.end(); ++it) {
				if (seriesMatcher->MatchesSeries((*it))) {
					pk = (*it);
					break;
				}
			}
		}
		//
		parentSizer->Add(pContract->createSeriesSlot(this->properties, pk, seriesMatcher), 1, wxEXPAND|wxALL);
	} else {
		//parent sizer has to have vertical layout...
		wxBoxSizer* pParentBox = dynamic_cast<wxBoxSizer*>(parentSizer);
		if (pParentBox == NULL || pParentBox->GetOrientation() != wxVERTICAL) {
			wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
			parentSizer->Add(verticalSizer, 1, wxEXPAND|wxALL);
			parentSizer = verticalSizer;
		}
		//
		std::list<GNC::GCS::Ptr<Layout> >::const_iterator itLayouts = this->cellList.begin();
		for (int i = 0; i < this->rows; i++) {
			wxBoxSizer* pRow = new wxBoxSizer(wxHORIZONTAL);
			for (int j = 0; j < this->columns; j++) {
				if (itLayouts != this->cellList.end()) {
					(*itLayouts)->doLayout(pContract, pRow);
					++itLayouts;
				} else {
					pRow->Add(pContract->createSeriesSlot(this->properties, -1, seriesMatcher), 1, wxEXPAND|wxALL);
				}
			}
			parentSizer->Add(pRow, 1, wxEXPAND);
		}
	}
}

GNC::GCS::Ptr<GNC::GCS::Layout> GNC::GCS::Layout::getLayoutFromSizerItem(wxSizerItem* pSizerItem)
{
	GNC::GCS::Ptr<GNC::GCS::Layout> pLayout(new GNC::GCS::Layout());
	//it's a cell
	if (pSizerItem->GetWindow() != NULL) {
		GNC::GCS::IHangingSeriesViewer* seriesViewerMatcher = dynamic_cast<GNC::GCS::IHangingSeriesViewer*>(pSizerItem->GetWindow());
		if (seriesViewerMatcher != NULL) {
			//build series matcher...
			GIL::DICOM::DicomDataset base;
			seriesViewerMatcher->getSeriesBase(base);
			GNC::GCS::Ptr<GNC::GCS::Matcher> pMatcher(new GNC::GCS::Matcher);
			for (GIL::DICOM::ListaTags::iterator it = base.tags.begin(); it != base.tags.end(); it++) {
				pMatcher->getMatchingCriteria()[(*it).first] = (*it).second;
			}
			pLayout->setMatcher(pMatcher);
		}
	} else if (pSizerItem->GetSizer() != NULL) {
		//count rows and columns...
		wxBoxSizer* pBoxPpal = dynamic_cast<wxBoxSizer*>(pSizerItem->GetSizer());
		if (pBoxPpal != NULL) {
			int elements = 0;
			for (wxSizerItemList::const_iterator itItems = pBoxPpal->GetChildren().begin(); itItems != pBoxPpal->GetChildren().end(); itItems++, elements++)
			{
				pLayout->cellList.push_back(getLayoutFromSizerItem((*itItems)));
			}
			if (pBoxPpal->GetOrientation() == wxVERTICAL) {
				pLayout->setRows(elements);
			} else {
				pLayout->setColumns(elements);
			}
			//if layout is 1x1 with one child point to this child
			if (pLayout->isACell() && pLayout->cellList.size() == 1) {				
				GNC::GCS::Ptr<GNC::GCS::Layout> tmp = pLayout->cellList.front();
				pLayout = tmp;
			}
		} else {
			LOG_ERROR("Layout::getLayoutFromSizer", "there is a sizer that isn't a box sizer");
		}
	} else {
		LOG_ERROR("Layout::getLayoutFromSizer", "there is a item that isn't a winwow or a sizer");
	}
	return pLayout;
}

GNC::GCS::Ptr<GNC::GCS::Layout> GNC::GCS::Layout::getLayoutFromSizer(wxSizer* pSizer)
{
	GNC::GCS::Ptr<GNC::GCS::Layout> pLayout(new GNC::GCS::Layout());
	wxBoxSizer* pBoxPpal = dynamic_cast<wxBoxSizer*>(pSizer);
	if (pBoxPpal != NULL) {
		//count rows and columns...
		wxBoxSizer* pBoxPpal = dynamic_cast<wxBoxSizer*>(pSizer);
		if (pBoxPpal != NULL) {
			int elements = 0;
			for (wxSizerItemList::const_iterator itItems = pBoxPpal->GetChildren().begin(); itItems != pBoxPpal->GetChildren().end(); itItems++, elements++)
			{
				pLayout->cellList.push_back(getLayoutFromSizerItem((*itItems)));
			}
			if (pBoxPpal->GetOrientation() == wxVERTICAL) {
				pLayout->setRows(elements);
			} else {
				pLayout->setColumns(elements);
			}
			//if layout is 1x1 with one child point to this child
			if (pLayout->isACell() && pLayout->cellList.size() == 1) {
				GNC::GCS::Ptr<GNC::GCS::Layout> tmp = pLayout->cellList.front();
				pLayout = tmp;
			}
		} else {
			LOG_ERROR("Layout::getLayoutFromSizer", "there is a sizer that isn't a box sizer");
		}
	} else {
		LOG_ERROR("Layout::getLayoutFromSizer", "there is a sizer that isn't a box sizer");
	}
	return pLayout;
}

int GNC::GCS::Layout::getRows()
{
	return this->rows;
}

void GNC::GCS::Layout::setRows(int rows)
{
	this->rows = rows;
}

int GNC::GCS::Layout::getColumns()
{
	return this->columns;
}

void GNC::GCS::Layout::setColumns(int columns)
{
	this->columns = columns;
}

std::list<GNC::GCS::Ptr<GNC::GCS::Layout> >& GNC::GCS::Layout::getCellList()
{
	return cellList;
}

void GNC::GCS::Layout::setCellList(const std::list<GNC::GCS::Ptr<GNC::GCS::Layout> >& cellList)
{
	this->cellList = cellList;
}

const GNC::GCS::Ptr<GNC::GCS::Matcher>& GNC::GCS::Layout::getMatcher()
{
	return seriesMatcher;
}

void GNC::GCS::Layout::setMatcher(const GNC::GCS::Ptr<GNC::GCS::Matcher>& matcher)
{
	this->seriesMatcher = matcher;
}


std::map<std::string, std::string>& GNC::GCS::Layout::getProperties()
{
	return this->properties;
}

void GNC::GCS::Layout::setProperties(const std::map<std::string, std::string>& properties)
{
	this->properties = properties;
}

GNC::GCS::HangingProtocol* GNC::GCS::Layout::getParent()
{
	return pParent;
}

void GNC::GCS::Layout::setParent(GNC::GCS::HangingProtocol* pH)
{
	pParent = pH;
}

void GNC::GCS::Layout::setDefaultPosition()
{
	isDetached = false;
}

void GNC::GCS::Layout::setWindowProperties(bool isMaximized, int xPos, int yPos, int width, int height)
{
	this->isDetached = true;
	this->isMaximized = isMaximized;
	this->xPos = xPos;
	this->yPos = yPos;
	this->width = width;
	this->height = height;
}

void GNC::GCS::Layout::copyWindowProperties(const GNC::GCS::Layout& other)
{
	this->isDetached = other.isDetached;
	this->isMaximized = other.isMaximized;
	this->xPos = other.xPos;
	this->yPos = other.yPos;
	this->width = other.width;
	this->height = other.height;
}

void GNC::GCS::Layout::applyWindowProperties(wxWindow* pWindow)
{
	if (isDetached) {
		int maxX = 0, maxY = 0, minX=std::numeric_limits<int>::max(), minY=std::numeric_limits<int>::max();
		int deviceCount = wxDisplay::GetCount();
		for (int i = 0; i < deviceCount; ++i) {
			wxDisplay dsply(i);
			maxX = std::max<int>(maxX, dsply.GetClientArea().x + dsply.GetCurrentMode().GetWidth());
			maxY = std::max<int>(maxY, dsply.GetClientArea().y + dsply.GetCurrentMode().GetHeight());
			minX = std::min<int>(minX, dsply.GetClientArea().x);
			minY = std::min<int>(minY, dsply.GetClientArea().y);
		}
		wxPoint position(xPos, yPos);
		if (position.x >= (minX - pWindow->GetMinSize().x) && position.x  < maxX 
			&& position.y >= (minY  - pWindow->GetMinSize().y) && position.y < maxY) {
			pWindow->SetPosition(position);
		}
		wxSize size(width, height);
		wxTopLevelWindow* pTopLevel = dynamic_cast<wxTopLevelWindow*>(pWindow);
		if (pTopLevel != NULL) {
			pTopLevel->Maximize(isMaximized);
			if (!isMaximized) {
				pWindow->SetSize(size);
			}
		} else {
			pWindow->SetSize(size);
		}
		
	}
}

void GNC::GCS::Layout::applyWindowProperties(GNC::GCS::IVista* pView)
{
	//detach view...
	wxWindow* pWindow = pView->GetWindow();
	if(pWindow != NULL) {
		wxWindow* pParentWindow = pWindow->GetParent();
		if(pParentWindow != NULL) {
			wxFrame* pdlg = dynamic_cast<wxFrame*>(pParentWindow);
			if(pdlg != NULL) {
				//it is detached
				if (!isDetached) {
					//undetach
					GNC::Entorno::Instance()->GetVentanaPrincipal()->EncajarDesencajar(pView);
				}
			}
			else {
				//it isn't detached
				if (isDetached) {
					//detach
					GNC::Entorno::Instance()->GetVentanaPrincipal()->EncajarDesencajar(pView);
					//apply properties
					applyWindowProperties(pWindow->GetParent());
				}
			}
		}
	}
}


/////////////////////////////////////////////////////
GNC::GCS::HangingProtocol::HangingProtocol():
    openingMode(0),
    active(true)
{
}

GNC::GCS::HangingProtocol::HangingProtocol(wxXmlNode* hangingNode)
{
	this->name = hangingNode->GetAttribute(HP_ATT_NAME, wxEmptyString).ToUTF8();
	this->description = hangingNode->GetAttribute(HP_ATT_DESCRIPTION, wxEmptyString).ToUTF8();
	this->moduleSid = hangingNode->GetAttribute(HP_ATT_MODULE_SID, wxEmptyString).ToUTF8();
	this->active = hangingNode->GetAttribute(HP_ATT_ACTIVE, wxT("true")).CmpNoCase(wxT("true")) == 0;
	long tmpLong;
	if(hangingNode->GetAttribute(HP_ATT_MODE_ID, wxEmptyString).ToLong(&tmpLong)) {
		this->openingMode = tmpLong;
	} else {
		this->openingMode = 0;
	}
	for (wxXmlNode* child = hangingNode->GetChildren(); child != NULL; child = child->GetNext())
	{
		if (child->GetName().CmpNoCase(LOS_ROOT) == 0) {
			//layouts...
			for (wxXmlNode* layoutChild = child->GetChildren(); layoutChild != NULL; layoutChild = layoutChild->GetNext()) {
				if (layoutChild->GetName().CmpNoCase(LO_ROOT) == 0) {
					GNC::GCS::Ptr<Layout> pLayout(new Layout(layoutChild));
					this->addLayout(pLayout);
				}
			}			
		} else if (child->GetName().CmpNoCase(LO_ROOT) == 0)  {
			//retrocompatib...
			GNC::GCS::Ptr<Layout> pLayout(new Layout(child));
			this->addLayout(pLayout);
		}  else if (child->GetName().CmpNoCase(MC_ROOT) == 0) {
			this->studyMatcher = new Matcher(child);
		} else if (child->GetName().CmpNoCase(PROPERTY_NODE) == 0) {
			const wxString key = child->GetAttribute(PROPERTY_KEY, wxEmptyString);
			const wxString value = child->GetAttribute(PROPERTY_VALUE, wxEmptyString);
			if (!key.IsEmpty()) {
				this->properties[std::string(key.ToUTF8())] = value.ToUTF8();
			}			
		} else {
			LOG_ERROR("HangingProtocol", "error deserializing hanging protocol, unknown node " << child->GetName().ToUTF8());
		}
	}
}

GNC::GCS::HangingProtocol::HangingProtocol(const GNC::GCS::HangingProtocol& other)
{
	this->name = other.name;
	this->description = other.description;
	this->moduleSid = other.moduleSid;
	this->openingMode = other.openingMode;
	if (other.studyMatcher.IsValid()) {
		this->studyMatcher = new Matcher(*other.studyMatcher);
	}
	for (GNC::GCS::HangingProtocol::TListOfLayouts::const_iterator it = other.layouts.begin(); it != other.layouts.end(); ++it) {
		if ((*it).IsValid()) {
			GNC::GCS::Ptr<GNC::GCS::Layout> layout(new Layout(*(*it)));
			this->layouts.push_back(layout);
		}
	}
	this->properties = other.properties;
	this->active = other.active;
}

GNC::GCS::HangingProtocol::~HangingProtocol()
{
	active = true;
}


wxXmlNode* GNC::GCS::HangingProtocol::serialize()
{
	wxXmlNode* hangingNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, HP_ROOT);
	hangingNode->AddAttribute(HP_ATT_NAME, wxString::FromUTF8(this->name.c_str()));
	hangingNode->AddAttribute(HP_ATT_DESCRIPTION, wxString::FromUTF8(this->description.c_str()));
	hangingNode->AddAttribute(HP_ATT_MODULE_SID, wxString::FromUTF8(this->moduleSid.c_str()));
	hangingNode->AddAttribute(HP_ATT_MODE_ID, wxString::Format(wxT("%d"), (int) this->openingMode));
	hangingNode->AddAttribute(HP_ATT_ACTIVE, active?wxT("true"):wxT("false"));

	wxXmlNode* layoutsNode = new wxXmlNode(hangingNode, wxXML_ELEMENT_NODE, LOS_ROOT);
	for (GNC::GCS::HangingProtocol::TListOfLayouts::const_iterator it = layouts.begin(); it != layouts.end(); ++it) {
		if ((*it).IsValid()) {
			layoutsNode->AddChild((*it)->serialize());
		}
	} 
	if (this->studyMatcher.IsValid()) {
		hangingNode->AddChild(this->studyMatcher->serialize());
	}
	
	for (std::map<std::string, std::string>::const_iterator it = this->properties.begin(); it != this->properties.end(); ++it)
	{
		wxXmlNode* propertyNode = new wxXmlNode(hangingNode, wxXML_ELEMENT_NODE, PROPERTY_NODE);
		propertyNode->AddAttribute(PROPERTY_KEY, wxString::FromUTF8((*it).first.c_str()));
		propertyNode->AddAttribute(PROPERTY_VALUE, wxString::FromUTF8((*it).second.c_str()));
	}

	return hangingNode;
}

bool GNC::GCS::HangingProtocol::matches(long idStudy)
{
	if (this->studyMatcher.IsValid()) {
		return this->studyMatcher->MatchesStudy(idStudy);
	}
	LOG_WARN("HangingProtocol", "there is a hanging protocol without matcher specified");
	return false;
}


const std::string& GNC::GCS::HangingProtocol::getName()
{
	return this->name;
}

void GNC::GCS::HangingProtocol::setName(const std::string& name)
{
	this->name = name;
}

const std::string& GNC::GCS::HangingProtocol::getDescription()
{
	return this->description;
}

void GNC::GCS::HangingProtocol::setDescription(const std::string& description)
{
	this->description = description;
}

const std::string& GNC::GCS::HangingProtocol::getModuleSid()
{
	return this->moduleSid;
}

void GNC::GCS::HangingProtocol::setModuleSid(const std::string& sid)
{
	this->moduleSid = sid;
}

int GNC::GCS::HangingProtocol::getOpeningMode()
{
	return this->openingMode;
}

void GNC::GCS::HangingProtocol::setOpeningMode(int mode)
{
	this->openingMode = mode;
}

const GNC::GCS::Ptr<GNC::GCS::Matcher>& GNC::GCS::HangingProtocol::getStudyMatcher()
{
	return this->studyMatcher;
}

void GNC::GCS::HangingProtocol::setStudyMatcher(const GNC::GCS::Ptr<GNC::GCS::Matcher>& matcher)
{
	this->studyMatcher = matcher;
}

const GNC::GCS::HangingProtocol::TListOfLayouts& GNC::GCS::HangingProtocol::getLayouts()
{
	return this->layouts;
}

void GNC::GCS::HangingProtocol::addLayout(const GNC::GCS::Ptr<GNC::GCS::Layout>& layout)
{
	if (layout.IsValid()) {
		layout->setParent(this);
		this->layouts.push_back(layout);
	}
}

void GNC::GCS::HangingProtocol::clearLayouts()
{
	this->layouts.clear();
}

std::map<std::string, std::string>& GNC::GCS::HangingProtocol::getProperties()
{
	return this->properties;
}

void GNC::GCS::HangingProtocol::setProperties(const std::map<std::string, std::string>& properties)
{
	this->properties = properties;
}

bool GNC::GCS::HangingProtocol::isActive()
{
	return this->active;
}

void GNC::GCS::HangingProtocol::setActive(bool active)
{
	this->active = active;
}

/**
Controller for hanging protocols
*/
GNC::GCS::HangingProtocolController* GNC::GCS::HangingProtocolController::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new HangingProtocolController();
	}
	return m_pInstance;
}

void GNC::GCS::HangingProtocolController::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

GNC::GCS::HangingProtocolController::HangingProtocolController()
{
	LoadFromConfiguration();
}

GNC::GCS::HangingProtocolController::~HangingProtocolController()
{
}
GNC::GCS::HangingProtocolController* GNC::GCS::HangingProtocolController::m_pInstance = NULL;

GNC::GCS::Ptr<GNC::GCS::HangingProtocol> GNC::GCS::HangingProtocolController::GetProtocolForStudy(long idStudy)
{
	for (std::list<GNC::GCS::Ptr<HangingProtocol> >::const_iterator it = this->listOfProtocols.begin(); it != this->listOfProtocols.end(); ++it)
	{
		if ((*it)->isActive() && (*it)->matches(idStudy)) {
			return (*it);
		}
	}
	//study doesn't match we create a hanging protocol as traditional ginkgo opening mode...

	return NULL;
}

void GNC::GCS::HangingProtocolController::StoreHangingProtocolConfiguration()
{
	//serialization...
	wxXmlDocument doc;
	wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,HP_DOCUMENT_ROOT);
	doc.SetRoot(root);
	for (std::list<GNC::GCS::Ptr<HangingProtocol> >::const_iterator it = this->listOfProtocols.begin(); it != this->listOfProtocols.end(); ++it)
	{
		root->AddChild((*it)->serialize());
	}
	wxMemoryOutputStream out;
	doc.SetFileEncoding(wxT("UTF-8"));
	doc.Save(out,0);
	char* vectorWidgets = new char[out.GetLength()];
	out.CopyTo(vectorWidgets,out.GetLength());
	GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/HangingProtocols","HangingList", std::string(vectorWidgets, out.GetLength()));
	delete[] vectorWidgets;
}

std::list<GNC::GCS::Ptr<GNC::GCS::HangingProtocol> > GNC::GCS::HangingProtocolController::getHangingProtocols()
{	
	LoadFromConfiguration();
	std::list<GNC::GCS::Ptr<GNC::GCS::HangingProtocol> >  returningList;

	for (std::list<GNC::GCS::Ptr<HangingProtocol> >::const_iterator it = this->listOfProtocols.begin(); it != this->listOfProtocols.end(); ++it)
	{
		GNC::GCS::Ptr<HangingProtocol> pProtocol(new HangingProtocol(*(*it)));
		returningList.push_back(pProtocol);
	}

	return returningList;
}
			
void GNC::GCS::HangingProtocolController::setHangingProtocols(std::list<GNC::GCS::Ptr<GNC::GCS::HangingProtocol> > listOfProtocols)
{
	this->listOfProtocols = listOfProtocols;
	StoreHangingProtocolConfiguration();
}

void GNC::GCS::HangingProtocolController::LoadFromConfiguration()
{
	//Deserialize...
	this->listOfProtocols.clear();
	std::string serializedValue;
	GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/HangingProtocols","HangingList", serializedValue);
	if (!serializedValue.empty()) {
		wxString wxSerializedValue = wxString::FromUTF8(serializedValue.c_str());
		wxStringInputStream istream(wxSerializedValue);
		wxXmlDocument doc;
		if (doc.Load(istream)) {
			wxXmlNode* root = doc.GetRoot();
			if (root != NULL && root->GetName().CmpNoCase(HP_DOCUMENT_ROOT) == 0) {
				for (wxXmlNode* hangingProtocolNode = root->GetChildren(); hangingProtocolNode != NULL; hangingProtocolNode = hangingProtocolNode->GetNext())
				{
					if (hangingProtocolNode->GetName().CmpNoCase(HP_ROOT) == 0) {
						GNC::GCS::Ptr<HangingProtocol> hangingProtocol(new HangingProtocol(hangingProtocolNode));
						this->listOfProtocols.push_back(hangingProtocol);
					}
				}
			}
		} else {
			LOG_ERROR("HangingProtocolController", "Error deserializing hanging protocol");
		}
	}
}




