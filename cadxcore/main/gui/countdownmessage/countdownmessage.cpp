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

#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>

#include "countdownmessage.h"
#include <wx/ginkgostyle/ginkgostyle.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/controllers/inactivitycontroller.h>
#include <main/entorno.h>

namespace GNC
{
namespace GUI
{
CountdownMessage::CountdownMessage(wxWindow* pParent, const wxString title, const wxString& message, int seconds): CountdownMessageBase(pParent), m_last(seconds)
{
        m_pBody->SetToolTip(title);
        m_pFormPanel->SetToolTip(title);
        m_message = message;
        m_pFormPanel->Layout();
        m_pBody->Layout();
        Layout();
}

CountdownMessage::~CountdownMessage()
{
}

int CountdownMessage::ShowModalCountdown()
{
        Notify();
        return ShowModal();
}

bool CountdownMessage::Show(bool show)
{
        if (show && !IsShown()) {
                Notify();
        } else {
                if (IsRunning()) {
                        Stop();
                }
        }
        return CountdownMessageBase::Show(show);
}

void CountdownMessage::Notify()
{
        if (m_last > 0) {
                Freeze();
                m_last--;
                m_pLabelInactivity->SetLabel(wxString::Format(m_message, m_last));
                m_pLabelInactivity->Wrap(m_pFormPanel->GetSize().x - 20);
                m_pFormPanel->Layout();
                m_pBody->Layout();
                Layout();
                this->Start(1000, true);
                Thaw();
        } else {
                if (IsRunning()) {
                        Stop();
                }
                if (IsModal()) {
                        EndModal(wxID_OK);
                } else {
                        SetReturnCode(wxID_OK);
                        Close();
                }
        }
}
}
}
