#pragma once
/*
*  
*  $Id: ishowtagstool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef ISHOWTAGSTOOL_H
#define ISHOWTAGSTOOL_H
#include <export/contracts/iviewcontractable.h>
#include <api/toolsystem/itool.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {
		class EXTAPI IShowTagsTool:  public ITool, public IViewContractable
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IShowTagsTool();
		};
	}
}
#endif