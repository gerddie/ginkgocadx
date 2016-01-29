/*
*  
*  $Id: ibrightnesscontrasttool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "ibrightnesscontrasttool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IBrightnessContrastTool::ID = HCore_BrightnessContrast;

GNC::GCS::IBrightnessContrastTool::IBrightnessContrastTool() : IWidgetTool(ID, TFamiliaVisualizacion, _Std("Brightness/Contrast \tCtrl+B"), GinkgoResourcesManager::ToolIcons::GetIcoBrightnessContrast())
{
	KeyCode = Name[Name.size() - 1];
}
