#pragma once
/*
 *  
 *  $Id: showtagshistorytool.h $
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
#if defined(ENABLE_SHOWTAGSHISTORYTOOL)
#ifndef SHOWTAGSHISTORYTOOL_H
#define SHOWTAGSHISTORYTOOL_H
#include <main/controllers/historytoolscontroller.h>
#include <export/tools/iherramientascore.h>

namespace GNC {

	class ShowTagsHistoryTool : public GNC::GCS::IHistoryTool {
	public:
		static const GNC::GCS::IHistoryTool::UID ID =  HCore_ShowDicomTagsHistory;
		ShowTagsHistoryTool();
		virtual ~ShowTagsHistoryTool();

		virtual void Execute();
	};
}
#endif
#endif