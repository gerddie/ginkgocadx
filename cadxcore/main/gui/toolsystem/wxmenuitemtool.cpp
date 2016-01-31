/*
 *  
 *  $Id: wxmenuitemtool.cpp$
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "wxmenuitemtool.h"

#include <api/toolsystem/itool.h>
#include <api/toolsystem/itoolcontroller.h>

#include <wx/menuitem.h>


GNC::GUI::wxMenuItemTool::wxMenuItemTool(wxMenu* pMenuPadre, long id, const wxString& name,
                                         const wxBitmap& /*bitmap*/, wxEvtHandler* pHandler, int type):
        wxMenuItem(pMenuPadre, id, name, name, type == wxITEM_CHECK ? wxITEM_CHECK : wxITEM_NORMAL)
{
	m_pHandler = pHandler;
	//this->SetBitmap(bitmap);
}

GNC::GUI::wxMenuItemTool::~wxMenuItemTool() {
	if(m_pHandler != NULL) {
		delete m_pHandler;
		m_pHandler = NULL;
	}
}


GNC::GUI::wxDefaultEvtHandlerTool::wxDefaultEvtHandlerTool(wxEvtHandler* pParent, GNC::GCS::ITool* pTool):wxEvtHandler()
{
	m_pParent = pParent;
	m_pTool=pTool;
	// Connect Events
	m_pParent->Connect(m_pTool->ID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxDefaultEvtHandlerTool::OnToolClick),NULL,this);
	m_pParent->Connect(m_pTool->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxDefaultEvtHandlerTool::OnUpdateUI),NULL,this);
}

GNC::GUI::wxDefaultEvtHandlerTool::~wxDefaultEvtHandlerTool()
{
	m_pParent->Disconnect(m_pTool->ID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxDefaultEvtHandlerTool::OnToolClick),NULL,this);
	m_pParent->Disconnect(m_pTool->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxDefaultEvtHandlerTool::OnUpdateUI),NULL,this);
}

void GNC::GUI::wxDefaultEvtHandlerTool::OnToolClick(wxCommandEvent& event)
{
	GNC::GCS::TriggerButton button;
	button.EnableLeft();
	event.Skip(false);
	if (m_pTool->GetToolBar() != NULL) {
		m_pTool->GetToolBar()->ActivateTool(m_pTool->ID, button, NULL);
	}
	if (m_pTool->AppendsInGlobalMenu()) {
		m_pTool->ExecuteAction();
	}
}

void GNC::GUI::wxDefaultEvtHandlerTool::OnUpdateUI(wxUpdateUIEvent& event)
{
	event.Enable(m_pTool->IsEnabled());
}

GNC::GUI::wxMenuTool::wxMenuTool(wxEvtHandler* pHandler) 
{
	m_pHandler = pHandler;
}

GNC::GUI::wxMenuTool::~wxMenuTool() {
	if(m_pHandler != NULL) {
		delete m_pHandler;
		m_pHandler = NULL;
	}
}
