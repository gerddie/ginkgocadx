/*
*  
*  $Id: itoolslider.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "ivideotool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IVideoTool::ID = HCore_Video;

GNC::GCS::IVideoTool::IVideoTool() : ITool(ID, TFamiliaVisualizacion,  _Std("Video \tCtrl+Space"), GinkgoResourcesManager::CineBar::GetIcoPlay())
{
	KeyCode = (int)' ';
}
