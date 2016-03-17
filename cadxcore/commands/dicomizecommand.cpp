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


#include <sstream>
#include <map>
#include <cmath>
#include <locale>

#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/xml/xml.h>
#include <wx/mstream.h>
#include <wx/image.h>

#include <api/globals.h>
#include <api/integration/dict.h>
#include <api/controllers/ieventscontroller.h>
#include <main/controllers/commandcontroller.h>
#include <api/controllers/ipacscontroller.h>
#include <api/internationalization/internationalization.h>

#include <api/dicom/idicomizador.h>
#include <api/dicom/idicommanager.h>

#include <main/controllers/controladorlog.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/controladorhl7.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/inactivitycontroller.h>
#include <main/controllers/integrationcontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <eventos/progresocomando.h>

#include <commands/sendpacscommand.h>
#include <commands/comandoincluirhistorial.h>

#include <export/tagsprivadoscomunes.h>
#include <main/entorno.h>

#include "dicomizecommand.h"
#include "dicomizationcompletebase.h"

#define IDC_DICOMIZACION_INTEGRACION           2030
#define IDC_SUBCOMANDO_ADD_HISTORIAL           2031

// Singleton de persistencia
namespace GADAPI
{

DicomizeCommandParams::DicomizeCommandParams(const GNC::GCS::Ptr<GNC::GUI::ImportationData>& pDatosPersistentes, const std::string& dirTemporal)
{
        m_pSemaforoEsperarComando = new wxSemaphore();
        if (pDatosPersistentes.IsValid()) {
                m_pImportData = pDatosPersistentes;
        } else {
                m_pImportData = new GNC::GUI::ImportationData();
        }

        //se copia todo el directorio temporal a uno nuevo
        wxString wxNuevoDirTemp = FROMPATH(GNC::Entorno::Instance()->CrearDirectorioTemporal());

        wxDir dir;
        if (!dirTemporal.empty()) {
                if (dir.Open(FROMPATH(dirTemporal))) {
                        wxString fileName;
                        wxString destino;
                        bool cont = dir.GetFirst(&fileName);

                        while (cont) {
                                destino = wxNuevoDirTemp + wxFileName::GetPathSeparator(wxPATH_NATIVE) + fileName;
                                fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +fileName;
                                wxRenameFile(fileName,destino);
                                for(GNC::GUI::ImportationData::TListOfFiles::iterator it = m_pImportData->m_SourceFiles.begin(); it!= m_pImportData->m_SourceFiles.end(); ++it) {
                                        if ( FROMPATH((*it)) == fileName ) {
                                                (*it) = TOPATH(destino);
                                                break;
                                        }
                                }
                                cont = dir.GetNext(&fileName);
                        }
                }
        }
        m_dirTemporal = TOPATH(wxNuevoDirTemp);
        ///
        m_mensajeError = "";
        m_copiadoCorrecto = false;
        m_importacionCorrecta = false;
        m_openAfterDicomize = true;
        m_pComandoEspera = NULL;
        m_pParamsEspera = NULL;
        //
        GTRACE("<< DicomizeCommandParams::DicomizeCommandParams(): " << this);
}

DicomizeCommandParams::DicomizeCommandParams( const DicomizeCommandParams& o)
{
        (*this) = o;
}

DicomizeCommandParams& DicomizeCommandParams::operator = (const DicomizeCommandParams& o)
{
        this->m_pImportData = o.m_pImportData;
        this->m_dirTemporal = o.m_dirTemporal;
        this->m_openAfterDicomize = o.m_openAfterDicomize;

        this->m_mensajeError = "";
        this->m_copiadoCorrecto = o.m_copiadoCorrecto;
        this->m_importacionCorrecta = o.m_importacionCorrecta;
        this->m_listOfFiles = o.m_listOfFiles;

        m_pSemaforoEsperarComando = new wxSemaphore();
        m_pComandoEspera = NULL;
        m_pParamsEspera = NULL;
        return *this;
}

DicomizeCommandParams::~DicomizeCommandParams()
{
        if (m_pSemaforoEsperarComando != NULL) {
                delete m_pSemaforoEsperarComando;
                m_pSemaforoEsperarComando = NULL;
        }
}

DicomizeCommand::DicomizeCommand(DicomizeCommandParams* pParams) : IComando(pParams)
{
        m_pIntegracionParams = pParams;
        SetId(IDC_DICOMIZACION_INTEGRACION);
        EsperaA(IDC_DICOMIZACION_INTEGRACION);

        GNC::Entorno::Instance()->GetControladorEventos()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
}

void DicomizeCommand::Execute()
{
        NotificarProgreso(0.05f, _Std("Step 1 of 3: Importing images"));

        if(!m_pIntegracionParams->m_importacionCorrecta) {
                m_pIntegracionParams->m_importacionCorrecta = ImportarFicherosOriginales();
        }
        if(!m_pIntegracionParams->m_importacionCorrecta) {
                return;
        }

        NotificarProgreso(0.40f, _Std("Step 2 of 3: Copying images"));
        //despues se copian todos los dicoms...

        if(!m_pIntegracionParams->m_copiadoCorrecto) {
                m_pIntegracionParams->m_copiadoCorrecto = CopiarDicom();
        }
        if(!m_pIntegracionParams->m_copiadoCorrecto) {
                return;
        }

        //ahora se suben al pacs...
        NotificarProgreso(0.50f, _Std("Step 3 of 3: Uploading images"));

        if(m_pIntegracionParams->m_pImportData->m_pIntegrationModel->GlobalVariables.Contains(GKDI_GLOBAL_PACS_STORE_SID)) {
                SubirPACS();
        }
}

void DicomizeCommand::Update()
{
        if(m_pIntegracionParams->m_mensajeError != "") {
                GNC::GCS::InactivityController::Instance()->StopsMonitoring();
                wxString mensaje = _("Import process failed:") + wxString(wxT("\n - ")) + wxString::FromUTF8(m_pIntegracionParams->m_mensajeError.c_str()) + _("\nWould you like to retry?");

                int answer = wxMessageBox(mensaje,_("Info"),wxYES_NO | wxICON_INFORMATION, GNC::Entorno::Instance()->GetVentanaRaiz());
                GNC::GCS::InactivityController::Instance()->RestartMonitoring();
                if(answer == wxYES) {
                        DicomizeCommandParams* pParams = new DicomizeCommandParams(*m_pIntegracionParams);
                        DicomizeCommand* pCmd = new DicomizeCommand(pParams);
                        GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Importing..."),pCmd,NULL);
                } else {
                        BorrarArchivosTemporales(FROMPATH(m_pIntegracionParams->m_dirTemporal));
                        GNC::GCS::InactivityController::Instance()->KillNow();
                }
        } else {
                BorrarArchivosTemporales(FROMPATH(m_pIntegracionParams->m_dirTemporal));
                bool showMessage;
                GNC::GCS::ConfigurationController::Instance()->readBoolUser("/GinkgoCore/Importation", "ShowFinishMessage", showMessage, true);
                if (showMessage) {
                        GNC::GUI::DicomizationCompleteBase dlg(GNC::Entorno::Instance()->GetVentanaRaiz());
                        dlg.ShowModal();
                        if (dlg.m_pCheckNotShow->IsChecked()) {
                                GNC::GCS::ConfigurationController::Instance()->writeBoolUser("/GinkgoCore/Importation", "ShowFinishMessage", false);
                        }
                }
                GNC::GCS::InactivityController::Instance()->KillNow();
        }
}

void DicomizeCommand::OnAbort()
{
        BorrarArchivosTemporales(FROMPATH(m_pIntegracionParams->m_dirTemporal));
}

void DicomizeCommand::LiberarRecursos()
{
        BorrarArchivosTemporales(FROMPATH(m_pIntegracionParams->m_dirTemporal));
}

bool DicomizeCommand::ImportarFicherosOriginales()
{
        GIL::IStudyModel studyModel;
        GNC::GUI::ImportationData& importationData = *m_pIntegracionParams->m_pImportData;
        importationData.m_pIntegrationModel->Studies.push_back(studyModel);
        GIL::ISeriesModel imageSeries = ImportOriginalFiles(false);
        if (!imageSeries.Images.empty()) {
                importationData.m_pIntegrationModel->Studies.front().Series.push_back(imageSeries);
        }
        GIL::ISeriesModel pdfSeries = ImportOriginalFiles(true);
        if (!pdfSeries.Images.empty()) {
                importationData.m_pIntegrationModel->Studies.front().Series.push_back(pdfSeries);
        }
        return (imageSeries.Images.size() + pdfSeries.Images.size()) == importationData.m_SourceFiles.size();
}

GIL::ISeriesModel DicomizeCommand::ImportOriginalFiles(bool pdfFiles)
{
        bool esElPrimero=true;
        GIL::ISeriesModel seriesModel;

        GIL::DICOM::IPACSController* pCI = GNC::Entorno::Instance()->GetPACSController();
        GNC::GUI::ImportationData& importationData = *m_pIntegracionParams->m_pImportData;

        if (importationData.m_pIntegrationModel->Studies.empty()) {
                LOG_ERROR("ImportOriginalFiles", "study model must exists");
                return seriesModel;
        }
        GIL::IStudyModel& studyModel = importationData.m_pIntegrationModel->Studies.front();

        int instanceNumber = 0;
        std::string seriesInstanceUID = "";
        for(GNC::GUI::ImportationData::TListOfFiles::iterator it= importationData.m_SourceFiles.begin(); it!= importationData.m_SourceFiles.end(); ++it) {
                if (pCI->EsPDF((*it)) != pdfFiles)
                        continue;
                GIL::DICOM::IDICOMImg2DCM* pI2D = NULL;
                GIL::DICOM::IDICOMManager*	pDICOMManager = NULL;
                try {
                        pI2D = pCI->CrearInstanciaDeImportacion();
                        std::string rutaFichero = (*it);

                        pI2D->Inicializar(rutaFichero, NULL);

                        std::string tag;

                        //base comun
                        GIL::DICOM::DicomDataset base = importationData.baseImages;

                        if (seriesInstanceUID != "") {
                                base.tags[std::string("0020|000e")] = seriesInstanceUID; //serie instance uid
                        }

                        //fecha imagen
                        base.tags[std::string("0008|0022")] = wxDateTime::Now().Format(wxT("%Y%m%d")).ToUTF8();
                        //hora imagen
                        base.tags[std::string("0008|0032")] = wxDateTime::Now().Format(wxT("%H%M%S")).ToUTF8();


                        //instance number
                        {
                                std::ostringstream ostr;
                                ostr << instanceNumber++;
                                base.tags[std::string("0020|0013")] = ostr.str();
                        }

                        if (!importationData.m_pIntegrationModel->rawXmlData.empty()) {
                                //raw data
                                GIL::DICOM::TipoPrivateTags tagsPrivados(UID_TAGS_PRIVADOS_COMUNES);
                                unsigned char tag = TAG_XML_INTEGRACION;
                                tagsPrivados.SetTag(tag,importationData.m_pIntegrationModel->rawXmlData);
                                pI2D->InsertarTagsPrivados(tagsPrivados);
                        }

                        pI2D->InsertarJerarquia(base);

                        pI2D->InsertarJerarquia(importationData.m_pIntegrationModel->TagsDICOMOverwrite);

                        wxFileName inputFileName = wxFileName::FileName(FROMPATH(rutaFichero));
                        //se crea el directorio de la serie
                        wxString tempFilePath = FROMPATH(m_pIntegracionParams->m_dirTemporal) + wxFileName::GetPathSeparator(wxPATH_NATIVE) + inputFileName.GetName() + wxT(".dcm");
                        while (wxFile::Exists(tempFilePath)) {
                                tempFilePath = FROMPATH(m_pIntegracionParams->m_dirTemporal) + wxFileName::GetPathSeparator(wxPATH_NATIVE) + inputFileName.GetName() + wxString::Format(wxT("%d.dcm"), (int)(rand()) );
                        }

                        std::string rutaImportacion(TOPATH(tempFilePath));

                        pI2D->Convertir(rutaImportacion);

                        pCI->LiberarInstanciaDeImportacion(pI2D);
                        pI2D = NULL;

                        {
                                //leemos tags
                                pDICOMManager = pCI->CrearInstanciaDeDICOMManager();
                                pDICOMManager->CargarFichero(rutaImportacion);
                                if(esElPrimero) {
                                        esElPrimero=false;

                                        if (!studyModel.MapOfVars.Contains(GKDI_STUDY_ACCESSION_NUMBER)) {
                                                if(pDICOMManager->GetTag(0x0008,0x0050,tag)) {//accession number
                                                        importationData.baseImages.tags[std::string("0008|0050")] = tag;
                                                        studyModel.MapOfVars.InsertVariable(GKDI_STUDY_ACCESSION_NUMBER, tag);
                                                }

                                                if(pDICOMManager->GetTag(0x0020,0x000d,tag)) {//studyUID
                                                        importationData.baseImages.tags[std::string("0020|000d")] = tag;
                                                        studyModel.MapOfVars.InsertVariable(GKDI_STUDY_INSTANCE_UID, tag);
                                                }
                                        }

                                        if(pDICOMManager->GetTag(0x0020,0x000e,tag)) {//uid serie original
                                                seriesInstanceUID = tag;
                                                seriesModel.MapOfVars.InsertVariable(GKDI_SERIE_INSTANCE_UID, tag);
                                        }
                                }
                                GIL::IImageModel imageModel;

                                pDICOMManager->GetTag(0x0008,0x0018,tag); //sop instance uid
                                imageModel.MapOfVars.InsertVariable(GKDI_IMAGE_SOP_INSTANCE_UID, tag);

                                pDICOMManager->GetTag(0x0008,0x0016,tag);//sop class uid
                                imageModel.MapOfVars.InsertVariable(GKDI_IMAGE_SOP_CLASS_UID, tag);

                                pCI->LiberarInstanciaDeDICOMManager(pDICOMManager);
                                pDICOMManager = NULL;

                                imageModel.MapOfVars.InsertVariable(GKDI_IMAGE_DICOM_PATH, rutaImportacion);
                                seriesModel.Images.push_back(imageModel);
                        }//fin leer tags

                } catch (GIL::DICOM::I2DException& ex) {
                        m_pIntegracionParams->m_mensajeError = ex.GetCause();
                        LOG_ERROR("DICOMIZATION", "Error in dicomization process: " << ex.GetCause());
                        if (pI2D != NULL) {
                                pCI->LiberarInstanciaDeImportacion(pI2D);
                        }
                        if (pDICOMManager != NULL) {
                                pCI->LiberarInstanciaDeDICOMManager(pDICOMManager);
                        }
                } catch (std::exception& e) {
                        m_pIntegracionParams->m_mensajeError = e.what();
                        LOG_ERROR("DICOMIZATION", "Error doing dicomization: " << e.what() );
                        if (pI2D != NULL) {
                                pCI->LiberarInstanciaDeImportacion(pI2D);
                        }
                        if (pDICOMManager != NULL) {
                                pCI->LiberarInstanciaDeDICOMManager(pDICOMManager);
                        }
                } catch (...) {
                        m_pIntegracionParams->m_mensajeError = _Std("Unknown error in dicomization process");
                        LOG_ERROR("Internal error in dicomization process", "DICOMIZATION");
                        if (pI2D != NULL) {
                                pCI->LiberarInstanciaDeImportacion(pI2D);
                        }
                        if (pDICOMManager != NULL) {
                                pCI->LiberarInstanciaDeDICOMManager(pDICOMManager);
                        }
                }
        }// end for
        return seriesModel;
}

bool DicomizeCommand::CopiarDicom()
{
        bool correcto = true;

        m_pIntegracionParams->m_listOfFiles.clear();
        //el directorio existe, se crea un directorio para la serie, con el uid de la serie donde se copiaran los ficheros importados
        wxDir dir;
        if (dir.Open(FROMPATH(m_pIntegracionParams->m_dirTemporal))) {
                wxString pathOrigenWx,destinoWx;
                bool cont = dir.GetFirst(&pathOrigenWx,wxT("*.dcm"));
                std::string pathDestinoStd,pathOrigenStd;
                GIL::DICOM::DicomDataset base;

                while (cont) {
                        pathOrigenWx=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +pathOrigenWx;
                        pathOrigenStd = TOPATH(pathOrigenWx);
                        m_pIntegracionParams->m_listOfFiles.push_back(pathOrigenStd);
                        cont = dir.GetNext(&pathOrigenWx);
                }
        }

        if(correcto) {
                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(m_pIntegracionParams->m_listOfFiles, m_pIntegracionParams->m_openAfterDicomize, GNC::GCS::HistoryController::TAA_MOVE);
                pParams->m_informar = false;
                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);

                LanzarYEsperar(pCmd, pParams);
        }
        return correcto;
}

