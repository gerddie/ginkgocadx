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
#include <export/contracts/iwidgetscontractable.h>
#include "ivisualizatortools.h"

namespace GNKVisualizator 
{
	class EXTAPI IECGRuleTool: public GNC::GCS::IWidgetTool, public GNC::GCS::IWidgetsContractable {
	public:

		static GNC::GCS::ITool::UID ID;
		IECGRuleTool();
	};

}
#endif