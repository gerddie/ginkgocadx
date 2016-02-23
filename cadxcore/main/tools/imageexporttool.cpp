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

