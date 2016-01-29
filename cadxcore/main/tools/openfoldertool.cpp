/*
 *  
 *  $Id: openfoldertool.cpp $
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

#include "openfoldertool.h"
#include <api/internationalization/internationalization.h>

#if defined(ENABLE_OPENFOLDERTOOL)

#include <main/gui/open/abrir.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/controllers/controladorpermisos.h>

//----------------------------------------------------------------------

GNC::OpenFolderTool::OpenFolderTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Open, _Std("Open folder"), _Std("Open folder")+ " \tAlt+o", GinkgoResourcesManager::MenuIcons::GetIcoOpenDir()) 
{
}

GNC::OpenFolderTool::~OpenFolderTool()
{
}

bool GNC::OpenFolderTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire");
}

void GNC::OpenFolderTool::Execute()
{
	GNC::GUI::Abrir::AbrirDirectorio(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow());
}



#endif