#pragma once
/*
*  
*  $Id: iruletool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IRULETOOL_H
#define IRULETOOL_H
#include <api/toolsystem/iwidgettool.h>
#include <export/contracts/irulecontractable.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {

		class EXTAPI IRuleTool: public IWidgetTool, public IRuleContractable {
		public:

			static GNC::GCS::ITool::UID ID;
			IRuleTool();
		};
	}
}
#endif