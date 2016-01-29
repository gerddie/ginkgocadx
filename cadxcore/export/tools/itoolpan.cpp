/*
*  
*  $Id: itoolpan.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "itoolpan.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IToolPan::ID = HCore_Pan;

GNC::GCS::IToolPan::IToolPan() : IWidgetTool(ID, TFamiliaVisualizacion,  _Std("Pan \tCtrl+P"), GinkgoResourcesManager::ToolIcons::GetIcoPan())
{
	KeyCode = Name[Name.size() - 1];
}
