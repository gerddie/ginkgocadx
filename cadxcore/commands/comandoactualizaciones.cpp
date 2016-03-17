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
#include <api/internationalization/internationalization.h>

#include <wx/xml/xml.h>
#include <wx/msgdlg.h>
#include <wx/sstream.h>
#include <wx/url.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/pacscontroller.h>

#include <api/globals.h>

#include <main/entorno.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorlog.h>
#include <eventos/updateavailable.h>

#include "comandoactualizaciones.h"

#define IDC_CHECK_UPDATES 112

GNC::ComandoComprobarActualizacionesParams::ComandoComprobarActualizacionesParams(const std::string& url, bool informar) :
        m_URL(url),
        m_Informar(informar)
{
}

GNC::ComandoComprobarActualizacionesParams::~ComandoComprobarActualizacionesParams()
{
}

GNC::ComandoComprobarActualizaciones::ComandoComprobarActualizaciones(ComandoComprobarActualizacionesParams* pParams) :
        IComando(pParams, _Std("Check Updates")),
        m_pComprobarActualizacionesParams(pParams),
        m_Version(),
        m_Revision(0),
        m_Description(),
        m_URL()
{
        SetId(IDC_CHECK_UPDATES);
        CancelaA(IDC_CHECK_UPDATES);
}

const std::string& GNC::ComandoComprobarActualizaciones::GetError() const
{
        return m_Error;
}

const std::string& GNC::ComandoComprobarActualizaciones::GetVersion() const
{
        return m_Version;
}

const std::string& GNC::ComandoComprobarActualizaciones::GetDescription() const
{
        return m_Description;
}

int GNC::ComandoComprobarActualizaciones::GetRevision() const
{
        return m_Revision;
}


void GNC::ComandoComprobarActualizaciones::Execute()
{
//	NotificarProgreso(0.0, tarea);

        if (EstaAbortado()) {
                return;
        }

        std::string tmpFile = GNC::Entorno::Instance()->CreateGinkgoTempFile();
        try {
                if (!GIL::DICOM::PACSController::Instance()->DownloadFileFromURL(m_pComprobarActualizacionesParams->m_URL, tmpFile, this)) {
                        std::ostringstream ostr;
                        ostr << "Error downloading update file: " << m_pComprobarActualizacionesParams->m_URL << " check http proxy settings";
                        m_Error = ostr.str();
                        return;
                }
        } catch (GIL::DICOM::PACSException&) {
                std::ostringstream ostr;
                ostr << "Error downloading update file: " << m_pComprobarActualizacionesParams->m_URL << " check http proxy settings";
                m_Error = ostr.str();
                return;
        }


        try {
                wxXmlDocument xml;
                xml.Load(FROMPATH(tmpFile));
                wxXmlNode* raiz = xml.GetRoot();
                if (raiz == NULL) {
                        m_Error = "XML invalid";
                        return;
                }
                if (raiz->GetName() == wxT("ginkgo")) {
                        wxString propVal;

                        for (wxXmlNode* nodo = raiz->GetChildren(); nodo != NULL; nodo = nodo->GetNext()) {

                                if (nodo->GetName() == wxT("version")) {
                                        m_Version = std::string(nodo->GetNodeContent().Trim().ToUTF8());
                                } else if (nodo->GetName() == wxT("revision")) {
                                        long val;
                                        if ( nodo->GetNodeContent().Trim().ToLong(&val) ) {
                                                if (val >= 0) {
                                                        m_Revision = (int) val;
                                                }
                                        }
                                } else if (nodo->GetName() == wxT("description")) {
                                        m_Description = std::string(nodo->GetNodeContent().Trim().ToUTF8());
                                } else if (nodo->GetName() == wxT("url")) {
                                        m_URL = std::string(nodo->GetNodeContent().Trim().ToUTF8());
                                }
                        }
                }

                if (m_URL.empty()) {
                        m_Error = _Std("Not enough parameters found on XML");
                        Abort();
                        return;
                }

                //Sleep(3000);
        } catch (...) {
                m_Error = _Std("XML parsing internal error");
                Abort();
        }
}

void GNC::ComandoComprobarActualizaciones::Update()
{

        if (m_Error.size() > 0) {
                LOG_ERROR("Core/Update", m_Error);
                if (m_pComprobarActualizacionesParams->m_Informar) {
                        wxMessageBox(_("Error checking application updates"), _("Ginkgo CADx updates"), wxICON_ERROR);
                }
                return;
        }

        if (EstaAbortado()) {
                return;
        }
        try {

                GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoUpdateAvailable(m_Version, m_Revision, m_Description, m_URL, m_pComprobarActualizacionesParams->m_Informar));
        } catch (...) {
                LOG_ERROR("CORE/Updates", _Std("Internal error checking updates"));
        }

}


