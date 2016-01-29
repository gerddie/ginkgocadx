#pragma once
/*
*  
*  $Id: itoolslider.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef ISLIDERTOOL_H
#define ISLIDERTOOL_H
#include <api/toolsystem/iwidgettool.h>
#include <export/contracts/islidercontractable.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {

		class EXTAPI IToolSlider: public IWidgetTool, public ISliderContractable 
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IToolSlider();
		};
	}
}
#endif