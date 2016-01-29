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
#include "icamerarotationtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::ICameraRotationTool::ID = HCore_CameraRotation;

GNC::GCS::ICameraRotationTool::ICameraRotationTool() : IWidgetTool(ID, TFamiliaVisualizacion,  _Std("Rotate camera"), GinkgoResourcesManager::ToolIcons::GetIcoRotateLeft())
{
}
