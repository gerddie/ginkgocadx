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
#include "irotateandfliptool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IRotateAndFlipTool::ID = HCore_RotateAndFlip;

GNC::GCS::IRotateAndFlipTool::IRotateAndFlipTool() : ITool(ID, TSeriesMenu,  _Std("Rotate and flip"), GinkgoResourcesManager::MenuIcons::GetIcoReset())
{
}
