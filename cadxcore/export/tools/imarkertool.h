#pragma once
/*
*  
*  $Id: imarkertool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IMARKERTOOL_H
#define IMARKERTOOL_H
#include "iherramientascore.h"
#include <api/toolsystem/iwidgettool.h>
#include <export/contracts/iwidgetscontractable.h>

namespace GNC {

	namespace GCS {

		class EXTAPI IMarkerTool : public IWidgetTool, public IWidgetsContractable {
		public:
	
			static GNC::GCS::ITool::UID ID;

			IMarkerTool();
		};

		
	}
}
#endif