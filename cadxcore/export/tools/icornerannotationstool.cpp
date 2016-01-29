/*
*  
*  $Id: icornerannotationstool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "icornerannotationstool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::ICornerAnnotationsTool::ID = HCore_CornerAnnotations;

GNC::GCS::ICornerAnnotationsTool::ICornerAnnotationsTool() : ITool(ID, TSeriesMenu,  _Std("Window annotations"), GinkgoResourcesManager::ToolIcons::GetIcoCornerAnnotations(),2)
{
}
