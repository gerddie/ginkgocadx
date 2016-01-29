/*
*  
*  $Id: imetricgridtool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "imetricgridtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IMetricGridTool::ID = HCore_MetricGrid;

GNC::GCS::IMetricGridTool::IMetricGridTool() : ITool(ID, TSeriesMenu,  _Std("Metric grid"), GinkgoResourcesManager::ToolIcons::GetIcoRejilla(), 1)
{
}
