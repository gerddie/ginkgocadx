#pragma once
/*
 *  
 *  $Id: anonymizetool.h $
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
#if defined(ENABLE_ANONYMIZETOOL)
#ifndef ANONYMIZETOOL_H
#define ANONYMIZETOOL_H
#include <export/tools/iherramientascore.h>
#include <api/ihistorytool.h>

namespace GNC {

	class AnonymizeTool : public GNC::GCS::IHistoryTool {
	public:
		static const GNC::GCS::IHistoryTool::UID ID =  HCore_Anonymize;
		AnonymizeTool();
		virtual ~AnonymizeTool();

		virtual void Execute();
		virtual void Execute(const std::list<long>& seriesPk);
	};
}
#endif
#endif