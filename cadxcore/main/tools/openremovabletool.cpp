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
