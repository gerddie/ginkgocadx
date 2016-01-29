#pragma once
/*
*  
*  $Id: ipointertool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IPOINTERTOOL_H
#define IPOINTERTOOL_H
#include <api/toolsystem/iwidgettool.h>
#include <export/contracts/iwidgetscontractable.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {

		class EXTAPI IPointerTool: public IWidgetTool, public IWidgetsContractable {
		public:

			static GNC::GCS::ITool::UID ID;
			IPointerTool();
		};
	}
}
#endif