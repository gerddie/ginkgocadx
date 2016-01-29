/*
*  
*  $Id: itoolzoom.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "itoolzoom.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IToolZoom::ID = HCore_Zoom;

GNC::GCS::IToolZoom::IToolZoom() : IWidgetTool(ID, TFamiliaVisualizacion,  _Std("Zoom \tCtrl+O"), GinkgoResourcesManager::ToolIcons::GetIcoZoom())
{
	KeyCode = Name[Name.size() - 1];
}
