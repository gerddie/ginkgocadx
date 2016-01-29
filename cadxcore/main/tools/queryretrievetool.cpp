/*
 *  
 *  $Id: RotateAndFlipTool.cpp $
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

#include "queryretrievetool.h"
#include <export/contracts/iviewcontract.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladorpermisos.h>

#include <main/gui/acquisition/dialogoadquisicion.h>
#include <main/gui/ginkgocloud/ginkgocloudsearchdialog.h>


#define QUERY_PACS  1871
#define QUERY_CLOUD  1872

namespace GNC {
	namespace GUI {
		class wxEventHandlerQRStudy : public wxEvtHandler {
		public:
			wxEventHandlerQRStudy(wxEvtHandler* pParent, GNC::QueryRetrieveTool* pHerramienta):wxEvtHandler() {
				m_pEvtParent = pParent;
				m_pHerramienta=pHerramienta;

				m_pEvtParent->Connect(QUERY_PACS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerQRStudy::OnQueryPacs),NULL,this);
				m_pEvtParent->Connect(QUERY_CLOUD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerQRStudy::OnQueryCloud),NULL,this);
			}

			~wxEventHandlerQRStudy() {
				m_pEvtParent->Disconnect(QUERY_PACS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerQRStudy::OnQueryPacs),NULL,this);
				m_pEvtParent->Disconnect(QUERY_CLOUD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerQRStudy::OnQueryCloud),NULL,this);
				m_pEvtParent = NULL;
				m_pHerramienta = NULL;
			}

			void OnQueryPacs( wxCommandEvent& event )
			{
				m_pHerramienta->QueryRetrievePacs();
				event.Skip(false);
			}

			void OnQueryCloud( wxCommandEvent& event )
			{
				m_pHerramienta->QueryRetrieveCloud();
				event.Skip(false);
			}

			GNC::QueryRetrieveTool* m_pHerramienta;
			wxEvtHandler* m_pEvtParent;
		};
	}
}

GNC::GCS::ITool* GNC::QueryRetrieveTool::NewTool()
{
	return new GNC::QueryRetrieveTool();
}

GNC::QueryRetrieveTool::QueryRetrieveTool()
{
}
GNC::QueryRetrieveTool::~QueryRetrieveTool()
{
}
		
bool GNC::QueryRetrieveTool::ExecuteAction()
{	
	return false;
}

bool GNC::QueryRetrieveTool::HasDropDownMenu()
{
	return true;
}

void GNC::QueryRetrieveTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (pMenu->GetMenuItemCount() > 0 && !pMenu->GetMenuItems().back()->IsSeparator()) {
		pMenu->AppendSeparator();
	}
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, QUERY_PACS, wxString(_("Acquire a DICOM study from PACS")), GinkgoResourcesManager::Acquisition::GetIcoAdquirePACS(), new GNC::GUI::wxEventHandlerQRStudy( pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, QUERY_CLOUD, wxString(_("Search Ginkgo Cloud")), GinkgoResourcesManager::ToolIcons::GetIcoSearchCloud(), new GNC::GUI::wxEventHandlerQRStudy( pParent, this)));

	pMenu->Enable(QUERY_PACS, GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && 
		GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_acquisition"));
	pMenu->Enable(QUERY_CLOUD,  GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && GNC::GCS::IControladorPermisos::Instance()->Get("ginkgocloud","download"));
}

void GNC::QueryRetrieveTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		AppendDropDownMenu(pParent, pMenu);
	}
}

void GNC::QueryRetrieveTool::QueryRetrievePacs()
{
	GNC::GUI::DialogoAdquisicion::Instance()->Show();
}

void GNC::QueryRetrieveTool::QueryRetrieveCloud()
{
	GNC::GUI::GinkgoCloudSearchDialog::Instance()->Show();
}
