#pragma once
/*
*  
*  $Id: ipixelsbluringtool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IINTERPOLATETOOL_H
#define IINTERPOLATETOOL_H
#include <export/contracts/ipixelsbluringcontractable.h>
#include <api/toolsystem/itool.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {
		class EXTAPI IPixelsBluringTool:  public ITool, public IPixelsBluringContractable
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IPixelsBluringTool();
		};
	}
}
#endif