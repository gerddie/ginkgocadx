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

#include <map>
#include <sstream>

#include <api/api.h>
#include <api/globals.h>

#include <api/controllers/ieventscontroller.h>

#include <eventos/integration.h>

#include <main/entorno.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/pacscontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/dcmtk/dicomservers.h>

#include <main/gui/countdownmessage/countdownmessage.h>

#include "sendpacscommand.h"

#include <wx/xml/xml.h>
#include <wx/mstream.h>
#include <wx/sstream.h>


#define IDC_PACS_BUSCAR                  61
#define IDC_PACS_DESCARGAR               62
#define IDC_PACS_ABORTAR_TODOS           63
#define IDC_PACS_SUBIR			         64

#define PARALLEL

#define DESCRIPTION _Std("Send files to PACS")
// Singleton de persistencia
namespace GADAPI
{
SendPACSCommand::SendPACSCommand(const std::string& selectedServer, const std::string& hl7Message, const std::list<long> seriesId) :
        GNC::GCS::IComando(NULL),
        m_selectedServer(selectedServer),
        m_seriesId(seriesId),
        m_hl7Message(hl7Message)
{
        checkDependencies();
}

SendPACSCommand::SendPACSCommand( const SendPACSCommand& o): GNC::GCS::IComando(NULL)
{
        (*this) = o;
        checkDependencies();
}

SendPACSCommand& SendPACSCommand::operator = (const SendPACSCommand& o)
{
        this->m_selectedServer = o.m_selectedServer;
        this->m_hl7Message = o.m_hl7Message;
        this->m_error = o.m_error;
        this->m_seriesId = o.m_seriesId;
        return *this;
}

SendPACSCommand::~SendPACSCommand()
{
}

void SendPACSCommand::checkDependencies()
{
        SetId(IDC_PACS_SUBIR);
        EsperaA(IDC_PACS_SUBIR);
#if !defined(PARALLEL)
        EsperaA(IDC_PACS_BUSCAR);
        EsperaA(IDC_PACS_DESCARGAR);
        EsperaA(IDC_PACS_SUBIR);
#endif
        AbortaSi(IDC_PACS_ABORTAR_TODOS);
}

std::string SendPACSCommand::getClassUID()
{
        return "2cc283db-3c88-4e91-ab2b-03fa01092f27";
}

std::string SendPACSCommand::serialize()
{
        wxXmlDocument documento;
        wxXmlNode* raiz = NULL;

        //si el funcionamiento es de n diagnosticos se guardan los widgets que hay
        raiz = new wxXmlNode(NULL,wxXML_ELEMENT_NODE, wxT("SendPacsCommand"));
        documento.SetFileEncoding(wxT("UTF-8"));
        documento.SetRoot(raiz);
        raiz->AddAttribute(wxT("pacs"),wxString::FromUTF8(m_selectedServer.c_str()));
        raiz->AddAttribute(wxT("hl7"), wxString::FromUTF8(m_hl7Message.c_str()));
        wxStringOutputStream out;
        documento.Save(out);
        return std::string(out.GetString().ToUTF8());
}

void SendPACSCommand::init(const std::string& serializedValue)
{
        wxString wxSerializedValue = wxString::FromUTF8(serializedValue.c_str());
        wxStringInputStream istream(wxSerializedValue);
        wxXmlDocument doc(istream);
        wxXmlNode* root = doc.GetRoot();
        if (root != NULL) {
                m_selectedServer = root->GetAttribute(wxT("pacs"),wxEmptyString).ToUTF8();
                m_hl7Message = root->GetAttribute(wxT("hl7"),wxEmptyString).ToUTF8();
        } else {
                //error
                LOG_ERROR("SendPACSCommand", "Error deserializing SendPACSCommand serialized value-"<< serializedValue << " -");
        }
}

void SendPACSCommand::Execute()
{
        if(m_selectedServer == "") {
                //usaremos el primer servidor (por defecto)
                if(!DicomServerList::Instance()->Empty()) {
                        m_selectedServer = DicomServerList::Instance()->GetDefaultServer()->ID;
                } else {
                        throw GIL::DICOM::PACSException(_Std("There is not any Remote PACS configured"));
                }
        }

        //one send operation for each tsyntax uid
        std::map<std::string, std::list<std::string> > mapOfTSyntax;

        for (std::list<long>::const_iterator it = m_seriesId.begin(); it != m_seriesId.end(); ++it) {
                GNC::GCS::HistoryController::LightFileModelList listOfFiles;
                GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels((*it), listOfFiles);
                for (GNC::GCS::HistoryController::LightFileModelList::iterator itFiles = listOfFiles.begin(); itFiles != listOfFiles.end(); itFiles++) {
                        if (mapOfTSyntax.find((*itFiles).tsuid) == mapOfTSyntax.end()) {
                                mapOfTSyntax[(*itFiles).tsuid] = std::list<std::string>();
                        }
                        mapOfTSyntax[(*itFiles).tsuid].push_back((*itFiles).real_path);
                }
        }

        std::string tarea = _Std("Uploading files ...");
        if (!NotificarProgreso(0.0f,tarea)) {
                return;
        }
        GIL::DICOM::IPACSController* pCI = NULL;
        try {
                pCI = GNC::Entorno::Instance()->GetPACSController();
                pCI->GetConnection(this);
                if (GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits", "pacs_upload")) {
                        for (std::map<std::string, std::list<std::string> >::iterator it = mapOfTSyntax.begin(); it != mapOfTSyntax.end(); ++it) {
                                pCI->SubirArchivos(this, m_selectedServer, (*it).second,this,GIL::DICOM::SendDefault);
                        }

                        if (!m_hl7Message.empty()) {
                                GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::HL7SendRequestEvent(m_hl7Message));
                        }
                } else {
                        LOG_INFO("C-STORE", "No se enviara el estudio al PACS porque el permiso de subida lo impide.");
                }
        } catch (GinkgoNoServerFoundException&) {
                m_error = _Std("Error: PACS entry configuration not found: ") + m_selectedServer;
        } catch (GIL::DICOM::PACSException& ex) {
                m_error = _Std("Error:") + (const std::string)ex;
        } catch (std::exception& ex) {
                m_error = _Std("Error sending evidences:") + ex.what();
        } catch (...) {
                m_error = _Std("Error sending evidences: Internal error.");
        }
        if (pCI) {
                pCI->ReleaseConnection(this);
        }
}

