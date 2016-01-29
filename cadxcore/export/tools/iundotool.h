#pragma once
/*
*  
*  $Id: iundotool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IUNDOTOOL_H
#define IUNDOTOOL_H
#include <api/toolsystem/itool.h>
#include "iherramientascore.h"
#include <export/contracts/iviewcontractable.h>

namespace GNC {
	namespace GCS {
		class EXTAPI IUndoTool: public ITool, public IViewContractable {
		public:

			static GNC::GCS::ITool::UID ID;
			IUndoTool();

		};
	}
}
#endif