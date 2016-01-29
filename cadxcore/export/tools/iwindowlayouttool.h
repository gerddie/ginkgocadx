#pragma once
/*
*  
*  $Id: iwindowlayouttool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IWINDOWLAYOUTTOOL_H
#define IWINDOWLAYOUTTOOL_H
#include <export/contracts/iwindowlayoutcontractable.h>
#include <api/toolsystem/itool.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {
		class EXTAPI IWindowLayoutTool:  public ITool, public IWindowLayoutContractable
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IWindowLayoutTool();
		};
	}
}
#endif