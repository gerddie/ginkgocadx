#pragma once
/*
*  
*  $Id: ipacsuploadtool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include <export/contracts/iviewcontractable.h>
#include <api/toolsystem/itool.h>
#include "iherramientascore.h"

namespace GNC {
	namespace GCS {
		class EXTAPI IPACSUploadTool:  public ITool, public IViewContractable
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IPACSUploadTool();
		};
	}
}