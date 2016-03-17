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

#include <set>
#include <ctime>

#include <wx/checkbox.h>
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>
#include <wx/uri.h>

#include "integrationcontroller.h"
#include "controladorlog.h"
#include "controladorextensiones.h"
#include "controladoreventos.h"
#include "commandcontroller.h"
#include "controladorhl7.h"
#include "configurationcontroller.h"
#include <commands/cloudcommand.h>
#include <commands/pacsdownloadcommand.h>
#include <commands/acquirefromurlcommand.h>
#include <api/integration/dict.h>
#include <api/dicom/dcmdictionary.h>
#include <main/controllers/controladorpermisos.h>
#include <main/gui/import/wxwizardimportacionginkgo.h>
#include <main/gui/acquisition/dialogoadquisicion.h>
#include <main/gui/acquisition/downloadagaindialog.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/entorno.h>
#include <eventos/integration.h>
#include <main/controllers/parserxmlenglish.h>
#include <main/controllers/parserxmlspanish.h>
#include <main/controllers/historycontroller.h>
#include <wx/filename.h>
#include <wx/sstream.h>
#include <wx/tokenzr.h>

#include <main/controllers/anonusagestats.h>

GIL::IntegrationController* GIL::IntegrationController::m_pInstance = 0;

GIL::IXMLIntegrationParser::~IXMLIntegrationParser()
{
}

GIL::IntegrationController::IntegrationController()
{
        GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::HL7SendRequestEvent());

        GNC::GCS::Ptr<GIL::IXMLIntegrationParser> spanish(new GIL::ParserXMLSpanish());
        std::list<std::string> keys = spanish->GetKeys();
        for (std::list<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
                RegisterParser((*it), spanish);
        }
        GNC::GCS::Ptr<GIL::IXMLIntegrationParser> english(new GIL::ParserXMLEnglish());
        keys = english->GetKeys();
        for (std::list<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
                RegisterParser((*it), english);
        }
}

GIL::IntegrationController::~IntegrationController()
{
        GNC::GCS::ControladorEventos::Instance()->DesRegistrar(this);
        m_mapParsers.clear();
}

GIL::IntegrationController* GIL::IntegrationController::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new IntegrationController();
        }
        return m_pInstance;
}

void GIL::IntegrationController::FreeInstance()
{
        if (m_pInstance != NULL) {
                delete m_pInstance;
                m_pInstance = NULL;
        }
}


void GIL::IntegrationController::RegisterParser(const std::string& xmlKey, const GNC::GCS::Ptr<IXMLIntegrationParser>& pParser)
{
        if (m_mapParsers.find(xmlKey) != m_mapParsers.end()) {
                throw IntegrationException(_Std("Fatal error! you can't register two parsers with same  key").c_str());
        }
        m_mapParsers[xmlKey] = pParser;
}

void GIL::IntegrationController::UnRegisterParser(const std::string& xmlKey)
{
        if (m_mapParsers.find(xmlKey) != m_mapParsers.end()) {
                m_mapParsers.erase(m_mapParsers.find(xmlKey));
        }
}



