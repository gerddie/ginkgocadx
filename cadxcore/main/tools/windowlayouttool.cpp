/*
 *  
 *  $Id: windowlayouttool.cpp $
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
#include "windowlayouttool.h"
#if defined(ENABLE_WINDOWLAYOUTTOOL)

#include <wx/menu.h>
#include <api/globals.h>
#include <export/contracts/iwindowlayoutcontract.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>


#include <resources/ginkgoresourcesmanager.h>

#define ADD_FILA  1601
#define ADD_COL	1602
#define DEL_FILA  1603
#define DEL_COL	1604

#define LAY_1_1	1605
#define LAY_1_2	1606
#define LAY_2_1	1607
#define LAY_2_2	1608
#define LAY_3_3	1609
#define LAY_4_4	1610

#define SPLIT_H	1611
#define SPLIT_V	1612

//layouts por defecto...
namespace GNC {
	namespace GUI {
		class wxEventHandlerWindowLayout: public wxEvtHandler
		{
		public:
			wxEventHandlerWindowLayout(wxEvtHandler* pParent, GNC::WindowLayoutTool* pHerramienta): wxEvtHandler()
			{
				m_pParent = pParent;
				m_pHerramienta=pHerramienta;

				m_pParent->Connect(ADD_COL,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnAddColumn),NULL,this);
				m_pParent->Connect(ADD_FILA,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnAddRow),NULL,this);
				m_pParent->Connect(DEL_COL,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnDelColumn),NULL,this);
				m_pParent->Connect(DEL_FILA,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnDelRow),NULL,this);

				m_pParent->Connect(LAY_1_1,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay1x1),NULL,this);
				m_pParent->Connect(LAY_1_2,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay1x2),NULL,this);
				m_pParent->Connect(LAY_2_1,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay2x1),NULL,this);
				m_pParent->Connect(LAY_2_2,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay2x2),NULL,this);
				m_pParent->Connect(LAY_3_3,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay3x3),NULL,this);
				m_pParent->Connect(LAY_4_4,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay4x4),NULL,this);

				m_pParent->Connect(SPLIT_V,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnSplitV),NULL,this);
				m_pParent->Connect(SPLIT_H,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnSplitH),NULL,this);

				m_pParent->Connect(ADD_COL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnAddColumnUpdate),NULL,this);
				m_pParent->Connect(ADD_FILA, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnAddRowUpdate),NULL,this);
				m_pParent->Connect(DEL_COL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnDelColumnUpdate),NULL,this);
				m_pParent->Connect(DEL_FILA, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnDelRowUpdate),NULL,this);

				m_pParent->Connect(LAY_1_1, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay1x1Update),NULL,this);
				m_pParent->Connect(LAY_1_2, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay1x2Update),NULL,this);
				m_pParent->Connect(LAY_2_1, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay2x1Update),NULL,this);
				m_pParent->Connect(LAY_2_2, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay2x2Update),NULL,this);
				m_pParent->Connect(LAY_3_3, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay3x3Update),NULL,this);
				m_pParent->Connect(LAY_4_4, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay4x4Update),NULL,this);

				m_pParent->Connect(SPLIT_V, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnSplitUpdate),NULL,this);
				m_pParent->Connect(SPLIT_H, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnSplitUpdate),NULL,this);

				m_pParent->Connect(m_pHerramienta->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnUpdateMenuLayout),NULL,this);
			}

			~wxEventHandlerWindowLayout()
			{
				m_pParent->Disconnect(ADD_COL,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnAddColumn),NULL,this);
				m_pParent->Disconnect(ADD_FILA,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnAddRow),NULL,this);
				m_pParent->Disconnect(DEL_COL,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnDelColumn),NULL,this);
				m_pParent->Disconnect(DEL_FILA,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnDelRow),NULL,this);

				m_pParent->Disconnect(LAY_1_1,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay1x1),NULL,this);
				m_pParent->Disconnect(LAY_1_2,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay1x2),NULL,this);
				m_pParent->Disconnect(LAY_2_1,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay2x1),NULL,this);
				m_pParent->Disconnect(LAY_2_2,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay2x2),NULL,this);
				m_pParent->Disconnect(LAY_3_3,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay3x3),NULL,this);
				m_pParent->Disconnect(LAY_4_4,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnLay4x4),NULL,this);
				
				m_pParent->Disconnect(SPLIT_V,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnSplitV),NULL,this);
				m_pParent->Disconnect(SPLIT_H,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerWindowLayout::OnSplitH),NULL,this);

				m_pParent->Disconnect(ADD_COL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnAddColumnUpdate),NULL,this);
				m_pParent->Disconnect(ADD_FILA, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnAddRowUpdate),NULL,this);
				m_pParent->Disconnect(DEL_COL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnDelColumnUpdate),NULL,this);
				m_pParent->Disconnect(DEL_FILA, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnDelRowUpdate),NULL,this);

				m_pParent->Disconnect(LAY_1_1, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay1x1Update),NULL,this);
				m_pParent->Disconnect(LAY_1_2, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay1x2Update),NULL,this);
				m_pParent->Disconnect(LAY_2_1, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay2x1Update),NULL,this);
				m_pParent->Disconnect(LAY_2_2, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay2x2Update),NULL,this);
				m_pParent->Disconnect(LAY_3_3, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay3x3Update),NULL,this);
				m_pParent->Disconnect(LAY_4_4, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnLay4x4Update),NULL,this);
				
				m_pParent->Disconnect(SPLIT_V, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnSplitUpdate),NULL,this);
				m_pParent->Disconnect(SPLIT_H, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnSplitUpdate),NULL,this);

				m_pParent->Disconnect(m_pHerramienta->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerWindowLayout::OnUpdateMenuLayout),NULL,this);
			}

			void OnAddRow(wxCommandEvent& )
			{
				m_pHerramienta->AddRow();
			}

			void OnAddColumn(wxCommandEvent& )
			{
				m_pHerramienta->AddColumn();
			}

			void OnDelRow(wxCommandEvent& )
			{
				m_pHerramienta->DelRow();
			}

			void OnDelColumn(wxCommandEvent& )
			{
				m_pHerramienta->DelColumn();
			}
			
			void OnLay1x1(wxCommandEvent& )
			{
				m_pHerramienta->SetWindowLayout(1,1);
			}

			void OnLay1x2(wxCommandEvent& )
			{
				m_pHerramienta->SetWindowLayout(1,2);
			}

			void OnLay2x1(wxCommandEvent& )
			{
				m_pHerramienta->SetWindowLayout(2,1);
			}

			void OnLay2x2(wxCommandEvent& )
			{
				m_pHerramienta->SetWindowLayout(2,2);
			}

			void OnLay3x3(wxCommandEvent& )
			{
				m_pHerramienta->SetWindowLayout(3,3);
			}

			void OnLay4x4(wxCommandEvent& )
			{
				m_pHerramienta->SetWindowLayout(4,4);
			}

			void OnSplitV(wxCommandEvent &)
			{
				m_pHerramienta->SplitActive(true);
			}

			void OnSplitH(wxCommandEvent &)
			{
				m_pHerramienta->SplitActive(false);
			}

			void OnAddColumnUpdate(wxUpdateUIEvent& event) 
			{
				event.Enable(m_pHerramienta->AddAllowed());
			}

			void OnAddRowUpdate(wxUpdateUIEvent& event) 
			{
				
				event.Enable(m_pHerramienta->AddAllowed());
			}

			void OnDelColumnUpdate(wxUpdateUIEvent& event) 
			{
				
				event.Enable(m_pHerramienta->DeleteColumnAllowed());
			}

			void OnDelRowUpdate(wxUpdateUIEvent& event) 
			{
				
				event.Enable(m_pHerramienta->DeleteRowAllowed());
			}

			void OnLay1x1Update(wxUpdateUIEvent& event) 
			{	
				event.Enable(m_pHerramienta->Supports(1,1));
			}

			void OnLay1x2Update(wxUpdateUIEvent& event) 
			{	
				event.Enable(m_pHerramienta->Supports(1,2));
			}

			void OnLay2x1Update(wxUpdateUIEvent& event) 
			{	
				event.Enable(m_pHerramienta->Supports(2,1));
			}

			void OnLay2x2Update(wxUpdateUIEvent& event) 
			{	
				event.Enable(m_pHerramienta->Supports(2,2));
			}

			void OnLay3x3Update(wxUpdateUIEvent& event) 
			{	
				event.Enable(m_pHerramienta->Supports(3,3));
			}

			void OnLay4x4Update(wxUpdateUIEvent& event) 
			{	
				event.Enable(m_pHerramienta->Supports(4,4));
			}

			void OnUpdateMenuLayout(wxUpdateUIEvent& /*event*/) 
			{
			}

			void OnSplitUpdate(wxUpdateUIEvent& event) 
			{
				event.Enable(m_pHerramienta->SupportsSplit());
			}

			GNC::WindowLayoutTool* m_pHerramienta;
			wxEvtHandler* m_pParent;
		};
	}
}


