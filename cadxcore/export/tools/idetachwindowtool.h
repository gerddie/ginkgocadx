#pragma once
/*
*  
*  $Id: IDetachWindowTool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IDETACHWINDOWTOOL_H
#define IDETACHWINDOWTOOL_H
#include <export/contracts/iviewcontractable.h>
#include <api/toolsystem/itool.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {
		class EXTAPI IDetachWindowTool:  public ITool, public IViewContractable
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IDetachWindowTool();
		};
	}
}
#endif