bool DicomizeCommand::SubirPACS()
{
        try {
                //get hl7 message...
                std::string hl7Message;
                if (m_pIntegracionParams->m_pImportData->m_pIntegrationModel.IsValid() && GNC::GCS::IControladorPermisos::Instance()->Get("core.integracion","send_message")) {
                        // Lanzar evento.
                        hl7Message = GIL::IntegrationController::Instance()->GetMessageFromIntegrationModel(m_pIntegracionParams->m_pImportData->m_pIntegrationModel);
                }

                //el pacs sera el primero d la configuracion si no lanzar excepcion
                if (GNC::GCS::IControladorPermisos::Instance()->Get("core.integracion", "upload_pacs_xml")) {
                        std::string pacsStore = m_pIntegracionParams->m_pImportData->m_pIntegrationModel->GlobalVariables.GetValue(GKDI_GLOBAL_PACS_STORE_SID);
                        ::GADAPI::SendPACSCommand* pCmd = new ::GADAPI::SendPACSCommand(pacsStore, hl7Message, m_pIntegracionParams->m_InsertedSeries);
                        GNC::Entorno::Instance()->GetCommandController()->ProcessAsync("Processing...",pCmd,NULL);
                } else {
                        LOG_INFO("DicomizeCommand","pacs upload is disabled");
                }
        } catch (GIL::HL7::HL7Exception& ex) {
                std::ostringstream ostr;
                m_pIntegracionParams->m_mensajeError = _Std("Error sending evidences:") + ex.GetCause();
                LOG_ERROR("SendHL7Message",m_pIntegracionParams->m_mensajeError);
                return false;
        } catch (GIL::DICOM::PACSException& ex) {
                std::stringstream ostr;
                ostr << _Std("Can not upload to the PACS:\n");
                ostr << ex.str();
                m_pIntegracionParams->m_mensajeError = ostr.str();
        } catch (GNC::GCS::IException& ex) {
                m_pIntegracionParams->m_mensajeError = _Std("Error sending evidences:") + ex.GetCause();
                LOG_ERROR("SendHL7Message",m_pIntegracionParams->m_mensajeError);
                return false;
        } catch (std::exception& ex) {
                std::stringstream ostr;
                ostr << _Std("Can not upload to the PACS:\n");
                ostr << ex.what();
                m_pIntegracionParams->m_mensajeError = ostr.str();
        } catch (...) {
                std::stringstream ostr;
                ostr << _Std("Can not upload to the PACS:\nInternal Error");
                m_pIntegracionParams->m_mensajeError = ostr.str();
        }
        if(m_pIntegracionParams->m_mensajeError != "") {
                return false;
        } else {
                return true;
        }
}

