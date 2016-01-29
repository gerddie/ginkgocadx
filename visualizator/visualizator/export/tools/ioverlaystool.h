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
#pragma once

#ifndef IOVERLAYSTOOL_H
#define IOVERLAYSTOOL_H	
#include <visualizator/export/contracts/ioverlayscontractable.h>
#include <api/toolsystem/itool.h>
#include "ivisualizatortools.h"

namespace GNKVisualizator {
	class IOverlaysTool:  public GNC::GCS::ITool, public GNKVisualizator::IOverlaysContractable
	{
	public:
		static GNC::GCS::ITool::UID ID;
		IOverlaysTool();
	};
}
#endif
