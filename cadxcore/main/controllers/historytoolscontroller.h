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

#pragma once
#include <list>
#include <map>

#include <api/api.h>
#include <api/ihistorytool.h>

class wxGinkgoToolBar;
class wxWindow;
class wxBitmap;
class wxEvtHandler;
class wxMenu;

namespace GNC {
	namespace GCS {

		class EXTAPI IHistoryPanel
		{
		public:
			virtual ~IHistoryPanel(){}

			virtual void ReloadToolBar() = 0;
			virtual void ReloadCombos(bool checkPurgue) = 0;
			virtual void RefreshSearch(bool force = false) = 0;
			virtual void GetSelectedSeriesPk(std::list<long>& listOfPks) = 0;
			virtual wxWindow* GetWxWindow() = 0;
		};
	}

	class EXTAPI HistoryToolsController {

		//------------------------------------------------------------------------------
		//region "Patron singleton"
	public:
		static HistoryToolsController* Instance();
		static void FreeInstance();

	protected:
		HistoryToolsController();
		virtual ~HistoryToolsController();

		static HistoryToolsController* m_pInstance;

	public:
		typedef std::list<GNC::GCS::IHistoryTool*> ToolList;
		typedef std::map<GNC::GCS::IHistoryTool::UID, GNC::GCS::IHistoryTool*> ToolMap;

		virtual void RegisterAll();
		virtual void UnRegisterAll();

		/* Registra una herramienta */
		virtual void RegisterTool(GNC::GCS::IHistoryTool* tool);

		/* Des-Registra una herramienta */
		virtual void UnRegisterTool(GNC::GCS::IHistoryTool* tool);
		virtual void UnRegisterTool(GNC::GCS::IHistoryTool::UID id);

		/*FIXME: Devuelve una copia de las herramientas registradas*/
		ToolList GetTools();

		ToolList GetTools(GNC::GCS::IHistoryTool::TToolFamily family);

		void AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::IHistoryTool::TToolFamily family);
		void AppendsToolInToolBar(wxGinkgoToolBar* pParent, GNC::GCS::IHistoryTool::TToolFamily family);
		void AppendsBigToolsInToolBar(wxGinkgoToolBar* pParent);

		GNC::GCS::IHistoryTool* GetTool(GNC::GCS::IHistoryTool::UID id);

		GNC::GCS::IHistoryPanel* GetHistoryPanel();

		ToolMap m_tools;
	};
}

