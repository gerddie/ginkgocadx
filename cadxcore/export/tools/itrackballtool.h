#pragma once
/*
*  
*  $Id: itoolpan.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef ITRACKBALLTOOL_H
#define ITRACKBALLTOOL_H
#include <api/toolsystem/iwidgettool.h>
#include <export/contracts/iwidgetscontractable.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {

		class EXTAPI ITrackballTool: public IWidgetTool, public IWidgetsContractable 
		{
		public:
			static GNC::GCS::ITool::UID ID;
			ITrackballTool();			
		};
	}
}
#endif