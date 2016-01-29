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
#include "iecgruletool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNKVisualizator::IECGRuleTool::UID GNKVisualizator::IECGRuleTool::ID = HGNKVisualizator_ECGRule;

GNKVisualizator::IECGRuleTool::IECGRuleTool() : IWidgetTool(ID, TFamiliaMedicion,  _Std("Rule \tCtrl+R"), GinkgoResourcesManager::ToolIcons::GetIcoLongitud())
{
	KeyCode = Name[Name.size() - 1];
}
