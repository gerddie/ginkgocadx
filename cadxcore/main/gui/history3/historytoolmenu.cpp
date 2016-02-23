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

//#define _GINKGO_TRACE

#include <api/globals.h>
#include <api/ilock.h>
#include <set>
#include <wx/ginkgostyle/ginkgostyle.h>

#include "historytoolmenu.h"
#include "historypanel3.h"
#include <resources/ginkgoresourcesmanager.h>
#include <main/entorno.h>
#include <main/controllers/controladorpermisos.h>


#define SIZE_ICONOS 16

#define ID_REMOVE				1766
#define ID_REMOVE_ALL		1780
#define ID_FILE_DROP_DOWN	1781
#define ID_SELECT_ALL	1782
#define ID_DESELECT		1783
#define ID_CLOSE_HISTORY		1784

GNC::GUI::HistoryToolMenu::HistoryToolMenu(HistoryPanel3* pParent) : wxGinkgoToolBar(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE )
{
	HistoryPanel = pParent;
	
	SetToolBitmapSize(wxSize(16,16));

	//drop down
	this->Connect(ID_FILE_DROP_DOWN,wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN, wxAuiToolBarEventHandler(HistoryToolMenu::OnDropDownOpenFile),NULL, this );
	this->Connect(ID_REMOVE,wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN, wxAuiToolBarEventHandler(HistoryToolMenu::OnDropDownRemove),NULL, this );

	this->Connect( ID_REMOVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnRemove),NULL, this );
	this->Connect( ID_REMOVE_ALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnRemoveAll),NULL, this );

	Reload();
}

GNC::GUI::HistoryToolMenu::~HistoryToolMenu()
{
	this->Disconnect(ID_FILE_DROP_DOWN,wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN, wxAuiToolBarEventHandler(HistoryToolMenu::OnDropDownOpenFile),NULL, this );
	this->Disconnect(ID_REMOVE,wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN, wxAuiToolBarEventHandler(HistoryToolMenu::OnDropDownRemove),NULL, this );

	this->Disconnect( ID_REMOVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnRemove),NULL, this );
	this->Disconnect( ID_REMOVE_ALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnRemoveAll),NULL, this );
}


void GNC::GUI::HistoryToolMenu::Reload()
{
	wxMenuBar* pMenuBar = HistoryPanel->m_pMenuBar;
	while (pMenuBar->GetMenuCount() > 0)
		delete pMenuBar->Remove(0);
	
	wxMenu* pHistoryMenu = new wxMenu();
	wxMenu* pEditMenu = new wxMenu();
	wxMenu* pExportMenu = new wxMenu();

	//se vacía la barra y se borran los menus que se crearon en la anterior pasada
	wxWindowDisabler dis;
	while (GetToolCount() > 0) {
		DeleteByIndex(0);
	}

	{
		//acquisition
		if(GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire")) {
			AddTool(ID_FILE_DROP_DOWN,_("Acquire"),GinkgoResourcesManager::MenuIcons::GetIcoOpenDir(),_("Acquire"));
			ForceSetToolDropDown(ID_FILE_DROP_DOWN, true);
			AddSeparator();
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pHistoryMenu, GNC::GCS::IHistoryTool::TFamily_Open);
			pHistoryMenu->AppendSeparator();
		}		
		//dicomization
		if(GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","import")) {
			GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_Dicomize);
			AddSeparator();
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pHistoryMenu, GNC::GCS::IHistoryTool::TFamily_Dicomize);
			pHistoryMenu->AppendSeparator();
		}
		//Q/R
		if(GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_acquisition")) {
			GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_Q_R);
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pHistoryMenu, GNC::GCS::IHistoryTool::TFamily_Q_R);
		}	
		AddSeparator();

		AddTool(ID_REMOVE,_("Remove"),GinkgoResourcesManager::MenuIcons::GetIcoEliminar(),_("Remove"));
		ForceSetToolDropDown(ID_REMOVE, true);
		GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_Delete);
		AddSeparator();
		AppendEditTools(pEditMenu, HistoryPanel);
		GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pEditMenu, GNC::GCS::IHistoryTool::TFamily_Delete);
		pEditMenu->AppendSeparator();
		GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_ShowMetadata);
		AddSeparator();
		GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pEditMenu, GNC::GCS::IHistoryTool::TFamily_ShowMetadata);
		pEditMenu->AppendSeparator();
		if(GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_upload")) {
			GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_Send);
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pExportMenu, GNC::GCS::IHistoryTool::TFamily_Send);
			pExportMenu->AppendSeparator();
		} 
		AddSeparator();
		GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_Anonymize);
		AddSeparator();
		GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pEditMenu, GNC::GCS::IHistoryTool::TFamily_Anonymize);
		pEditMenu->AppendSeparator();
		if(GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","export")) {
			GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_Export);
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pExportMenu, GNC::GCS::IHistoryTool::TFamily_Export);
			pExportMenu->AppendSeparator();
		}
		//todo grabar cds
		GNC::HistoryToolsController::Instance()->AppendsToolInToolBar(this, GNC::GCS::IHistoryTool::TFamily_Other);
		GNC::HistoryToolsController::Instance()->AppendsToolInMenu(HistoryPanel, pExportMenu, GNC::GCS::IHistoryTool::TFamily_Other);
		pExportMenu->AppendSeparator();

		if (pHistoryMenu->GetMenuItemCount() > 0) {
			while (pHistoryMenu->GetMenuItems().back()->IsSeparator()) {
				pHistoryMenu->Delete(pHistoryMenu->GetMenuItems().back());
			}
			pHistoryMenu->AppendSeparator();
			pHistoryMenu->Append(ID_CLOSE_HISTORY, _("Close history")+wxString(wxT(" \tAlt+q")), _("Close history"));
			pMenuBar->Append(pHistoryMenu, _("History"));
			HistoryPanel->Connect( ID_CLOSE_HISTORY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnCloseWindow),NULL, this );
		} else {
			delete pHistoryMenu;
		}

		if (pEditMenu->GetMenuItemCount() > 0) {
			while (pEditMenu->GetMenuItems().back()->IsSeparator()) {
				pEditMenu->Delete(pEditMenu->GetMenuItems().back());
			}
			pMenuBar->Append(pEditMenu, _("Edit"));
		} else {
			delete pEditMenu;
		}		

		if (pExportMenu->GetMenuItemCount() > 0) {
			while (pExportMenu->GetMenuItems().back()->IsSeparator()) {
				pExportMenu->Delete(pExportMenu->GetMenuItems().back());
			}
			pMenuBar->Append(pExportMenu, _("Export"));
		} else {
			delete pExportMenu;
		}

		Realize();
	}
}

