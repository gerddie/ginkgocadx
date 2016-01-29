#pragma once
/*
*  
*  $Id: inotetool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef INOTETOOL_H
#define INOTETOOL_H
#include <api/toolsystem/iwidgettool.h>
#include "iherramientascore.h"
#include <export/contracts/iwidgetscontractable.h>

namespace GNC {

	namespace GCS {

		class EXTAPI INoteTool : public IWidgetTool, public IWidgetsContractable {
		public:
	
			static GNC::GCS::ITool::UID ID;

			INoteTool();
		};

		
	}
}
#endif