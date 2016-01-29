/*
*  
*  $Id: iruletool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "iruletool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IRuleTool::ID = HCore_Rule;

GNC::GCS::IRuleTool::IRuleTool() : IWidgetTool(ID, TFamiliaMedicion,  _Std("Rule \tCtrl+R"), GinkgoResourcesManager::ToolIcons::GetIcoLongitud())
{
	KeyCode = Name[Name.size() - 1];
}
