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

#ifndef ITOOL_H
#define ITOOL_H

#include <api/api.h>
#include <api/globals.h>
#include <api/ibutton.h>
#include <api/iexception.h>
#include <wx/bitmap.h>
#include <wx/cursor.h>

#include <wx/bitmap.h>

class wxXmlNode;
class wxPanel;
class wxSizer;
class wxMenu;
class wxGinkgoToolBar;
class wxEvtHandler;

namespace GNC {
	namespace GCS {
		class IVista;
		class ITool;
		class IToolController;
		class IModuleController;
	}
}

namespace GNC {

	namespace GCS {
		class EventHandlerITool;

		class EXTAPI ITool {
		public:
			typedef enum {
				TFamiliaGlobal,
				TFamiliaVisualizacion,
				TFamiliaMedicion,
				TFamiliaAnotacion,
				TFamiliaChroma,
				TFamiliaVision,
				TMenuArchivo,
				TMenuEdicion,
				TMenuVer,
				TSeriesMenu,
				TMenuHerramientas,
				TMenuHelp,
				TMenuExport,
				TMenuPACSAcquisition,
				TViewOptions
			} TToolFamily;

			typedef unsigned int UID;

			virtual ~ITool();

			//new tool
			ITool(const ITool::UID& uid, const ITool::TToolFamily& familyUID, const std::string& name, const wxBitmap& icon, const int subFamily = 0xFFFFFFFF, const int& priority = 0);

			TToolFamily GetFamily() {
				return FamilyId;
			}

			int GetSubFamily() {
				return SubFamilyId;
			}

			virtual bool AppendsInGlobalMenu() {
				return false;
			}

			virtual bool AppendsInMenu() {
				return true;
			}

			virtual bool AppendsInToolBar() {
				return true;
			}

			virtual bool HasDropDownMenu() {
				return false;
			}

			virtual void AppendToolIn(wxGinkgoToolBar* pParent);
			virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
			virtual void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);

			virtual bool IsEnabled();
			virtual void Enable(bool enabled);
			/*If a tool is not ready it wont be attached to any menu*/
			virtual bool IsReady();
			
			virtual bool ExecuteAction() = 0;


			/** Registers the tool in the Tool Registry Container **/
			void RegisterTool();

			/** Sets the Owner module of the tool (the module providing the tool). NULL if CADxCore **/
			void SetOwnerModule(GNC::GCS::IModuleController* pOwner);
			/** Gets the Owner module of the tool (the module providing the tool). NULL if CADxCore **/
			GNC::GCS::IModuleController* GetOwnerModule();

			virtual GNC::GCS::ITool* NewTool() = 0;

			virtual void SetToolBar(GNC::GCS::IToolController* pParent);
			virtual GNC::GCS::IToolController* GetToolBar();

			//register key events to this parent
			virtual void ConnectKeyEvents(wxWindow* pParent);

			virtual int GetKeyCode();
			
			ITool::UID ID;
			TToolFamily FamilyId;
			//sirve para meter separadores en los menus dentro de la misma familia
			int	SubFamilyId;
			int Priority;
			std::string Name;
			wxBitmap Icon;
			GNC::GCS::IToolController* m_pParent;
			IModuleController* OwnerModule;
			int KeyCode;
		protected:
			bool Enabled;
			//

		};
	}
}
#endif
