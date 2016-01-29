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
#include "ioverlaystool.h"

#include <api/internationalization/internationalization.h>
#include <export/tools/iherramientascore.h>
#include <resources/visualizatorresourcesmanager.h>

GNC::GCS::ITool::UID GNKVisualizator::IOverlaysTool::ID = HGNKVisualizator_Overlays;

GNKVisualizator::IOverlaysTool::IOverlaysTool(): GNC::GCS::ITool(ID, TSeriesMenu, _Std("Overlay layers"), GNKVisualizator::ResourcesManager::ToolIcons::GetIcoCapas(), 1)
{
}