bool GIL::IntegrationController::Process(GIL::IntegrationModelList& models)
{

        unsigned int numProcessed = 0;

        typedef GIL::IntegrationModelList::iterator TIteradorModelos;
        for (TIteradorModelos itModelo = models.begin(); itModelo != models.end(); ++itModelo) {
                GNC::GCS::Ptr<GIL::IModeloIntegracion> modelo = (*itModelo);
                if (modelo->atendido) {
                        numProcessed++;
                        continue;
                } else {
                        modelo->atendido = true;
                }
                if(modelo->accion == GIL::IModeloIntegracion::TA_Dicomizar) {
                        LOG_STAT(ANON_STAT_INT_DICOMIZE)
                        //se busca el dicomizador del modulo indicado, habra un mapa que transformara sid en uids de modulo
                        std::string uidModulo("");
                        if (modelo->idPlantilla != "") {
                                uidModulo = GetUID(modelo->idPlantilla);
                                if (GNC::ControladorExtensiones::Instance()->ObtenerModulo(uidModulo) == NULL) {
                                        throw GIL::IntegrationException("No existe el modulo asociado", "Modelo", true);
                                }
                        }

                        try {
                                GNC::GUI::wxWizardImportacionGinkgo ib(NULL, modelo, uidModulo);
                                ib.ShowModal();
                        } catch(GIL::DICOM::I2DException& i) {
                                LOG_ERROR("GIL/WF/Importar", (const std::string)i);
                        } catch(std::exception& ex1) {
                                LOG_ERROR("GIL/WF/Importar", std::string("Error al importar: ") + ex1.what());
                        } catch (...) {
                                LOG_ERROR("GIL/WF/Importar", std::string("Error al importar: Error interno"));
                        }
                        GNC::Entorno::Instance()->GetVentanaPrincipal()->MostrarVentana();
                        numProcessed++;
                } else if(modelo->accion == GIL::IModeloIntegracion::TA_Obtener) {
                        LOG_STAT(ANON_STAT_INT_RETRIEVE)
                        //if wado retrieve...
                        if (!modelo->Files.empty()) {
                                //retrieve files downloading as wado files using acquirefromurl command
                                GADAPI::AcquireFromURLCommandParameters* pCmdParams = new GADAPI::AcquireFromURLCommandParameters(modelo->Files);
                                GADAPI::AcquireFromURLCommand* pCmd = new GADAPI::AcquireFromURLCommand(pCmdParams);
                                GNC::Entorno::Instance()->GetCommandController()->ProcessAsync("Downloading files...",pCmd, NULL);
                                GNC::Entorno::Instance()->GetVentanaPrincipal()->MostrarVentana();
                                numProcessed++;
                        } else {
                                GADAPI::PACSDownloadCommandParams * pParams = NULL;
                                std::string tag;
                                if (!modelo->TagsDICOMOverwrite.getTag("0008|0052", tag)) {
                                        //show only acquisition form
                                        GNC::Entorno::Instance()->GetVentanaPrincipal()->MostrarVentana();
                                        GNC::GUI::DialogoAdquisicion::Instance()->Show(modelo->GlobalVariables.GetValue(GKDI_PATIENT_ID), modelo->GlobalVariables.GetValue(GKDI_GLOBAL_PACS_RETRIEVE_SID));
                                        numProcessed++;
                                } else {
                                        GIL::DICOM::DicomDataset base = modelo->TagsDICOMOverwrite;
                                        std::string qrlevel, uid;
                                        if (base.getTag(GKDCM_QueryRetrieveLevel,qrlevel)) {
                                                if (qrlevel == "STUDY" && base.getTag(GKDCM_StudyInstanceUID, uid) && GNC::GCS::HistoryController::Instance()->ExistsStudyWithUID(uid)) {
                                                        GNC::GUI::DownloadAgainDialog dlg(GNC::Entorno::Instance()->GetVentanaRaiz(), uid, false);
                                                        int answer = dlg.ShowModal();
                                                        if (answer != wxID_OK) {
                                                                continue;
                                                        }
                                                } else if (qrlevel == "SERIES" && base.getTag(GKDCM_SeriesInstanceUID, uid) && GNC::GCS::HistoryController::Instance()->ExistsSeriesWithUID(uid)) {
                                                        GNC::GUI::DownloadAgainDialog dlg(GNC::Entorno::Instance()->GetVentanaRaiz(), uid, false);
                                                        int answer = dlg.ShowModal();
                                                        if (answer != wxID_OK) {
                                                                continue;
                                                        }
                                                } else {
                                                        LOG_INFO("GIL/WF/Importar", "Unknown GKDCM_QueryRetrieveLevel '"<< qrlevel << "'");
                                                }
                                        } else {
                                                LOG_INFO("GIL/WF/Importar", "unable to aquire GKDCM_QueryRetrieveLevel");
                                        }
                                        pParams = new GADAPI::PACSDownloadCommandParams(modelo->GlobalVariables.GetValue(GKDI_GLOBAL_PACS_RETRIEVE_SID), base);

                                        GADAPI::PACSDownloadCommand* pComandoPACS = new GADAPI::PACSDownloadCommand(pParams);
                                        GNC::Entorno::Instance()->GetCommandController()->ProcessAsync("Descargando PACS...",pComandoPACS, NULL);
                                        GNC::Entorno::Instance()->GetVentanaPrincipal()->MostrarVentana();
                                        numProcessed++;
                                }
                        }
                } else if(modelo->accion == GIL::IModeloIntegracion::TA_Open)  {
                        LOG_STAT(ANON_STAT_INT_OPEN)
                        for (GIL::IModeloIntegracion::ListOfPaths::iterator itFile = modelo->Files.begin(); itFile != modelo->Files.end(); ++itFile) {
                                wxFileName fileName(wxString::FromUTF8((*itFile).c_str()));
                                if (fileName.IsRelative()) {
                                        fileName.MakeAbsolute();
                                }
                                GNC::Entorno::Instance()->GetVentanaPrincipal()->MostrarVentana();
                                GNC::Entorno::Instance()->GetVentanaPrincipal()->EjecutarParametro(fileName.GetFullPath());
                        }
                        numProcessed++;
                }
        }
        return numProcessed == models.size();
}


typedef std::list<const GIL::IMapOfVarsModel*> TMapStack;
typedef std::map<std::string, long> TSeqMap;

