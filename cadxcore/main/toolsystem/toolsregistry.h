#pragma once
/*
 *  
 *  $Id: toolsregistry.h $
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

#ifndef TOOLSREGISTRY_H
#define TOOLSREGISTRY_H
#include <map>

#include <api/ilock.h>
#include <api/observers/iviewsobserver.h>
#include <api/observers/iextensionsobserver.h>

#include <api/toolsystem/itoolsregistry.h>

namespace GNC {

	/** Tools Registry container internal implementation **/
	class ToolsRegistry : public GNC::GCS::IToolsRegistry, public GNC::GCS::IExtensionsObserver {

//------------------------------------------------------------------------------
//region "Singleton pattern"

	public:
		/** Singleton getter **/
		static GNC::GCS::IToolsRegistry* Instance();
		/** Singleton free **/
		static void FreeInstance();

	protected:
		/** Singleton protected constructor **/
		ToolsRegistry();
		/** Singleton protected destructor **/
		~ToolsRegistry();

		static GNC::ToolsRegistry* m_pInstance;

//endregion

//------------------------------------------------------------------------------
//region "IToolsRegistry realization"

	public:

		/** Registers a tool **/
		virtual void RegisterTool(GNC::GCS::ITool* pTool);

		/** UnRegisters a tool given its uid and frees its factory instance **/			
		virtual void UnRegisterTool(GNC::GCS::ITool::UID uid);

		/** Constructs and returns a new abstract instance of a tool given its uid **/
		virtual GNC::GCS::ITool* NewTool(GNC::GCS::ITool::UID uid);

		void AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::ITool::TToolFamily family);

		/** Returns tools of a concrete family **/
		virtual TToolMap GetToolsFromFamily(GNC::GCS::ITool::TToolFamily family);

//endregion

//------------------------------------------------------------------------------
//region "IExtensionsObserver realization"

	public:

		/** The module is loaded **/
		virtual void OnModuleLoaded(GNC::GCS::IModuleController* pControladorModulo);

		/** The module is unloaded **/
		virtual void OnModuleUnloaded(GNC::GCS::IModuleController* pControladorModulo);

//endregion


//------------------------------------------------------------------------------
//region "Attributes"
	protected:

		typedef std::map< GNC::GCS::ITool::UID, GNC::GCS::ITool* > ToolRegistry;

		ToolRegistry Registry;

		GNC::GCS::ILockable RegistryLock;

//endregion

	};
}
#endif