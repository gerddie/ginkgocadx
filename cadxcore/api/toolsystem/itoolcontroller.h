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

#ifndef ITOOLBAR_H
#define ITOOLBAR_H
#include <string>
#include <list>

#include <api/api.h>
#include <api/toolsystem/itool.h>
#include <api/ievento.h>

class wxXmlNode;
class wxWindow;
class wxEvtHandler;
class wxMenu;
class wxGinkgoToolBar;

namespace GNC {
	namespace GCS {
		class IContract;
		class IWidgetTool;

		class IToolController {
		public:
			typedef std::list<long> TIdList;

			IToolController();
			virtual ~IToolController();

//region "Tool Bar interface"
			
			virtual void AddTool(long idTool, GNC::GCS::IContract* pContract) = 0;
			virtual void RemoveTool(long idTool) = 0;

			virtual GNC::GCS::ITool* GetTool(long idTool) = 0;

			virtual void Enable(long idTool, bool enabled) = 0;

			virtual void InitializeToolController() = 0;
			virtual void ReconnectToolController() = 0;
			virtual void FinalizeToolController() = 0;

			virtual void LoadStatus(const std::string& view, const std::string& modality = "ALL") = 0;
			virtual void SaveStatus(const std::string& view, const std::string& modality = "ALL") = 0;

			//register tool key events to parent window
			virtual void ConnectKeyEvents(wxWindow* pParent) = 0;

			virtual wxWindow* CreateNewToolBar(wxWindow* pParent, bool alignLeft = true) = 0;
			virtual wxWindow* CreateNewToolBar(wxWindow* pParent, GNC::GCS::ITool::TToolFamily family, bool alignLeft = true) = 0;
			virtual wxWindow* CreateNewToolBar(wxWindow* pParent, const TIdList& toolsToInclude, bool alignLeft = true) = 0;
			virtual void AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::ITool::TToolFamily family) = 0;
			virtual void AppendsToolsInToolBar(wxGinkgoToolBar* pToolBar, GNC::GCS::ITool::TToolFamily family) = 0;
			virtual void AppendsToolsInToolBar(wxGinkgoToolBar* pToolBar, const GNC::GCS::IToolController::TIdList& toolsToInclude) = 0;

			virtual void OnKeyAccelerator(GNC::GCS::Events::EventoTeclado& evento, wxWindow* pWindow) = 0;

			virtual void ActivateTool(long idTool, const TriggerButton& buttonmask, wxWindow* pParent) = 0;
			virtual void AppendDropDownMenu(long idTool, wxEvtHandler* pParent, wxMenu* pMenu) = 0;
			
			virtual GNC::GCS::IWidgetTool* GetLeftTool() = 0;
			virtual GNC::GCS::IWidgetTool* GetRightTool() = 0;
			virtual GNC::GCS::IWidgetTool* GetMiddleTool() = 0;

			static GNC::GCS::IToolController* New();
//endregion

		};
	}
}
#endif
