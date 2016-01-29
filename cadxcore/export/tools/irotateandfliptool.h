#pragma once
/*
*  
*  $Id: iresettool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IROTATEANDFLIPTOOL_H
#define IROTATEANDFLIPTOOL_H
#include <export/contracts/iwidgetscontractable.h>
#include <api/toolsystem/itool.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {
		class EXTAPI IRotateAndFlipTool:  public ITool, public IWidgetsContractable
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IRotateAndFlipTool();
		};
	}
}
#endif
