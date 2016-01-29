#pragma once
/*
 *  
 *  $Id: queryretrievetool.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#include "enabled_tools.h"
#if defined(ENABLE_QUERYRETRIEVETOOL)
#ifndef QUERYRETRIEVETOOL_H
#define QUERYRETRIEVETOOL_H
#include <main/controllers/historytoolscontroller.h>
#include <export/tools/iherramientascore.h>

namespace GNC {

	class QueryRetrieveHistoryTool : public GNC::GCS::IHistoryTool {
	public:
		static const GNC::GCS::IHistoryTool::UID ID =  HCore_QueryRetrieveHistory;
		QueryRetrieveHistoryTool();
		virtual ~QueryRetrieveHistoryTool();
		virtual bool Enabled();

		virtual void Execute();
	};
}
#endif
#endif