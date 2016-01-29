/*
*  
*  $Id: ipixelsbluringtool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "ipixelsbluringtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IPixelsBluringTool::ID = HCore_PixelsBluring;

GNC::GCS::IPixelsBluringTool::IPixelsBluringTool() : ITool(ID, TSeriesMenu,  _Std("Pixels blurring"), wxNullBitmap,2)
{
}
