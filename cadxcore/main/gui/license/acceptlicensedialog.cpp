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

#include <string>
#include <sstream>
#include "acceptlicensedialog.h"
#include <api/ientorno.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <api/internationalization/internationalization.h>
#include <main/controllers/anonusagestats.h>
#include <api/globals.h>
#include <main/license.h>
#include <wx/ginkgostyle/ginkgostyle.h>



GNC::GUI::AcceptLicenseDialog::AcceptLicenseDialog(wxWindow* pPadre):GNC::GUI::AcceptLicenseDialogBase(pPadre)
{
        m_pTextoLicencia->SetValue(wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetApplicationEula().c_str()));
        Layout();
        m_buttonAceptar->SetFocus();
        GNC::GCS::Permisos::EstadoPermiso updates = GNC::GCS::IControladorPermisos::Instance()->Get("core.update","check_updates");
        m_pCheckForUpdates->Show(updates);
        m_pCheckForUpdates->SetValue(updates);
#ifdef GINKGO_PRO
        m_pCheckAnonUsageStats->Show();
#else
        m_pCheckAnonUsageStats->Show(false);
#endif
        m_pFooter->Layout();
        Layout();
}

GNC::GUI::AcceptLicenseDialog::~AcceptLicenseDialog()
{
}

void GNC::GUI::AcceptLicenseDialog::OnAceptarClick(wxCommandEvent &event)
{
        GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/Estacion", "CheckForUpdates", m_pCheckForUpdates->GetValue());
        GNC::GCS::ConfigurationController::Instance()->Flush();
        event.Skip(true);
#ifdef GINKGO_PRO
        GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/General", "AnonUsageStats", m_pCheckAnonUsageStats->GetValue());
        GNC::GCS::ConfigurationController::Instance()->Flush();
        GNC::GCS::AnonUsageStats::Instance()->resetSettings();
#endif
        LOG_STAT(ANON_STAT_ACCEPT_LICENSE)
}
