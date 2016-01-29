/*
 *  
 *  $Id: showhistorytool.cpp $
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
#include "imageexporttool.h"
#include <api/ivista.h>
#include <export/contracts/iwidgetscontract.h>
#include <api/iwidgetsmanager.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <main/gui/export/wxwizardexportacionginkgo.h>


//----------------------------------------------------------------------
namespace GNC {
	namespace GUI {
		class wxImageExportEventHandler: public wxDefaultEvtHandlerTool
		{
		public:
			wxImageExportEventHandler(wxEvtHandler* pParent, GNC::ImageExportTool* pTool): wxDefaultEvtHandlerTool(pParent, pTool)
			{
				m_pImageExport = pTool;
			}

			~wxImageExportEventHandler()
			{
			}

			virtual void OnUpdateUI(wxUpdateUIEvent &event)
			{
				wxDefaultEvtHandlerTool::OnUpdateUI(event);
				if (event.GetEnabled()) {
					event.Enable(m_pImageExport->IsEnabled());
				}
			}
			GNC::ImageExportTool* m_pImageExport;
		};

	}
}

GNC::GCS::ITool* GNC::ImageExportTool::NewTool()
{
	return new GNC::ImageExportTool();
}

GNC::ImageExportTool::ImageExportTool()
{
}
GNC::ImageExportTool::~ImageExportTool()
{
}
		
bool GNC::ImageExportTool::ExecuteAction()
{	
	GNC::GUI::wxWidzardExportacionGinkgo dlg(WidgetsContract->GetManager()->GetVista()->GetWindow(), WidgetsContract);
	dlg.ShowModal();
	return true;
}

void GNC::ImageExportTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		GNC::GUI::wxImageExportEventHandler* evtHandler = new GNC::GUI::wxImageExportEventHandler(pParent, this);
		pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
	}
}

