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

#include "openginkgocloudlinktool.h"

#include <api/controllers/icontroladorpermisos.h>
#include <main/gui/ginkgocloud/openginkgoclouddialog.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>


#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::OpenGinkgoCloudLinkTool::OpenGinkgoCloudLinkTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Open, _Std("Open Ginkgo Cloud link"), _Std("Open Ginkgo Cloud link"), GinkgoResourcesManager::ToolIcons::GetIcoDownloadCloud()) 
{
}

GNC::OpenGinkgoCloudLinkTool::~OpenGinkgoCloudLinkTool()
{
}

bool GNC::OpenGinkgoCloudLinkTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && GNC::GCS::IControladorPermisos::Instance()->Get("ginkgocloud","download");
}

void GNC::OpenGinkgoCloudLinkTool::Execute()
{
	GNC::GUI::OpenGinkgoCloudDialog* dlg = new GNC::GUI::OpenGinkgoCloudDialog(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow());
	dlg->Show();
	dlg->Raise();
}



