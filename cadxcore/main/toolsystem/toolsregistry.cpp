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


#include <api/ientorno.h>
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/imodulecontroller.h>
#include <api/internationalization/internationalization.h>
#include <api/toolsystem/itool.h>

#include "toolsregistry.h"

#include <wx/menu.h>

//------------------------------------------------------------------------------
//region "Singleton pattern"

GNC::ToolsRegistry* GNC::ToolsRegistry::m_pInstance = 0;

/** Singleton getter **/
GNC::GCS::IToolsRegistry* GNC::ToolsRegistry::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new GNC::ToolsRegistry();
        }
        return m_pInstance;
}

/** Singleton free **/
void GNC::ToolsRegistry::FreeInstance()
{
        if (m_pInstance != NULL) {
                delete m_pInstance;
                m_pInstance = NULL;
        }
}

GNC::ToolsRegistry::ToolsRegistry()
{
        GNC::GCS::IEntorno::Instance()->RegisterExtensionsObserver(this);
}

GNC::ToolsRegistry::~ToolsRegistry()
{
        GNC::GCS::IEntorno::Instance()->UnRegisterExtensionsObserver(this);

        GNC::GCS::ILocker lock(RegistryLock);

        for(ToolRegistry::iterator it = Registry.begin(); it != Registry.end(); ++it) {
                GNC::GCS::ITool* pTool = (*it).second;
                if (pTool->GetOwnerModule() != NULL) {
                        LOG_WARN("Core", _Std("Module tool still registered when disposing ToolsRegistry"));
                }
                delete pTool;
        }
        Registry.clear();

}

//endregion

//------------------------------------------------------------------------------
//region "IToolsRegistry realization"

/** Registers a tool **/
void GNC::ToolsRegistry::RegisterTool(GNC::GCS::ITool* pTool)
{
        GNC::GCS::ILocker lock(RegistryLock);
        ToolRegistry::iterator it = Registry.find(pTool->ID);
        if (it != Registry.end()) {
                LOG_WARN("Core", _Std("The tool is already registered"));
                delete pTool;
        } else {
                Registry[pTool->ID] = pTool;
        }
}

/** UnRegisters a tool given its uid and frees its factory instance **/
void GNC::ToolsRegistry::UnRegisterTool(GNC::GCS::ITool::UID uid)
{
        GNC::GCS::ILocker lock(RegistryLock);

        ToolRegistry::iterator it = Registry.find(uid);
        if (it != Registry.end()) {
                GNC::GCS::ITool* pTool = (*it).second;
                delete pTool;
                Registry.erase(it);
        }
}

/** Constructs and returns a new abstract instance of a tool given its uid **/
GNC::GCS::ITool* GNC::ToolsRegistry::NewTool(GNC::GCS::ITool::UID uid)
{
        GNC::GCS::ILocker lock(RegistryLock);

        GNC::GCS::ITool* pTool = NULL;
        ToolRegistry::iterator it = Registry.find(uid);
        if (it != Registry.end()) {
                GNC::GCS::ITool* pSrcTool = (*it).second;
                pTool = pSrcTool->NewTool();
                pTool->SetOwnerModule(pSrcTool->GetOwnerModule());
        }
        return pTool;
}

GNC::GCS::IToolsRegistry::TToolMap GNC::ToolsRegistry::GetToolsFromFamily(GNC::GCS::ITool::TToolFamily family)
{
        GNC::GCS::ILocker lock(RegistryLock);

        TToolMap tools;
        for (ToolRegistry::iterator it = Registry.begin(); it != Registry.end(); ++it) {
                GNC::GCS::ITool* pSrcTool = (*it).second;
                if (pSrcTool->FamilyId == family) {
                        if (tools.find(pSrcTool->SubFamilyId) == tools.end()) {
                                tools[pSrcTool->SubFamilyId] = TToolList();
                        }
                        tools[pSrcTool->SubFamilyId].push_back(pSrcTool);
                }
        }
        return tools;
}

void GNC::ToolsRegistry::AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::ITool::TToolFamily family)
{
        std::map<int , std::list<GNC::GCS::ITool*> > mapSubFamilies;
        for (ToolRegistry::iterator it = Registry.begin(); it != Registry.end(); ++it) {
                GNC::GCS::ITool* pTool = (*it).second;
                if (pTool->AppendsInGlobalMenu() && pTool->GetFamily() == family) {
                        if (mapSubFamilies.find(pTool->GetSubFamily()) == mapSubFamilies.end()) {
                                mapSubFamilies[pTool->GetSubFamily()] = std::list<GNC::GCS::ITool*>();
                        }
                        mapSubFamilies[pTool->GetSubFamily()].push_back(pTool);
                }
        }
        for (std::map<int , std::list<GNC::GCS::ITool*> >::iterator itSubFamilies = mapSubFamilies.begin(); itSubFamilies != mapSubFamilies.end(); ++itSubFamilies) {
                if (!pMenu->GetMenuItems().empty() && !pMenu->GetMenuItems().back()->IsSeparator()) {
                        pMenu->AppendSeparator();
                }
                for (std::list<GNC::GCS::ITool*>::iterator itTools = (*itSubFamilies).second.begin(); itTools != (*itSubFamilies).second.end(); ++itTools) {
                        GNC::GCS::ITool*& pTool = (*itTools);
                        pTool->AppendToolIn(pParent, pMenu);
                }
                if (!pMenu->GetMenuItems().empty() && pMenu->GetMenuItems().back()->IsSeparator()) {
                        pMenu->Delete(pMenu->GetMenuItems().back()->GetId());
                }
        }
        if (!pMenu->GetMenuItems().empty() && pMenu->GetMenuItems().back()->IsSeparator()) {
                pMenu->Delete(pMenu->GetMenuItems().back()->GetId());
        }
}

//endregion

//------------------------------------------------------------------------------
//region "IExtensionsObserver realization"

/** The module is loaded **/
void GNC::ToolsRegistry::OnModuleLoaded(GNC::GCS::IModuleController* /*pMC*/)
{
}

/** The module is unloaded **/
void GNC::ToolsRegistry::OnModuleUnloaded(GNC::GCS::IModuleController* pMC)
{
        GNC::GCS::ILocker lock(RegistryLock);

        for (ToolRegistry::iterator it = Registry.begin(); it != Registry.end(); ++it) {
                GNC::GCS::ITool* pTool = (*it).second;
                if (pMC == pTool->GetOwnerModule()) {
                        LOG_WARN("Core", _Std("The module ")  << pMC->GetUID() << (" was unloaded before its tools were unregistered"));
                }
        }
}

//endregion
