/*
*  
*  $Id: IDetachWindowTool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "idetachwindowtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IDetachWindowTool::ID = HCore_DetachWindow;

GNC::GCS::IDetachWindowTool::IDetachWindowTool() : ITool(ID, TMenuVer,  _Std("Detach window"), GinkgoResourcesManager::ToolIcons::GetIcoDesencajar())
{
}
