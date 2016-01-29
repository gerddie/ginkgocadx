/*
*  
*  $Id: iangletool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "iangletool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IAngleTool::ID = HCore_Angulo;

GNC::GCS::IAngleTool::IAngleTool() : IWidgetTool(ID, TFamiliaMedicion, _Std("Angle \tCtrl+A"), GinkgoResourcesManager::AngleBar::GetIcoAnguloNormal())
{
	KeyCode = Name[Name.size() - 1];
}