void SendPACSCommand::Update()
{
        if (EstaAbortado()) {
                return;
        }

        if(m_error!="") {
                wxString message = _("Error uploading the study to PACS: ");
                message += wxT("\n") + wxString::FromUTF8(m_error.c_str());
                message += _("\nWould you like to retry(automatically retried in %d seconds)?");
                GNC::GUI::CountdownMessage modalDlg(NULL, _("Retry send to PACS"), message);
                int returnCode = modalDlg.ShowModalCountdown();
                switch (returnCode) {
                case wxID_OK: {
                        //retry; copy and execute...
                        SendPACSCommand* pCmd = new SendPACSCommand(*this);
                        GNC::CommandController::Instance()->ProcessAsync("Sending...",pCmd, NULL);
                }
                break;
                default:
                        //set status error
                        break;
                }

                //GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, _Std("Error uploading the study to PACS: ") + "\n" + m_errorDescription, GNC::GCS::Events::EventoMensajes::PopUpMessage,false, GNC::GCS::Events::EventoMensajes::Error));
                return ;
        } else {
                /*			if (m_pPACSParams->m_informar) {
                				GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL,_Std("Study successfully sent to PACS"),GNC::GCS::Events::EventoMensajes::PopUpMessage,false, GNC::GCS::Events::EventoMensajes::Informacion));
                			}*/
                return;
        }
}

bool SendPACSCommand::NotificarProgreso(float progresoNormalizado,const std::string &texto)
{
        if (EstaAbortado()) {
                return false;
        }
        return IComando::NotificarProgreso(progresoNormalizado, texto);
}

void SendPACSCommand::LiberarRecursos()
{
}

}