void GNC::GUI::HistoryToolMenu::OnDropDownOpenFile(wxAuiToolBarEvent& event)
{
	wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());

	tb->SetToolSticky(event.GetId(), true);

	// create the popup menu
	wxMenu menuPopup;

	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menuPopup, &menuPopup, GNC::GCS::IHistoryTool::TFamily_Open);

	// line up our menu with the button
	wxRect rect = tb->GetToolRect(event.GetId());
	wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
	pt = ScreenToClient(pt);

	PopupMenu(&menuPopup, pt);

	// make sure the button is "un-stuck"
	tb->SetToolSticky(event.GetId(), false);
}

void GNC::GUI::HistoryToolMenu::OnDropDownRemove(wxAuiToolBarEvent& event)
{
	if (event.IsDropDownClicked())
	{
		wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());

		tb->SetToolSticky(event.GetId(), true);

		// create the popup menu
		wxMenu menuPopup;

		wxMenuItem* pMenuRemove = new wxMenuItem( &menuPopup, ID_REMOVE, wxString( _("Remove")), _("Remove"), wxITEM_NORMAL );
		wxMenuItem* pMenuRemoveAll = new wxMenuItem( &menuPopup, ID_REMOVE_ALL, wxString( _("Clear history")), _("Clear history"), wxITEM_NORMAL );
		#ifdef __WXMSW__
		pMenuRemove->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoEliminar());
		pMenuRemoveAll->SetBitmaps(GinkgoResourcesManager::History::GetIcoCleanAll());
		#else
		pMenuRemove->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEliminar());
		pMenuRemoveAll->SetBitmap(GinkgoResourcesManager::History::GetIcoCleanAll());
		#endif

		menuPopup.Append( pMenuRemove );
		menuPopup.Append( pMenuRemoveAll );

		// line up our menu with the button
		wxRect rect = tb->GetToolRect(event.GetId());
		wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
		pt = ScreenToClient(pt);

		PopupMenu(&menuPopup, pt);

		// make sure the button is "un-stuck"
		tb->SetToolSticky(event.GetId(), false);
	} else {
		event.Skip(true);
	}	
}

void GNC::GUI::HistoryToolMenu::AppendEditTools(wxMenu* pEditMenu, wxEvtHandler* pHandler)
{	
	wxMenuItem* pMenuRemove = new wxMenuItem( pEditMenu, ID_REMOVE, wxString( _("Remove")), _("Remove"), wxITEM_NORMAL );
	wxMenuItem* pMenuRemoveAll = new wxMenuItem( pEditMenu, ID_REMOVE_ALL, wxString( _("Clear history")), _("Clear history"), wxITEM_NORMAL );

	wxMenuItem* pMenuSelect = new wxMenuItem( pEditMenu, ID_SELECT_ALL, wxString( _("Select all")), _("Select all"), wxITEM_NORMAL );
	wxMenuItem* pMenuDeselect = new wxMenuItem( pEditMenu, ID_DESELECT, wxString( _("Deselect all")), _("Deselect all"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	pMenuRemove->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoEliminar());
	pMenuRemoveAll->SetBitmaps(GinkgoResourcesManager::History::GetIcoCleanAll());
	#else
	pMenuRemove->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEliminar());
	pMenuRemoveAll->SetBitmap(GinkgoResourcesManager::History::GetIcoCleanAll());
	#endif

	pEditMenu->Append( pMenuRemove );
	pEditMenu->Append( pMenuRemoveAll );
	pEditMenu->AppendSeparator();
	pEditMenu->Append( pMenuSelect );
	pEditMenu->Append( pMenuDeselect );
	
	pHandler->Connect( ID_REMOVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnRemove),NULL, this );
	pHandler->Connect( ID_REMOVE_ALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnRemoveAll),NULL, this );
	pHandler->Connect( ID_SELECT_ALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnSelectAll),NULL, this );
	pHandler->Connect( ID_DESELECT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HistoryToolMenu::OnDeselectAll),NULL, this );
}

void GNC::GUI::HistoryToolMenu::OnRemove(wxCommandEvent& /*event*/)
{
	HistoryPanel->RemoveSelected();
}

void GNC::GUI::HistoryToolMenu::OnRemoveAll(wxCommandEvent& /*event*/)
{
	HistoryPanel->ClearHistory();
}

void GNC::GUI::HistoryToolMenu::OnSelectAll(wxCommandEvent& /*event*/)
{
	HistoryPanel->SelectAll();
}

void GNC::GUI::HistoryToolMenu::OnDeselectAll(wxCommandEvent& /*event*/)
{
	HistoryPanel->DeselectAll();
}

void GNC::GUI::HistoryToolMenu::OnCloseWindow(wxCommandEvent& /*event*/)
{
	HistoryPanel->Show(false);
}

