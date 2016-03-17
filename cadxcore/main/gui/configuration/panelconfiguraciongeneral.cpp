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

#include <wx/checkbox.h>
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <sstream>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>

#include <wx/ginkgostyle/ginkgostyle.h>

#include "panelconfiguraciongeneral.h"

#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/autocleaningcontroller.h>
#include <main/gui/history3/historypanel3.h>
#include <main/entorno.h>
#include <api/controllers/icontroladorinternacionalizacion.h>
#include <api/internationalization/internationalization.h>

#include <main/controllers/anonusagestats.h>

namespace GNC
{
namespace GUI
{
class ProxySettingsDialog: public ProxySettingsDialogBase
{
public:
        ProxySettingsDialog(wxWindow* pParent): ProxySettingsDialogBase(pParent)
        {
        }
        ~ProxySettingsDialog()
        {
        }
        void SetProxySettings(const GNC::GCS::ProxySettings& settings)
        {
                m_pHost->SetValue(wxString::FromUTF8(settings.hostName.c_str()));
                m_pPort->SetValue(wxString::Format(wxT("%d"), settings.port));
                m_pCheckAuthentication->SetValue(settings.requiresAuth);
                m_pUsername->SetValue(wxString::FromUTF8(settings.userName.c_str()));
                m_pPassword->SetValue(wxString::FromUTF8(settings.password.c_str()));
                m_pUsername->Enable(settings.requiresAuth);
                m_pPassword->Enable(settings.requiresAuth);
                m_pExceptions->SetValue(wxString::FromUTF8(settings.exceptions.c_str()));
        }

        GNC::GCS::ProxySettings GetProxySettings()
        {
                GNC::GCS::ProxySettings settings;
                settings.exceptions = m_pExceptions->GetValue().ToUTF8();
                settings.hostName = m_pHost->GetValue().ToUTF8();
                long portTmp;
                m_pPort->GetValue().ToLong(&portTmp);
                settings.port = (int)portTmp;
                settings.requiresAuth = m_pCheckAuthentication->IsChecked();
                if (settings.requiresAuth) {
                        settings.password = m_pPassword->GetValue().ToUTF8();
                        settings.userName = m_pUsername->GetValue().ToUTF8();
                }
                settings.useProxy = true;
                return settings;
        }

        virtual void OnCheckAuthentication(wxCommandEvent &/*evt*/)
        {
                m_pUsername->Enable(m_pCheckAuthentication->IsChecked());
                m_pPassword->Enable(m_pCheckAuthentication->IsChecked());
        }

