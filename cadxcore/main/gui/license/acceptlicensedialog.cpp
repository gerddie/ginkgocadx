/*
 *  
 *  $Id: acceptlicensedialog.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
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
