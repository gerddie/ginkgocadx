/*
 *  
 *  $Id: openremovabletool.cpp $
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

#include "openremovabletool.h"

#if defined(ENABLE_OPENREMOVABLETOOL)

#include <commands/openremovableunit.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/commandcontroller.h>
#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::OpenRemovableTool::OpenRemovableTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Open, _Std("Open removable unit"),_Std("Open removable unit"), GinkgoResourcesManager::MenuIcons::GetIcoRemovableUnit()) 
{
}

GNC::OpenRemovableTool::~OpenRemovableTool()
{
}

bool GNC::OpenRemovableTool::Enabled() 
{
#ifdef _WIN32
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire");
#else
	return false;
#endif
}

void GNC::OpenRemovableTool::Execute()
{
	GADAPI::OpenRemovableUnitCommandParams* pParams = new GADAPI::OpenRemovableUnitCommandParams(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow());
	GADAPI::OpenRemovableUnitCommand* pCmd = new GADAPI::OpenRemovableUnitCommand(pParams);
	GNC::GCS::ICommandController::Instance()->ProcessAsync("Listing units...", pCmd, NULL);
}

#endif
