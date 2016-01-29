/*
*  
*  $Id: iresettool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "iresettool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IResetTool::ID = HCore_Reset;

GNC::GCS::IResetTool::IResetTool() : ITool(ID, TSeriesMenu,  _Std("Reset"), GinkgoResourcesManager::MenuIcons::GetIcoReset(),2)
{
}
