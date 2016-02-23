#pragma once
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

#ifndef TOOLCONTROLLER_H
#define TOOLCONTROLLER_H
#include <api/api.h>
#include <api/autoptr.h>
#include <api/toolsystem/itoolcontroller.h>

// Forward declarations
namespace GNC {
	namespace GCS {
		class IWidgetTool;
	}
}

namespace GNC {
	namespace GUI
	{
		class wxEventHandlerToolController;
	}
		
	class EXTAPI ToolController : public GNC::GCS::IToolController
	{
	public:
		
		ToolController();

		virtual ~ToolController();

		virtual void InitializeToolController();
		virtual void ReconnectToolController();
		virtual void FinalizeToolController();

		virtual void LoadStatus(const std::string& view, const std::string& modality = "ALL");
		virtual void SaveStatus(const std::string& view, const std::string& modality = "ALL");

		virtual void AddTool(long idTool, GNC::GCS::IContract* pContract);
		virtual void RemoveTool(long idTool);

		virtual GNC::GCS::ITool* GetTool(long idTool);
		virtual void Enable(long idTool, bool enabled);

		virtual wxWindow* CreateNewToolBar(wxWindow* pParent, bool alignLeft = true);
		virtual wxWindow* CreateNewToolBar(wxWindow* pParent, GNC::GCS::ITool::TToolFamily family, bool alignLeft = true);
		virtual wxWindow* CreateNewToolBar(wxWindow* pParent, const TIdList& toolsToInclude, bool alignLeft = true);
		virtual void AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::ITool::TToolFamily family);
		virtual void AppendsToolsInToolBar(wxGinkgoToolBar* pToolBar, GNC::GCS::ITool::TToolFamily family);
		virtual void AppendsToolsInToolBar(wxGinkgoToolBar* pToolBar, const GNC::GCS::IToolController::TIdList& toolsToInclude);

		virtual void OnKeyAccelerator(GNC::GCS::Events::EventoTeclado& evento, wxWindow* pWindow);

		virtual void ActivateTool(long idTool, const GNC::GCS::TriggerButton& buttonmask, wxWindow* pParent);
		virtual void DoActivateTool(long idTool, const GNC::GCS::TriggerButton& buttonmask, wxWindow* pParent);
		virtual void AppendDropDownMenu(long idTool, wxEvtHandler* pParent, wxMenu* pMenu);

		//connect key events to child tools
		virtual void ConnectKeyEvents(wxWindow* pParent);

		virtual GNC::GCS::IWidgetTool* GetLeftTool();
		virtual GNC::GCS::IWidgetTool* GetRightTool();
		virtual GNC::GCS::IWidgetTool* GetMiddleTool();

		typedef std::map<long, GNC::GCS::ITool*> TToolMap;
		TToolMap m_tools;
		GNC::GCS::Ptr<GNC::GUI::wxEventHandlerToolController> EventHandler;
		GNC::GCS::IWidgetTool* m_leftTool;
		GNC::GCS::IWidgetTool* m_rightTool;
		GNC::GCS::IWidgetTool* m_middleTool;



	private:
		std::string GetToolStatusScope(const std::string& view, const std::string& modality);
	};
}
#endif
