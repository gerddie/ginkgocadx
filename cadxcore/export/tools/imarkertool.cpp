/*
*  
*  $Id: imarkertool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "imarkertool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IMarkerTool::ID = HCore_Marcado;

GNC::GCS::IMarkerTool::IMarkerTool() : IWidgetTool(ID, TFamiliaAnotacion, _Std("Marking Tools \tCtrl+M"), GinkgoResourcesManager::MarkingBar::GetIcoPunto())
{
	KeyCode = Name[Name.size() - 1];
}
