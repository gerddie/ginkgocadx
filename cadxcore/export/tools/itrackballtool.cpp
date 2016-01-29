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
#include "itrackballtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::ITrackballTool::ID = HCore_Trackball;

GNC::GCS::ITrackballTool::ITrackballTool() : IWidgetTool(ID, TFamiliaVisualizacion,  _Std("Trackball \tCtrl+T"), GinkgoResourcesManager::ToolIcons::GetIcoTrackball())
{
	KeyCode = Name[Name.size() - 1];
}
