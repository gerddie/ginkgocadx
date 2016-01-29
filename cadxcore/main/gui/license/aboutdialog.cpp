/*
 *
 *  $Id: aboutdialog.cpp $
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
#include "aboutdialog.h"
#include <api/globals.h>

#include <api/ientorno.h>
#include <main/controllers/controladorextensiones.h>
#include <resources/ginkgoresourcesmanager.h>
#include "dialogolicenciasopensource.h"
#include <wx/ginkgostyle/ginkgostyle.h>

AboutDialog::AboutDialog(wxWindow* pPadre):AboutDialogBase(pPadre)
{
	wxString title = _("About");
	title += wxT(" ");
	title += wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetApplicationName().c_str()) + wxT("...");
	SetTitle( title);
	m_pHeader->SetTitle(wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetApplicationName().c_str()));
	m_pHeader->SetSubtitle(wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetApplicationSubTitle().c_str()));

	std::ostringstream ostr;
	ostr << "<html><body><h1>";
	ostr << GNC::GCS::IEntorno::Instance()->GetApplicationName() << " " << GNC::GCS::IEntorno::Instance()->GetGinkgoVersionString() << "</h1>";
	ostr << GNC::GCS::IEntorno::Instance()->GetApplicationCredits();
	ostr << "</body></html>";
	
	m_pCredits->SetPage(wxString::FromUTF8(ostr.str().c_str()));

	Layout();
}

AboutDialog::~AboutDialog()
{

}

void AboutDialog::OnLicenciasOpenSource(wxCommandEvent &)
{
	DialogoLicenciasOpenSource dlg (this);
	dlg.ShowModal();
}

void AboutDialog::OnSize(wxSizeEvent &event)
{
	Layout();
	event.Skip(true);
}

void AboutDialog::OnTextURL(wxHtmlLinkEvent &event)
{
	wxLaunchDefaultBrowser(event.GetLinkInfo().GetHref());
}

