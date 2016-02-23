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

