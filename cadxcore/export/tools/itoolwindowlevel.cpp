/*
*  
*  $Id: itoolwindowlevel.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "itoolwindowlevel.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IToolWindowLevel::ID = HCore_WindowLevel;

GNC::GCS::IToolWindowLevel::IToolWindowLevel() : IWidgetTool(ID, TFamiliaVisualizacion,  _Std("Window/Level \tCtrl+W"), GinkgoResourcesManager::ToolIcons::GetIcoWindowLevel())
{
	KeyCode = Name[Name.size() - 1];
}