void Parse(std::ostringstream& os, GIL::IModeloIntegracion* integrationModel, wxXmlNode* root, GIL::IStudyModel* study, int studyIdx, GIL::ISeriesModel* serie, int serieIdx, int imageIdx, TMapStack& mapStack, TSeqMap& seqMap)
{
        if (integrationModel == NULL || root == NULL) {
                return;
        }
        for (wxXmlNode* child = root->GetChildren(); child != NULL; child = child->GetNext()) {
                if (child->GetName() == wxT("seg")) {
                        for (wxXmlNode* subChild = child->GetChildren(); subChild != NULL; subChild = subChild->GetNext()) {
                                if ( (subChild->GetType() == wxXML_CDATA_SECTION_NODE) || (subChild->GetType() == wxXML_TEXT_NODE) ) {
                                        os << std::string(subChild->GetContent().ToUTF8());
                                } else {
                                        if (subChild->GetName() == wxT("idx")) {
                                                std::string content(subChild->GetAttribute(wxT("expr"), wxEmptyString).ToUTF8());

                                                long acc = 0;

                                                std::string::size_type lastOpIdx = 0;
                                                std::string::size_type currOpIdx;
                                                int currOp = ' ';

                                                std::string term;
                                                bool pending = true;
                                                do {
                                                        std::string::size_type currOpIdx1 = content.find('+', lastOpIdx);
                                                        std::string::size_type currOpIdx2 = content.find('*', lastOpIdx);
                                                        if (currOpIdx1 < currOpIdx2) {
                                                                currOp = '+';
                                                                currOpIdx = currOpIdx1;
                                                        } else {
                                                                currOpIdx = currOpIdx2;
                                                                if (currOpIdx2 == std::string::npos) {
                                                                        currOp = ' ';
                                                                        pending = false;
                                                                } else {
                                                                        currOp = '*';
                                                                }
                                                        }

                                                        term = content.substr(lastOpIdx, currOpIdx);
                                                        long termVal = 0;

                                                        if (term == "study:idx") {
                                                                termVal = studyIdx;
                                                        } else if (term == "serie:idx") {
                                                                termVal = serieIdx;
                                                        } else if (term == "image:idx") {
                                                                termVal = imageIdx;
                                                        } else {
                                                                std::string::size_type dotPos = term.find('.');
                                                                if (dotPos != std::string::npos) {
                                                                        std::string seqName = term.substr(0, dotPos);
                                                                        std::string seqMethod = term.substr(dotPos + 1);
                                                                        for (TSeqMap::iterator it = seqMap.begin(); it != seqMap.end(); ++it) {
                                                                                const std::string& siName = (*it).first;
                                                                                if (seqName == siName) {

                                                                                        long& val = (*it).second;

                                                                                        if (seqMethod == "curval") {
                                                                                                termVal = val;
                                                                                                break;
                                                                                        } else if (seqMethod == "nextval") {
                                                                                                termVal = ++val;
                                                                                                break;
                                                                                        } else {
                                                                                                break;
                                                                                        }
                                                                                }

                                                                        }
                                                                } else {	// Should be a integer
                                                                        std::istringstream is (term);
                                                                        is >> termVal;
                                                                }
                                                        }

                                                        switch (currOp) {
                                                        case '*':
                                                                acc *= termVal;
                                                                break;
                                                        default:
                                                                acc += termVal;
                                                                break;
                                                        }

                                                        lastOpIdx = currOpIdx + 1;

                                                } while (pending);

                                                os << acc;
                                        } else if (subChild->GetName() == wxT("repl")) {
                                                std::string key = std::string(subChild->GetAttribute(wxT("key"), wxEmptyString).ToUTF8());
                                                bool notFound = true;
                                                TMapStack::iterator it = mapStack.begin();
                                                while (it != mapStack.end() && notFound) {
                                                        if ( (*it)->Contains(key) ) {
                                                                os << (*it)->GetValue(key);
                                                                notFound = false;
                                                        }
                                                        ++it;
                                                }
                                        }
                                }
                        }
                        os << "\x0D";
                } else if (child->GetName() == wxT("for.study")) {
                        int idx = 0;
                        for (GIL::IModeloIntegracion::TListOfStudies::iterator it = integrationModel->Studies.begin(); it != integrationModel->Studies.end(); ++it) {
                                const GIL::IMapOfVarsModel& vars = (*it).MapOfVars;
                                mapStack.push_back(&vars);
                                Parse(os, integrationModel, child, &(*it), idx++, NULL, 0, 0, mapStack, seqMap);
                                mapStack.pop_back();
                        }
                } else if (child->GetName() == wxT("for.serie")) {
                        if (study != NULL) {
                                int idx = 0;
                                for (GIL::IStudyModel::TListOfSeries::iterator it = study->Series.begin(); it != study->Series.end(); ++it) {
                                        const GIL::IMapOfVarsModel& vars = (*it).MapOfVars;
                                        mapStack.push_back(&vars);
                                        Parse(os, integrationModel, child, study, studyIdx, &(*it), idx++, 0, mapStack, seqMap);
                                        mapStack.pop_back();
                                }
                        }
                } else if (child->GetName() == wxT("for.image")) {
                        if (serie != NULL) {
                                int idx = 0;
                                for (GIL::ISeriesModel::TListOfImages::iterator it = serie->Images.begin(); it != serie->Images.end(); ++it) {
                                        const GIL::IMapOfVarsModel& vars = (*it).MapOfVars;
                                        mapStack.push_back(&vars);
                                        Parse(os, integrationModel, child, study, studyIdx, serie, serieIdx, idx++, mapStack, seqMap);
                                        mapStack.pop_back();
                                }
                        }
                } else if (child->GetName() == wxT("sequence")) {
                        std::string key(child->GetAttribute(wxT("key"), wxT("")).ToUTF8());
                        if (key.size() > 0) {
                                std::string val(child->GetAttribute(wxT("val"), wxT("0")).ToUTF8());
                                long keyVal = 0;
                                std::istringstream is (val);
                                is >> keyVal;

                                seqMap[key] = keyVal;
                        }
                }

        }

}

