/*
*  
*  $Id: iwindowlayouttool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "iwindowlayouttool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IWindowLayoutTool::ID = HCore_WindowLayout;

GNC::GCS::IWindowLayoutTool::IWindowLayoutTool() : ITool(ID, TMenuVer,  _Std("Window layout"), GinkgoResourcesManager::ToolIcons::GetIcoLayout())
{
}
