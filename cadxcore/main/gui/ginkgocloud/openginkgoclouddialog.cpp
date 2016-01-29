/*
*  
*  $Id: selectexternalfiles.cpp $
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

#include "openginkgoclouddialog.h"
#include <commands/cloudcommand.h>
#include <main/controllers/commandcontroller.h>
#include <resources/ginkgoresourcesmanager.h>

#include <wx/msgdlg.h>

GNC::GUI::OpenGinkgoCloudDialog::OpenGinkgoCloudDialog(wxWindow* pParent): GNC::GUI::OpenGinkgoCloudDialogBase(pParent)
{
	wxIcon icono;
	icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
	this->SetIcon(icono);

	m_pFileLink->SetFocus();
}

GNC::GUI::OpenGinkgoCloudDialog::~OpenGinkgoCloudDialog()
{
}

void GNC::GUI::OpenGinkgoCloudDialog::OnAccept(wxCommandEvent &)
{
	wxString link = m_pFileLink->GetValue().Trim(true).Trim(false);
	if (!link.StartsWith(wxT("ginkgocadx://cloud:"))) {
		wxMessageBox(_("Invalid link"), wxT("Ginkgo Cloud"), wxICON_WARNING|wxOK);
		return;
	} else {
		std::string stdLink(link.SubString(19, link.size()-1).ToUTF8());
		//call to command
		GNC::GCS::CloudGetCommandParams* pParams = new GNC::GCS::CloudGetCommandParams(stdLink, true);
		GNC::GCS::CloudCommand* pCmd = new GNC::GCS::CloudCommand(pParams);
		GNC::CommandController::Instance()->ProcessAsync("Downloading...", pCmd, NULL);
		//
		Close();
	}
}

void GNC::GUI::OpenGinkgoCloudDialog::OnCancel(wxCommandEvent& )
{
	Close();
}

void GNC::GUI::OpenGinkgoCloudDialog::OnKeyUp(wxKeyEvent &evt)
{
	if (evt.GetKeyCode() == WXK_ESCAPE) {
		Close();
	} else if (evt.GetKeyCode() == WXK_NUMPAD_ENTER || evt.GetKeyCode() == WXK_RETURN) {
		wxCommandEvent evt;
		OnAccept(evt);
	}
	evt.Skip();
}