std::string GIL::IntegrationController::GetMessageFromIntegrationModel(GNC::GCS::Ptr<GIL::IModeloIntegracion>& integrationModel)
{
        if (integrationModel) {
                if ( integrationModel->messageTemplate.size() == 0) {
                        //
                        GNC::GCS::Permisos::EstadoPermiso perm = GNC::GCS::IControladorPermisos::Instance()->Get("core.integracion", "integration_template");
                        integrationModel->messageTemplate = perm.GetRawValue();
                        if (integrationModel->messageTemplate.empty()) {
                                LOG_ERROR("IntegrationController", "core.integracion.integration_template permission is not active");
                        }
                }

                wxStringInputStream is(wxString::FromUTF8(integrationModel->messageTemplate.c_str()));
                wxXmlDocument doc;
                if (doc.Load(is, wxT("UTF-8"))) {
                        wxXmlNode* root = doc.GetRoot();
                        if (root != NULL && root->GetName() == wxT("msg")) {
                                std::ostringstream os;
                                std::string val;

                                // Add variables
                                const GNC::ControladorExtensiones::ListaModulos& listaModulos = GNC::ControladorExtensiones::Instance()->Modulos();
                                std::string hl7_timestamp(wxDateTime::Now().Format(wxT("%Y%m%d%H%M%S")).ToUTF8());
                                std::string epoch_timestamp;

                                {
                                        time_t tstamp = std::time(NULL);
                                        std::ostringstream os;
                                        os << tstamp;
                                        epoch_timestamp = os.str();
                                }

                                integrationModel->GlobalVariables.InsertVariable(GKDI_MESSAGE_EPOCH_TIMESTAMP, epoch_timestamp, "Message Epoch Timestamp");
                                integrationModel->GlobalVariables.InsertVariable(GKDI_MESSAGE_HL7_TIMESTAMP, hl7_timestamp, "Message HL7 Timestamp");

                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_MODULE_TID, integrationModel->idPlantilla, "Ginkgo CADx module TID");


                                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE", "IDOrganizacionReceptora", val);
                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_CONF_RECEIVING_ORG_ID, val);

                                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","IDSistemaPeticionario", val);
                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_CONF_PETITIONER_SYS_ID, val);

                                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","IDAplicacionRellenadora", val);
                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_CONF_FILLER_APP_ID, val);

                                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","CentroNombre", val);
                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_CONF_ORGANIZATION_NAME, val);

                                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","CentroId", val);
                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_CONF_ORGANIZATION_ID, val);

                                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","IDGinkgoApplication", val);
                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_CONF_GINKGO_APP_ID, val);

                                integrationModel->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_LOCAL_AET, GNC::Entorno::Instance()->GetDicomLocalAET());

                                try {
                                        std::string uidModulo = GetUID(integrationModel->idPlantilla);
                                        integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_MODULE_UID, uidModulo, "Ginkgo CADx module UID");

                                        GNC::ControladorExtensiones::IteradorConstanteListaModulos it = listaModulos.find(uidModulo);
                                        if (it != listaModulos.end()) {
                                                integrationModel->GlobalVariables.InsertVariable(GKDI_GINKGOCADX_MODULE_NAME, (*it).second->GetImporterDescription(), "Ginkgo CADx module name");
                                        }
                                } catch(...) {}

                                TSeqMap seqMap;
                                TMapStack mapStack;
                                mapStack.push_back(& (integrationModel->GlobalVariables) );
                                Parse(os, integrationModel.GetRawPointer(), root, NULL, 0, NULL, 0, 0, mapStack, seqMap);
                                mapStack.clear();
                                seqMap.clear();

                                return os.str();
                        }
                }
        }
        return "";
}

// Event listener realization
void GIL::IntegrationController::ProcesarEvento(GNC::GCS::Events::IEvent *rawEvent)
{

        if (rawEvent->GetCodigoEvento() == ginkgoEVT_Core_HL7SendRequest) {
                GNC::GCS::Events::HL7SendRequestEvent* pEvt = dynamic_cast<GNC::GCS::Events::HL7SendRequestEvent*>(rawEvent);
                if (pEvt != NULL) {
                        GIL::HL7::ControladorHL7 ch(GIL::HL7::ControladorHL7::TP_MLLP);
                        ch.EnviarMensaje(GIL::HL7::Message::FromER7(pEvt->GetMessage()), true);
                }
        }
}


