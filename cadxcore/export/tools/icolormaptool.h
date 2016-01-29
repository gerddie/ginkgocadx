/*
 *  
 *  $Id$
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#ifndef ICOLORMAPTOOL_H
#define ICOLORMAPTOOL_H
#include <api/toolsystem/iwidgettool.h>
#include "iherramientascore.h"
#include <export/contracts/iwidgetscontractable.h>

namespace GNC {
	namespace GCS {
		class EXTAPI IColorMapTool:  public GNC::GCS::ITool, public GNC::GCS::IWidgetsContractable
		{
		public:
			static GNC::GCS::ITool::UID ID;
			IColorMapTool();
		};
	}
}
#endif
