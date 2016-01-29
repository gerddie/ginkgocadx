#pragma once
/*
*  
*  $Id: iangletool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IANGLETOOL_H
#define IANGLETOOL_H
#include <api/toolsystem/iwidgettool.h>
#include "iherramientascore.h"
#include <export/contracts/iwidgetscontractable.h>

namespace GNC {

	namespace GCS {

		class EXTAPI IAngleTool : public IWidgetTool, public IWidgetsContractable {
		public:
	
			static GNC::GCS::ITool::UID ID;

			IAngleTool();
		};

		
	}
}
#endif