std::string GIL::IntegrationController::GetUID(const std::string& idPlantilla) const
{
        std::string uid;
        wxString plantillawx(idPlantilla.c_str(), wxConvUTF8);

        wxStringTokenizer tkid(plantillawx,wxString::FromUTF8("^"));

        size_t numTokens = tkid.CountTokens();

        if (numTokens != 3) {
                std::ostringstream os;
                os << _Std("Malformed template Id (") << idPlantilla.c_str() << _Std("):\nIs not a valid HL7 identifier (ie: code^description^namespaceId)");
                throw IntegrationException(os.str(),"PID");
        }
        std::string codigo(tkid.GetNextToken().ToUTF8());
        std::string descripcion(tkid.GetNextToken().ToUTF8());
        std::string namespaceid(tkid.GetNextToken().ToUTF8());

        GNC::ControladorExtensiones* pCE = GNC::ControladorExtensiones::Instance();

        for (GNC::ControladorExtensiones::IteradorConstanteListaModulos it = pCE->Modulos().begin(); it != pCE->Modulos().end(); ++it) {
                const GNC::GCS::IModuleController* m = (*it).second;

                LOG_TRACE("HL7", "Parsing module: " << ", " << m->GetUID().c_str() );

                std::string entradaConf = "TUID" + m->GetUID();

                LOG_TRACE("HL7", "EntradaConf = TUID" << m->GetUID().c_str() );

                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE/Plantillas", entradaConf, uid);

                LOG_TRACE("HL7", "tuid = " << uid);

                wxStringTokenizer tkz(wxString::FromUTF8(uid.c_str()), wxT(";"));

                while (tkz.HasMoreTokens()) {
                        wxString token = tkz.GetNextToken();
                        wxStringTokenizer tkidmodulo(token,wxString::FromUTF8("^"));
                        numTokens = tkidmodulo.CountTokens();
                        if (numTokens != 3) {
                                std::ostringstream os;
                                os << _Std("Id malformed template in the module configuration") << "(" << uid.c_str() << _Std("):\nIs not a valid HL7 identifier (ie: code^description^namespaceId)");
                                throw IntegrationException(os.str(),"PID");
                        }

                        std::string codigo_m(tkidmodulo.GetNextToken().ToUTF8());
                        std::string descripcion_m(tkidmodulo.GetNextToken().ToUTF8());
                        std::string namespaceid_m(tkidmodulo.GetNextToken().ToUTF8());
                        if ( (codigo_m == codigo) && (namespaceid_m == namespaceid) ) {
                                std::ostringstream os;
                                LOG_TRACE("HL7", "Found entry:" << m->GetUID().c_str());
                                os << m->GetUID();
                                return os.str();
                        }
                }
        }

        throw IntegrationException(_Std("Template Id unsupported (") + (idPlantilla) + _Std("):\nCheck the configuration and attach it if necessary"),"PID");

}


std::string GIL::IntegrationController::GetUIDModulo(const std::string& idPlantilla) const
{
        std::string uidModulo = GetUID(idPlantilla);
        std::string sidModulo("");

        const GNC::ControladorExtensiones::ListaModulos& listaModulos = GNC::ControladorExtensiones::Instance()->Modulos();
        for(GNC::ControladorExtensiones::IteradorConstanteListaModulos itModulos = listaModulos.begin(); itModulos != listaModulos.end(); ++itModulos) {
                std::ostringstream os;
                os << itModulos->first;
                if(os.str() == uidModulo) {
                        return itModulos->second->GetUID();
                }
        }
        return sidModulo;
}

std::string GIL::IntegrationController::GetIdPlantilla(const std::string& uidModulo) const
{
        GNC::GCS::IModuleController* pModuleController = GNC::ControladorExtensiones::Instance()->ObtenerModulo(uidModulo);
        if (pModuleController == NULL) {
                std::ostringstream os;
                os << _Std("Unknown module Id (") << uidModulo.c_str();
                throw IntegrationException(os.str(),"PID");
        }

        std::string entradaConf = "TUID" + pModuleController->GetUID();
        std::string templateId;
        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE/Plantillas", entradaConf, templateId);
        return templateId;
}



/* Parsea la cadena XML y construye el modelo de integracion. Los parametros no encontrados se asignan por defecto segun lo especificado en la configuracion global */
void GIL::IntegrationController::ParsearModeloIntegracion(GIL::IntegrationModelList& modelos, const std::string& xmlString, const wxString& configurationFilePath) const
{
        GTRACE("GIL::HL7::ControladorHL7::ParsearModeloIntegracion( modelo, [\n" << xmlString.c_str() << "\n] )");
        wxXmlDocument xml;
        std::string xpp; // XML Pretty Printed for extended information info.

        wxStringInputStream flujoEntrada(wxString::FromUTF8(xmlString.c_str()));

        if (!xml.Load(flujoEntrada,wxT("UTF 8"))) {
                throw IntegrationException(_Std("Empty configuration"), "CONF");
        }

        wxXmlNode* raiz = xml.GetRoot();

        if (raiz == NULL) {
                throw IntegrationException(_Std("Empty configuration"), "CONF");
        } else {
                wxString xmlPrettyPrint;
                wxStringOutputStream wsos;
                if (xml.Save(wsos, 1)) {
                        xpp = wsos.GetString().ToUTF8();
                        LOG_TRACE("Integration", "Parsing XML Integration: " << std::endl << xpp.c_str());
                }
        }

        std::string parserKey (raiz->GetName().ToUTF8());

        if (m_mapParsers.find(parserKey) != m_mapParsers.end()) {
                m_mapParsers.find(parserKey)->second->ParseIntegrationXML(modelos, raiz);
                wxFileName absoluteFile(configurationFilePath.c_str());
                if (absoluteFile.FileExists()) {
                        absoluteFile = absoluteFile.GetPath();
                }
                for (GIL::IntegrationModelList::iterator it = modelos.begin(); it != modelos.end(); ++it) {
                        GNC::GCS::Ptr<GIL::IModeloIntegracion>& modelo = (*it);
                        //if action is open... absolutize files
                        if (modelo->accion == GIL::IModeloIntegracion::TA_Open) {
                                GIL::IModeloIntegracion::ListOfPaths absoluteFiles;
                                for (GIL::IModeloIntegracion::ListOfPaths::iterator itFile = modelo->Files.begin(); itFile != modelo->Files.end(); ++itFile) {
                                        wxFileName fileName(wxString::FromUTF8((*itFile).c_str()));
                                        if (fileName.IsRelative() && absoluteFile.IsOk()) {
                                                fileName.MakeAbsolute(absoluteFile.GetFullPath());
                                        }
                                        std::string pathStd(fileName.GetFullPath().ToUTF8());
                                        absoluteFiles.push_back(pathStd);
                                }
                                modelo->Files = absoluteFiles;
                        }
                }

        } else {
                std::ostringstream ostr;
                ostr << _Std("XML Parser Not Found. Key=") << parserKey << " " << xpp;
                throw IntegrationException(ostr.str(), "CONF");
        }
}