        virtual void OnOkClick(wxCommandEvent& evt)
        {
                long port;
                if (!m_pPort->GetValue().ToLong(&port)) {
                        wxMessageBox(_("Port has to be a valid number"),_("Info"));
                        evt.Skip(false);
                }
                EndModal(wxID_OK);
        }
};

PanelConfiguracionGeneral::PanelConfiguracionGeneral(wxWindow* pParent,IDialogoConfiguracion* pDialogo): PanelConfiguracionGeneralBase(pParent), IPasoConfiguracion(pDialogo)
{
        m_idiomaCambiado = false;
        pProxySettings = new GNC::GCS::ProxySettings();
        Recargar();
#ifdef GINKGO_PRO
        m_pCheckAnonUsageStats->Show();
#else
        m_pCheckAnonUsageStats->Show(false);
#endif
        m_pUpdate->Layout();
        Layout();
}

PanelConfiguracionGeneral::~PanelConfiguracionGeneral()
{
        delete pProxySettings;
}

void PanelConfiguracionGeneral::Recargar()
{
        //check for updates
        bool check;
        GNC::GCS::ConfigurationController::Instance()->readBoolGeneral("/GinkgoCore/Estacion", "CheckForUpdates", check, true);
        m_pCheckForUpdates->SetValue(check);


        //internacionalizacion
        int language;
        GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station", "Language", language);

        //explore lang directory to find installed languages
        {
                m_pComboLanguage->Clear();
                m_mapLanguages.clear();
                m_mapLanguages[_Std("Default")] = -1;
                m_mapLanguages["English"] = (int)wxLANGUAGE_ENGLISH;
                wxString wxDirPath = FROMPATH(GNC::Entorno::Instance()->GetGinkgoLanguageDir());
                wxDir dir;
                if (dir.Open(wxDirPath)) {
                        wxString wxPathLang;
                        bool cont = dir.GetFirst(&wxPathLang,wxEmptyString, wxDIR_DIRS);
                        while (cont) {
                                const wxLanguageInfo* pInfo = wxLocale::FindLanguageInfo(wxPathLang);
                                if (pInfo != NULL) {
                                        m_mapLanguages[std::string(pInfo->Description.ToUTF8())] = pInfo->Language;
                                }
                                cont = dir.GetNext(&wxPathLang);
                        }
                }
                for (TMapLanguages::iterator it  = m_mapLanguages.begin(); it != m_mapLanguages.end(); ++it) {
                        int position = m_pComboLanguage->Append(wxString::FromUTF8((*it).first.c_str()));
                        if ((*it).second == language) {
                                m_pComboLanguage->Select(position);
                        }
                }
                if (m_pComboLanguage->GetSelection() == -1) {
                        m_pComboLanguage->Select(m_pComboLanguage->FindString(_("Default")));
                }
        }

        //anon usage
#ifdef GINKGO_PRO
        bool anonUsage;
        GNC::GCS::ConfigurationController::Instance()->readBoolGeneral("/GinkgoCore/General", "AnonUsageStats", anonUsage, false);
        m_pCheckAnonUsageStats->SetValue(anonUsage);
#endif

        //proxy Settings
        {
                *pProxySettings = GNC::GCS::ConfigurationController::Instance()->GetProxySettings();
                m_pEnableProxy->SetValue(pProxySettings->useProxy);
                m_pProxySettingsButton->Enable(pProxySettings->useProxy);
        }
}

//region "Metodos de IPasoConfiguracion"
wxWindow* PanelConfiguracionGeneral::GetPanel()
{
        return this;
}

std::string PanelConfiguracionGeneral::GetTitle()
{
        return _Std("General");
}

std::string PanelConfiguracionGeneral::GetCabecera()
{
        return _Std("General configuration parameters");
}

bool PanelConfiguracionGeneral::Validar()
{
        if(m_idiomaCambiado) {
                wxMessageBox(_("Ginkgo CADx must restart for the changes to take effect"),_("Info"));
        }

        return true;
}

bool PanelConfiguracionGeneral::Guardar()
{
        GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/Estacion", "CheckForUpdates", m_pCheckForUpdates->GetValue());

        //internacionalizacion
        if (m_pComboLanguage->GetStringSelection() != _("Default")) {
                std::string selectedLanguage(m_pComboLanguage->GetStringSelection().ToUTF8());
                for (TMapLanguages::iterator it  = m_mapLanguages.begin(); it != m_mapLanguages.end(); ++it) {
                        if ((*it).first == selectedLanguage) {
                                GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/Station", "Language", (*it).second);
                                break;
                        }
                }
        } else {
                GNC::GCS::ConfigurationController::Instance()->deleteEntryUser("/GinkgoCore/Station", "Language");
        }
        //anon usage
#ifdef GINKGO_PRO
        GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/General", "AnonUsageStats", m_pCheckAnonUsageStats->GetValue());
        GNC::GCS::ConfigurationController::Instance()->Flush();
        GNC::GCS::AnonUsageStats::Instance()->resetSettings();
#endif

        //proxy
        GNC::GCS::ConfigurationController::Instance()->SetProxySettings(*pProxySettings);

        return true;
}
//endregion

void PanelConfiguracionGeneral::OnCheckForUpdates(wxCommandEvent &)
{
        if(IsShown()) {
                OnPropiedadCambiada();
        }
}


void PanelConfiguracionGeneral::OnTextoCambiado(wxCommandEvent &)
{
        if(IsShown()) {
                OnPropiedadCambiada();
        }
}

void PanelConfiguracionGeneral::OnIdiomaCambiado(wxCommandEvent &)
{
        if(IsShown()) {
                OnPropiedadCambiada();
                m_idiomaCambiado = true;
        }
}

void PanelConfiguracionGeneral::OnProxySettings(wxCommandEvent &)
{
        ProxySettingsDialog dlg(this);
        dlg.SetProxySettings(*pProxySettings);
        if (dlg.ShowModal() == wxID_OK) {
                *pProxySettings = dlg.GetProxySettings();
                OnPropiedadCambiada();
        }
}

void PanelConfiguracionGeneral::OnCheckEnableProxySettings(wxCommandEvent &)
{
        m_pProxySettingsButton->Enable(m_pEnableProxy->IsChecked());
        pProxySettings->useProxy = m_pEnableProxy->IsChecked();
        OnPropiedadCambiada();
}
}
}
