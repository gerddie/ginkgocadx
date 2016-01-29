/*
*  
*  $Id: itoolslider.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "itoolslider.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IToolSlider::ID = HCore_Slider;

GNC::GCS::IToolSlider::IToolSlider() : IWidgetTool(ID, TFamiliaVisualizacion,  _Std("Slider \tCtrl+S"), GinkgoResourcesManager::ToolIcons::GetIcoSlider())
{
	KeyCode = Name[Name.size() - 1];
}
