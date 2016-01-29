/*
 *  
 *  $Id$
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */

#include "icolormaptool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>
GNC::GCS::ITool::UID GNC::GCS::IColorMapTool::ID = HCore_ColorMap;

GNC::GCS::IColorMapTool::IColorMapTool(): GNC::GCS::ITool(ID, TSeriesMenu, _Std("Color Map"), GinkgoResourcesManager::ToolIcons::GetIcoCruz(), 1)
{
}
