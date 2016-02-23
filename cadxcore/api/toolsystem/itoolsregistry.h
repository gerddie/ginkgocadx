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

#ifndef ITOOLSREGISTRY_H
#define ITOOLSREGISTRY_H

#include <string>
#include <list>
#include <map>

#include <api/toolsystem/itool.h>
#include <api/iexception.h>

namespace GNC {
	namespace GCS {

		class EXTAPI ToolsRegistryException : public GNC::GCS::IException {
		public:
			ToolsRegistryException(const std::string& msg) throw();
		};

		/** Tools Registry container interface **/
		class EXTAPI IToolsRegistry {

//------------------------------------------------------------------------------
//region "Singleton pattern"

		public:

			/** Singleton getter **/
			static GNC::GCS::IToolsRegistry* Instance();

			/** Singleton free **/
			static void FreeInstance();

		protected:

			IToolsRegistry();
			~IToolsRegistry();

//endregion

//------------------------------------------------------------------------------
//region "Tool Registry interface"

		public:
			/** Registers a tool **/
			virtual void RegisterTool(GNC::GCS::ITool* pTool) = 0;

			/** UnRegisters a tool given its uid and frees its factory instance **/			
			virtual void UnRegisterTool(GNC::GCS::ITool::UID uid) = 0;

			/** UnRegisters a tool given its uid and frees its factory instance **/
			virtual void UnRegisterAll();

			/** Constructs and returns a new abstract instance of a tool given its uid **/
			virtual GNC::GCS::ITool* NewTool(GNC::GCS::ITool::UID uid) = 0;

			/** appends general tools in menu */
			virtual void AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::ITool::TToolFamily family) = 0;

			/** Constructs and returns a new concrete instance of a tool given its uid **/
			template <typename ToolType> ToolType* NewConcreteTool(GNC::GCS::ITool::UID uid) {
				ToolType* pConcreteTool = NULL;
				GNC::GCS::ITool* pAbstractTool = NewTool(uid);
				if (pAbstractTool != NULL) {
					pConcreteTool = dynamic_cast<ToolType*>(pAbstractTool);
				}
				return pConcreteTool;
			}
			
			typedef std::list<GNC::GCS::ITool*> TToolList;
			typedef std::map<int, TToolList> TToolMap;
			/** Returns tools of a concrete family **/
			virtual TToolMap GetToolsFromFamily(GNC::GCS::ITool::TToolFamily family) = 0;

//endregion

		};
	}
}
#endif