std::string GIL::IntegrationController::ParsesModelFromURL(const wxString& url) const
{
        LOG_STAT(ANON_STAT_INT_URL)
        //parse ginkgo cloud uris
        if (url.StartsWith(wxT("ginkgocadx://cloud:"))) {
                GNC::GCS::CloudGetCommandParams* pParams = new GNC::GCS::CloudGetCommandParams(std::string(url.SubString(19, url.size() - 1).ToUTF8()), true);
                GNC::GCS::CloudCommand* pCmd = new GNC::GCS::CloudCommand(pParams);
                GNC::CommandController::Instance()->ProcessAsync("Downloading",pCmd, NULL);

                return "";
        }

        //  ginkgocadx://?pacs=dicom%3A%2F%2FDCM4CHEE%40ironman%3A11112%3Fmethod%3Dget%26mode%3Dseries&action=retrieve&level=STUDY&query=0020%7C000D%3A1.3.6.1.4.1.39470.1.1.3.0.1.2.2259150805.5724.1353416100.533&aid=JIMENA
        // ginkgocadx://?pacs=dicom%3A%2F%2FDCM4CHEE%40ironman%3A11112%3Fmethod%3Dget%26mode%3Dseries&action=dicomize&petition-id=asdf&scope=AA&patient-id=234234&episode-number=asewr&query
        // ginkgocadx://?pacs=dicom%3A%2F%2FDCM4CHEE%40ironman%3A11112%3Fmethod%3Dget%26mode%3Dseries&action=dicomize&patient-id=234234&query=0010%7C0040%3AM&query=0010%7C0010%3ADoe%5Ejoe%20Charl


        // ginkgocadx://?action=retrieve&pacs=test&level=STUDY&query=0020|000D:1.2.3.4.5.6 de un pacs id
        // ginkgocadx://?action=retrieve&level=STUDY&query=0020|000D:1.2.3.4.5.6  | del pacs por defecto
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <gnkworkflows>
        <!-- Configuracion de PACS, estos son opcionales y se pueden declarar tantos como uno quiera. -->
        <pacs sid="test" aet="DCM4CHEE" hostname="172.26.0.3" port="11112" tls="no" verify-credentials="no" retrieve-method="get" pdu="16384" retrieve-mode="series">
        <!--sid: Identificador del pacs(obligatorio) -->
        <!--aet: Application entity title (obligatorio) -->
        <!--hostname: hostname o ip (obligatorio) -->
        <!--port: puerto del PACS (obligatorio) -->
        <!--tls (yes/no): usar tls. (por defecto no) -->
        <!--verify-credentials (yes/no): Verificar credenciales del servidor PACS. (por defecto no) -->
        <!--retrieve-method (get/move): Metodo de descargar del pacs (por defecto get) -->

        <!--retrieve-mode (series/studies): Modo de obtencion (por defecto series) -->
        	<!--pdu: Tamanio de la PDU. (por defecto=16384) -->
        <!--pacs-user: Usuario de acceso al pacs. -->
        <!--pacs-password: Password de acceso al PACS  -->
        	<gnkworkflow action="retrieve" pacs-retrieve-sid="test">
        <!-- action: flujo de trabajo a ejecutar {dicomize, retrieve} -->
        <!-- pacs-retrieve-sid: Identificador del PACS del que se va a descargar la prueba. Puede ser uno almacenado en ginkgo cadx o alguno declarado anteriormente. Si no se especifica ninguno se utiliza el PACS por defecto. -->

        			<!-- Nivel de consulta en el PACS (obligatorio) -->
                <query-retrieve-level value="STUDY"/>
        			<!-- query-retrieve-level modos: {PATIENT, STUDY, SERIES, IMAGE} -->

                <!-- Metadatos para realizar la consulta en el PACS (obligatorio) -->
                <metadata code="DICOM" key="0020|000D" value="1.2.3.4.5.6" />   <!-- StudyInstanceUID -->
        			<!--code: tipo de codigo, solo esta soportado DICOM (obligatorio) -->
        			<!--key: clave a buscar (obligatorio) -->
        			<!--value: valor (obligatorio) -->
        			<!-- Ejemplo SeriesInstanceUID: <metadata code="DICOM" key="0020|000E" value="2.18.840.1.113883.5.242.33.14.3.10110519171259.709.76244" /> -->
        			<!-- Ejemplo AccessionNumber: <metadata code="DICOM" key="0008|0050" value="2011-27843782" /> -->

        			<!-- Aplicacion que genera la llamada. Se usa para mensajeria y LOG (en este caso no hay mensajeria con esta operacion) (obligatorio) -->
                <hce aid="JIMENA" />
        </gnkworkflow>
        	*/
        // ginkgocadx://?action=retrieve&level=STUDY&query=0020|000D:1.2.3.4.5.6&aid=JIMENA

        wxURI uri(url);
        wxString query = uri.GetQuery();
        std::string xmlIntegration;
        if (!query.IsEmpty()) {
                std::map<std::string, std::string> parameters;
                std::list<std::string> listOfQueries; //query is an special multivalued parameter...
                wxStringTokenizer tknzr(query, wxT("&"));
                while (tknzr.HasMoreTokens()) {
                        wxString token = tknzr.GetNextToken();
                        if (token.StartsWith(wxT("query="))) {
                                //special multivalued parameter...
                                int pos = token.Find(wxT("="));
                                listOfQueries.push_back(std::string(wxURI::Unescape(token.SubString(pos+1, token.size() - 1)).ToUTF8()));
                        } else {
                                if (token.Contains(wxT("="))) {
                                        int pos = token.Find(wxT("="));
                                        parameters[std::string(token.SubString(0, pos-1).Lower().ToUTF8())] = std::string(wxURI::Unescape(token.SubString(pos+1, token.size() - 1)).ToUTF8());
                                } else {
                                        parameters[std::string(token.Lower().ToUTF8())] = "";
                                }
                        }
                }
                wxXmlDocument doc;
                doc.SetFileEncoding(wxT("UTF-8"));
                doc.SetRoot(new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("gnkworkflows")));
                //pacs parsing...
                //pacs configuration from url is simple, you can configure:
                //pacs=pacsid
                //pacs=dicom://AET@host:port&method=<get/move>&mode=<STUDY/SERIES>
                //pacs=dicom://DCM4CHEE@172.26.0.33:11112?method=WADO&wadourl=http://172.26.0.33:18080/wado => pacs=dicom://DCM4CHEE@172.26.0.33:11112?method=WADO&urlWado=http%3A%2F%2F172.26.0.33%3A18080%2Fwado
                wxString pacsSid;
                if (parameters.find("pacs") != parameters.end()) {
                        wxString pacsParameter = wxString::FromUTF8(parameters["pacs"].c_str());
                        if (pacsParameter.StartsWith(wxT("dicom://"))) {
                                wxXmlNode* pacsNode = new wxXmlNode(doc.GetRoot(), wxXML_ELEMENT_NODE, wxT("pacs"));
                                //randomSid
                                pacsSid =  wxString::Format(wxT("temppacs%d"), (int) rand());
                                pacsNode->AddAttribute(wxT("sid"),pacsSid);
                                //parse url...
                                wxURI pacsUri(pacsParameter);

                                if (pacsUri.GetUserInfo().IsEmpty() || pacsUri.GetServer().IsEmpty()) {
                                        throw IntegrationException("Error in pacs url format: dicom://AET@host:port", "URLPARSER");
                                }
                                pacsNode->AddAttribute(wxT("aet"), pacsUri.GetUserInfo());
                                pacsNode->AddAttribute(wxT("hostname"), pacsUri.GetServer());
                                if (!pacsUri.GetPort().IsEmpty()) {
                                        pacsNode->AddAttribute(wxT("port"), pacsUri.GetPort());
                                }

                                //tokenize query &
                                wxStringTokenizer dicomTknzr(pacsUri.GetQuery(), wxT("&"));
                                while (dicomTknzr.HasMoreTokens()) {
                                        wxString token = dicomTknzr.GetNextToken();
                                        if (token.StartsWith(wxT("method="))) {
                                                int pos = token.Find(wxT("="));
                                                pacsNode->AddAttribute(wxT("retrieve-method"),wxURI::Unescape(token.SubString(pos+1, token.size() - 1)));
                                        }
                                        if (token.StartsWith(wxT("mode="))) {
                                                int pos = token.Find(wxT("="));
                                                pacsNode->AddAttribute(wxT("retrieve-mode"),wxURI::Unescape(token.SubString(pos+1, token.size() - 1)));
                                        }
                                        if (token.StartsWith(wxT("wadourl="))) {
                                                int pos = token.Find(wxT("="));
                                                pacsNode->AddAttribute(wxT("wado-url"),wxURI::Unescape(token.SubString(pos+1, token.size() - 1)));
                                        }
                                }
                        } else {
                                pacsSid = pacsParameter;
                        }
                }
                //action parsing...
                if (parameters.find("action") != parameters.end()) {
                        wxXmlNode* workFlowNode = new wxXmlNode(doc.GetRoot(), wxXML_ELEMENT_NODE, wxT("gnkworkflow"));
                        if (parameters["action"]=="retrieve") {
                                //retrieve
                                //action=retrieve&aid=JIMENA&query=group|element:value
                                workFlowNode->AddAttribute(wxT("action"), wxT("retrieve"));
                                if (!pacsSid.IsEmpty()) {
                                        workFlowNode->AddAttribute(wxT("pacs-retrieve-sid"), pacsSid);
                                }
                                //retrieve..
                                //parse level and query...
                                if (parameters.find("level") != parameters.end()) {
                                        wxXmlNode* qrLevel = new wxXmlNode(workFlowNode, wxXML_ELEMENT_NODE, wxT("query-retrieve-level"));
                                        qrLevel->AddAttribute(wxT("value"), wxString::FromUTF8(parameters["level"].c_str()));
                                }
                                //aid
                                if (parameters.find("aid") != parameters.end()) {
                                        wxXmlNode* qrLevel = new wxXmlNode(workFlowNode, wxXML_ELEMENT_NODE, wxT("hce"));
                                        qrLevel->AddAttribute(wxT("aid"), wxString::FromUTF8(parameters["aid"].c_str()));
                                }
                                //parse query...
                                //query=0020|000D:1.2.3.4.5.6
                                //<metadata code="DICOM" key="0020|000D" value="1.2.3.4.5.6" />
                                for (std::list<std::string>::iterator it = listOfQueries.begin(); it != listOfQueries.end(); ++it) {
                                        wxString queryString = wxString::FromUTF8((*it).c_str());
                                        int colonPosition = queryString.Find(wxT(":"));
                                        if (colonPosition == -1) {
                                                throw IntegrationException("Malformated query string: group|element:value", "URLPARSER");
                                        }
                                        wxXmlNode* queryNode = new wxXmlNode(workFlowNode, wxXML_ELEMENT_NODE, wxT("metadata"));
                                        queryNode->AddAttribute(wxT("code"), wxT("DICOM"));
                                        queryNode->AddAttribute(wxT("key"), queryString.SubString(0, colonPosition-1));
                                        queryNode->AddAttribute(wxT("value"), queryString.SubString(colonPosition+1, queryString.size() - 1));
                                }
                        } else if(parameters["action"] == "dicomize") {
                                //dicomize...
                                //action=dicomize&petition-id=asdf&scope=AA&patient-id=234234&episode-number=asewr&query...
                                //action
                                workFlowNode->AddAttribute(wxT("action"), wxT("dicomize"));
                                //pacs
                                if (!pacsSid.IsEmpty()) {
                                        workFlowNode->AddAttribute(wxT("pacs-store-sid"), pacsSid);
                                }
                                //petition-id
                                if (parameters.find("petition-id") != parameters.end()) {
                                        workFlowNode->AddAttribute(wxT("petition-id"), wxString::FromUTF8(parameters["petition-id"].c_str()));
                                }
                                //scope
                                if (parameters.find("scope") != parameters.end()) {
                                        workFlowNode->AddAttribute(wxT("scope"), wxString::FromUTF8(parameters["scope"].c_str()));
                                }
                                //patient...
                                if (parameters.find("patient-id") != parameters.end()) {
                                        wxXmlNode* patientNode = new wxXmlNode(workFlowNode, wxXML_ELEMENT_NODE, wxT("patient"));
                                        wxXmlNode* idNode = new wxXmlNode(patientNode, wxXML_ELEMENT_NODE, wxT("id"));
                                        idNode->AddAttribute(wxT("code"),wxT("PI"));
                                        idNode->AddAttribute(wxT("value"), wxString::FromUTF8(parameters["patient-id"].c_str()));
                                        //episode-number
                                        if (parameters.find("episode-number") != parameters.end()) {
                                                wxXmlNode* episodeNumberNode = new wxXmlNode(patientNode, wxXML_ELEMENT_NODE, wxT("episode-number"));
                                                new wxXmlNode(episodeNumberNode, wxXML_TEXT_NODE,wxT("content"),wxString::FromUTF8(parameters["episode-number"].c_str()));
                                        }
                                } else {
                                        throw IntegrationException("patient-id parameter is required in dicomization process", "URLPARSER");
                                }
                                //parse query...
                                //query=0020|000D:1.2.3.4.5.6
                                //<metadata code="DICOM" key="0020|000D" value="1.2.3.4.5.6" />
                                for (std::list<std::string>::iterator it = listOfQueries.begin(); it != listOfQueries.end(); ++it) {
                                        wxString queryString = wxString::FromUTF8((*it).c_str());
                                        int colonPosition = queryString.Find(wxT(":"));
                                        if (colonPosition == -1) {
                                                throw IntegrationException("Malformated query string: group|element:value", "URLPARSER");
                                        }
                                        wxXmlNode* queryNode = new wxXmlNode(workFlowNode, wxXML_ELEMENT_NODE, wxT("metadata"));
                                        queryNode->AddAttribute(wxT("code"), wxT("DICOM"));
                                        queryNode->AddAttribute(wxT("key"), queryString.SubString(0, colonPosition-1));
                                        queryNode->AddAttribute(wxT("value"), queryString.SubString(colonPosition+1, queryString.size() - 1));
                                }
                        } else {
                                throw IntegrationException("Unsupported action parameter", "URLPARSER");
                        }
                }

                wxStringOutputStream out;
                doc.Save(out);
                xmlIntegration = std::string(out.GetString().ToUTF8());
                LOG_DEBUG("ItegrationController", "Generated XML\n"<<xmlIntegration);
        }

        return xmlIntegration;
}