void DicomizeCommand::LanzarYEsperar(GNC::GCS::IComando* pCmd, GNC::GCS::IComandoParams* pCmdParams)
{
        LOG_TRACE("DicomizeCommand", "Esperando a " << pCmd);
        m_pIntegracionParams->m_pComandoEspera = pCmd;
        m_pIntegracionParams->m_pParamsEspera = pCmdParams;
        GNC::Entorno::Instance()->GetCommandController()->ProcessAsync("Processing...",pCmd,NULL);
        m_pIntegracionParams->m_pSemaforoEsperarComando->Wait();
        LOG_TRACE("DicomizeCommand", "Espera de " << pCmd << " finalizada");
}

bool  DicomizeCommand::BorrarArchivosTemporales(wxString dirPath)
{
        //borra en cascada
        if(!wxRmdir(dirPath)) {
                //vaciar
                wxDir dir;
                if (dir.Open(dirPath)) {
                        wxString fileName;
                        bool cont = dir.GetFirst(&fileName);
                        while (cont) {
                                fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +fileName;
                                if(wxDir::Exists(fileName)) {
                                        if (BorrarArchivosTemporales(fileName)) {
                                                cont = dir.GetFirst(&fileName);
                                        } else {
                                                cont = dir.GetNext(&fileName);
                                        }
                                } else {
                                        if (wxRemoveFile(fileName)) {
                                                cont = dir.GetFirst(&fileName);
                                        } else {
                                                cont = dir.GetNext(&fileName);
                                        }
                                }
                        }
                }
        }
        if(wxDir::Exists(dirPath)) {
                return wxRmdir(dirPath);
        }
        return true;
}

void DicomizeCommand::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
        GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);

        if (pEvt == NULL  || pEvt->GetComando() == NULL || pEvt->GetComando() != m_pIntegracionParams->m_pComandoEspera) {
                LOG_TRACE("DicomizeCommand", "Evento ignorado. No corresponde al comando de interes");
                return;
        }

        if (pEvt->GetTipo() == GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado) {
                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pCmdHistoryParams= dynamic_cast< GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* > (m_pIntegracionParams->m_pParamsEspera);
                if (pCmdHistoryParams != NULL) {
                        m_pIntegracionParams->m_InsertedSeries = pCmdHistoryParams->m_InsertedSeries;
                }
                LOG_TRACE("DicomizeCommand", "Procesando evento de terminacion del subcomando avisar.");

                m_pIntegracionParams->m_pComandoEspera = NULL;
                m_pIntegracionParams->m_pParamsEspera = NULL;
                m_pIntegracionParams->m_pSemaforoEsperarComando->Post();
        }
}
}

