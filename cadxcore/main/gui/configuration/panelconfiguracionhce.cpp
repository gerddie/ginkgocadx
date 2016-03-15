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

#include <vector>
#include <sstream>

#include <api/internationalization/internationalization.h>
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/configurationcontroller.h>

#include <endpoint/endpoint.h>

#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include <wx/xml/xml.h>

#ifndef _WIN32
#include <signal.h>
#endif

#include "panelconfiguracionhce.h"


GNC::GUI::PanelConfiguracionHCE::PanelConfiguracionHCE(wxWindow* pParent,IDialogoConfiguracion* pDialogo): PanelConfiguracionHCEBase(pParent), IPasoConfiguracion(pDialogo)
{
	Recargar();
}

GNC::GUI::PanelConfiguracionHCE::~PanelConfiguracionHCE()
{
}

void GNC::GUI::PanelConfiguracionHCE::Recargar()
{
	std::string value;
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","IDOrganizacionReceptora", value);
	m_pIdOrganizacionReceptora->SetValue(wxString::FromUTF8(value.c_str()));
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","IDSistemaPeticionario", value);
	m_pIdSistemaPeticionario->SetValue(wxString::FromUTF8(value.c_str()));
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","IDAplicacionRellenadora", value);
	m_pIdAplicacionRellenadora->SetValue(wxString::FromUTF8(value.c_str()));
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","IDGinkgoApplication", value, "GINKGO");
	m_pIdGinkgoAplication->SetValue(wxString::FromUTF8(value.c_str()));
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","HostnameMI", value);
	m_pHostnameMI->SetValue(wxString::FromUTF8(value.c_str()));
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","PuertoMI", value);
	m_pPuertoMI->SetValue(wxString::FromUTF8(value.c_str()));

}

//region "Metodos de IPasoConfiguracion"
wxWindow* GNC::GUI::PanelConfiguracionHCE::GetPanel()
{
	return this;
}

std::string GNC::GUI::PanelConfiguracionHCE::GetTitle()
{
	return _Std("Electronic health record");
}

std::string GNC::GUI::PanelConfiguracionHCE::GetCabecera()
{
	return _Std("Setting the electronic health record system");
}

bool GNC::GUI::PanelConfiguracionHCE::Validar()
{
	bool ok = true;
    // GW: Whatever this was meant to validate, no idea, eliminating dead code.

	//reset xml-rpc server
	return ok;
}

bool GNC::GUI::PanelConfiguracionHCE::Guardar()
{
	GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/HCE","IDOrganizacionReceptora", std::string(m_pIdOrganizacionReceptora->GetValue().ToUTF8()));
	GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/HCE","IDSistemaPeticionario", std::string(m_pIdSistemaPeticionario->GetValue().ToUTF8()));
	GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/HCE","IDAplicacionRellenadora", std::string(m_pIdAplicacionRellenadora->GetValue().ToUTF8()));
	GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/HCE","IDGinkgoApplication", std::string(m_pIdGinkgoAplication->GetValue().ToUTF8()));
	GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/HCE","IDGinkgoApplication", std::string(m_pIdGinkgoAplication->GetValue().ToUTF8()));
	GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/HCE","HostnameMI", std::string(m_pHostnameMI->GetValue().ToUTF8()));
	GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/HCE","PuertoMI", std::string(m_pPuertoMI->GetValue().ToUTF8()));

	return true;
}

void GNC::GUI::PanelConfiguracionHCE::OnTextoCambiado(wxCommandEvent &)
{
	OnPropiedadCambiada();
}

void GNC::GUI::PanelConfiguracionHCE::OnCambioSistema(wxCommandEvent &)
{
	OnPropiedadCambiada();
}

void GNC::GUI::PanelConfiguracionHCE::OnTestClick(wxCommandEvent &)
{

	try {
		//se abre una conexión tcp al host(puerto...)
		std::string url;
		{
			std::ostringstream ostr;
			ostr << m_pHostnameMI->GetValue().ToUTF8()<< ":" << m_pPuertoMI->GetValue().ToUTF8();
			url = ostr.str();
		}
		#ifndef _WIN32
		signal(SIGPIPE, SIG_IGN);
		#endif
		Endpoint::Initialize();

		wxBusyInfo* info = new wxBusyInfo(_("Testing conectivity with the server..."));
		Endpoint ep(TCP | CLIENT, url);
		delete info;

		if (!ep) {
			std::stringstream ostr;
			if (url.empty() || url.at(url.size() - 1) == ':' ) {
				ostr << _Std("Error connecting to url") << std::string(": ") << _Std("hostname/port not set");
			}
			else {
				ostr << _Std("Error connecting to url") << std::string(" ") << url;
			}
			wxMessageBox(wxString::FromUTF8(ostr.str().c_str()), _("Connection test fail"), wxICON_ERROR, this);
		} else {
			wxMessageBox(_("Connection test successful"),_("Info"),wxOK | wxICON_INFORMATION, this);
		}
		ep.Close();
	}
	catch (std::exception& ex) {
		LOG_ERROR("HL7Test", "Internal error testing integration server: " << ex.what() );
	}
}

void GNC::GUI::PanelConfiguracionHCE::OnCheckXMLRPC(wxCommandEvent&)
{
	OnPropiedadCambiada();
}

//endregion
