/*
 *  
 *  $Id: openfiletool.cpp $
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
//#define _GINKGO_TRACE

#include "openfiletool.h"

#include <api/controllers/icontroladorpermisos.h>
#include <main/gui/open/abrir.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>


#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::OpenFileTool::OpenFileTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Open, _Std("Open File"), _Std("Open File")+ " \tAlt+f", GinkgoResourcesManager::MenuIcons::GetIcoOpenFile()) 
{
}

GNC::OpenFileTool::~OpenFileTool()
{
}

bool GNC::OpenFileTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire");
}

void GNC::OpenFileTool::Execute()
{
	GNC::GUI::Abrir::AbrirFichero(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow());
}



