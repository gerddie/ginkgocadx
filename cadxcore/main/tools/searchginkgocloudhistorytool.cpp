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

#include "searchginkgocloudhistorytool.h"

#include <api/controllers/icontroladorpermisos.h>
#include <main/gui/ginkgocloud/ginkgocloudsearchdialog.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>


#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::SearchGinkgoCloudHistoryTool::SearchGinkgoCloudHistoryTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Q_R, _Std("Search Ginkgo Cloud"), _Std("Search Ginkgo Cloud"), GinkgoResourcesManager::ToolIcons::GetIcoSearchCloud(),GinkgoResourcesManager::BigIcons::GetIcoBigGinkgoCloudSearch()) 
{
}

GNC::SearchGinkgoCloudHistoryTool::~SearchGinkgoCloudHistoryTool()
{
}

bool GNC::SearchGinkgoCloudHistoryTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && GNC::GCS::IControladorPermisos::Instance()->Get("ginkgocloud","download");
}

void GNC::SearchGinkgoCloudHistoryTool::Execute()
{
	GNC::GUI::GinkgoCloudSearchDialog::Instance()->Show();
}



