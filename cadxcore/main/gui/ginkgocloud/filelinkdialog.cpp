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

#include "filelinkdialog.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>
#include <main/controllers/controladorlog.h>

#include <wx/clipbrd.h>
#include <wx/dataobj.h>

GNC::GUI::FileLinkDialog::FileLinkDialog(wxWindow* pParent, std::string fileLink): GNC::GUI::FileLinkDialogBase(pParent)
{
	m_pFileLink->SetValue(wxString::FromUTF8(fileLink.c_str()));
	wxIcon icono;
	icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
	this->SetIcon(icono);
}

GNC::GUI::FileLinkDialog::~FileLinkDialog()
{
}

void GNC::GUI::FileLinkDialog::OnCopyClipboard(wxCommandEvent& )
{
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(new wxTextDataObject(m_pFileLink->GetValue()));
		wxTheClipboard->Close();
	}
}

void GNC::GUI::FileLinkDialog::OnAccept(wxCommandEvent &)
{
	Close();
}

void GNC::GUI::FileLinkDialog::OnKeyUp(wxKeyEvent &evt)
{
	if (evt.GetKeyCode() == WXK_ESCAPE) {
		Close();
	}
	evt.Skip();
}

void GNC::GUI::FileLinkDialog::OnSendMail(wxCommandEvent& )
{
	std::ostringstream ostr;
	ostr << "mailto:?Subject=" << _Std("New Ginkgo Cloud study") << "&body="<<_Std("To download the study, paste this url ");
	ostr <<":\n\n" << m_pFileLink->GetValue().ToUTF8();
	ostr <<"\n\n " << _Std("into \"Open Ginkgo Cloud link\" tool of Ginkgo CADx history, or paste it into the address bar of your browser.");


	std::string url = ostr.str();

	wxString wxUrl = wxString::FromUTF8(url.c_str());
	wxUrl.Replace(wxT(" "), wxT("%20"));
	wxUrl.Replace(wxT("\n"), wxT("%0A"));
	LOG_DEBUG("SendMail", "url: " << wxUrl.ToUTF8());
	wxLaunchDefaultBrowser(wxUrl);
}

