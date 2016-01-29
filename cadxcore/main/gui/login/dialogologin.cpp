/*
 *  
 *  $Id: dialogologin.cpp $
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
#include <ostream>
#include <wx/msgdlg.h>
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include "dialogologin.h"
#include <main/controllers/controladorautenticacion.h>
#include <main/controllers/controladorpermisos.h>
GNC::GUI::DialogoLogin::DialogoLogin(wxWindow* pParent) : DialogoLoginBase(pParent)
{
	if (GSEC::Auth::ControladorAutenticacion::Instance()->GetTipoAutenticacion() == GSEC::Auth::TA_PASSWORD)
	{
		m_pTCUsuario->SetValue(_("User"));
		m_pTCUsuario->Show(false);
		m_pLabelUser->Show(false);
		m_pFormPanel->Layout();
		m_pTCPassword->SetFocus();
	} else {
		m_pTCUsuario->SetFocus();
	}
}

GNC::GUI::DialogoLogin::~DialogoLogin()
{
}

void GNC::GUI::DialogoLogin::OnLogin( wxCommandEvent& /*event*/ )
{
	if (GSEC::Auth::ControladorAutenticacion::Instance()->Auth( std::string(m_pTCUsuario->GetValue().ToUTF8()), std::string(m_pTCPassword->GetValue().ToUTF8()) ) ) {
		this->SetReturnCode(wxID_OK);
		this->Hide();
	}
	else {
		this->SetReturnCode(wxID_CANCEL);
		m_pTCPassword->SelectAll();
		wxMessageBox(_("The username or password is incorrect"),_("Info"), wxOK | wxICON_INFORMATION, this);
	}
}