GNC::GCS::ITool* GNC::WindowLayoutTool::NewTool()
{
	return new GNC::WindowLayoutTool();
}

GNC::WindowLayoutTool::WindowLayoutTool()
{
}
GNC::WindowLayoutTool::~WindowLayoutTool()
{
}

bool GNC::WindowLayoutTool::HasDropDownMenu()
{
	return true;
}

void GNC::WindowLayoutTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ADD_COL,_("Add column"), GinkgoResourcesManager::LayoutBar::GetIcoAddColumn(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ADD_FILA,_("Add row"), GinkgoResourcesManager::LayoutBar::GetIcoAddRow(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->AppendSeparator();
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, DEL_COL,_("Remove column"), GinkgoResourcesManager::LayoutBar::GetIcoDelColumn(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, DEL_FILA,_("Remove row"), GinkgoResourcesManager::LayoutBar::GetIcoDelRow(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->AppendSeparator();
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, SPLIT_H,_("Split active horizontally"), GinkgoResourcesManager::LayoutBar::GetIcoLayout2x1(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, SPLIT_V,_("Split active vertically"), GinkgoResourcesManager::LayoutBar::GetIcoLayout1x2(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->AppendSeparator();
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, LAY_1_1,_("1x1 Layout"), GinkgoResourcesManager::LayoutBar::GetIcoLayout1x1(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, LAY_1_2,_("1x2 Layout"), GinkgoResourcesManager::LayoutBar::GetIcoLayout1x2(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, LAY_2_1,_("2x1 Layout"), GinkgoResourcesManager::LayoutBar::GetIcoLayout2x1(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, LAY_2_2,_("2x2 Layout"), GinkgoResourcesManager::LayoutBar::GetIcoLayout2x2(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, LAY_3_3,_("3x3 Layout"), GinkgoResourcesManager::LayoutBar::GetIcoLayout3x3(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, LAY_4_4,_("4x4 Layout"), GinkgoResourcesManager::LayoutBar::GetIcoLayout4x4(), new GNC::GUI::wxEventHandlerWindowLayout(pParent, this)));
}
		
bool GNC::WindowLayoutTool::ExecuteAction()
{	
	return false;
}

void GNC::WindowLayoutTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		wxMenu* pLayoutMenu = new wxMenu();
		AppendDropDownMenu(pParent, pLayoutMenu);
		pMenu->Append(ID,wxString::FromUTF8(Name.c_str()),pLayoutMenu);		
	}
}

void GNC::WindowLayoutTool::SetWindowLayout(int filas, int columnas)
{
	WindowLayoutContract->SetWindowLayout(filas,columnas);
}

void GNC::WindowLayoutTool::AddRow()
{
	WindowLayoutContract->AddRow();
}

void GNC::WindowLayoutTool::AddColumn()
{
	WindowLayoutContract->AddColumn();
}

void GNC::WindowLayoutTool::DelRow()
{
	WindowLayoutContract->DelRow();
}

void GNC::WindowLayoutTool::DelColumn()
{
	WindowLayoutContract->DelColumn();
}

bool GNC::WindowLayoutTool::AddAllowed()
{
	if( WindowLayoutContract->GetRows() * WindowLayoutContract->GetColumns() >= (WindowLayoutContract->m_MaxSlice-WindowLayoutContract->m_MinSlice +1))
	{
		return false;
	}		
	return true;	
}

bool GNC::WindowLayoutTool::Supports(int filas, int columnas)
{
	int zocalos = filas * columnas;
	int slices = WindowLayoutContract->m_MaxSlice - WindowLayoutContract->m_MinSlice +1;
	
	if(zocalos-slices  >= std::min(filas,columnas))
	{
		return false;
	}
	return true;
}

bool GNC::WindowLayoutTool::DeleteRowAllowed()
{
	if( WindowLayoutContract->GetRows() <=1)
	{
		return false;
	}
	return true;
}
bool GNC::WindowLayoutTool::DeleteColumnAllowed()
{
	if( WindowLayoutContract->GetColumns() <=1)
	{
		return false;
	}
	return true;
}

bool GNC::WindowLayoutTool::SupportsSplit()
{
	return WindowLayoutContract->SupportsSplit();
}

void GNC::WindowLayoutTool::SplitActive(bool vertically)
{
	return WindowLayoutContract->SplitActive(vertically);
}


#endif