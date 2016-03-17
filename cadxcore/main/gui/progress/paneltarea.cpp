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

#include "paneltarea.h"
#include <main/controllers/commandcontroller.h>
#include <resources/ginkgoresourcesmanager.h>

#include <wx/image.h>
#include <wx/dc.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/wxginkgotoolbar.h>

#define ID_BUTTON_INIT_STOP 0

GNC::GUI::PanelTarea::PanelTarea( wxWindow* pPadre,long pidTarea) : PanelTareaBase(pPadre)
{
        m_PIDTarea = pidTarea;
        m_pProgress->SetColorSchema(GinkgoGauge::TCS_BLUE);

        wxGinkgoToolBar* m_pToolBar = new wxGinkgoToolBar(this, GetBackgroundColour());
        m_pToolBar->AddTool(ID_BUTTON_INIT_STOP,_("Start"),GinkgoResourcesManager::CineBar::GetIcoStop(),_("Stop"));
        m_pToolBar->Connect(ID_BUTTON_INIT_STOP,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GNC::GUI::PanelTarea::OnTareaCancelarClick),NULL,this);

        m_pToolBar->Realize();
        GetSizer()->Add(m_pToolBar,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);

        m_pProgress->SetMinSize(wxSize(-1, m_pToolBar->GetSize().y));
        Layout();
}

GNC::GUI::PanelTarea::~PanelTarea()
{

}

void GNC::GUI::PanelTarea::SetProgress(const wxString& text, float progress)
{
        m_pProgress->SetStatus(text);
        m_pProgress->SetProgress(progress);
}

wxString GNC::GUI::PanelTarea::GetStatus()
{
        return m_pProgress->GetStatus();
}
float GNC::GUI::PanelTarea::GetProgress()
{
        return m_pProgress->GetProgress();
}

void GNC::GUI::PanelTarea::OnTareaCancelarClick(wxCommandEvent &)
{
        GNC::GCS::ICommandController::Instance()->AbortarComando(m_PIDTarea, false);
}
