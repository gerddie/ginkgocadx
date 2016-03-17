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
        if (GSEC::Auth::ControladorAutenticacion::Instance()->GetTipoAutenticacion() == GSEC::Auth::TA_PASSWORD) {
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
        } else {
                this->SetReturnCode(wxID_CANCEL);
                m_pTCPassword->SelectAll();
                wxMessageBox(_("The username or password is incorrect"),_("Info"), wxOK | wxICON_INFORMATION, this);
        }
}
