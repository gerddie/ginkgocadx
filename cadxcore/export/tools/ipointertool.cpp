/*
*  
*  $Id: ipointertool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "ipointertool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IPointerTool::ID = HCore_Pointer;

GNC::GCS::IPointerTool::IPointerTool() : IWidgetTool(ID, TFamiliaVisualizacion,  _Std("Pointer \tEsc"), GinkgoResourcesManager::ToolIcons::GetIcoFlecha())
{
	KeyCode = WXK_ESCAPE;
}
