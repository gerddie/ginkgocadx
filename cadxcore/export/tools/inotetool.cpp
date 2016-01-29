/*
*  
*  $Id: inotetool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "inotetool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::INoteTool::ID = HCore_Nota;

GNC::GCS::INoteTool::INoteTool() : IWidgetTool(ID, TFamiliaAnotacion, _Std("Text Note \tCtrl+N"), GinkgoResourcesManager::ToolIcons::GetIcoAnotacion())
{
	KeyCode = Name[Name.size() - 1];
}
