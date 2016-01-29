#pragma once
/*
 *  
 *  $Id: openfiletool.h $
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
#if defined(ENABLE_OPENFILETOOL)
#ifndef OPENFILETOOL_H
#define OPENFILETOOL_H
#include <main/controllers/historytoolscontroller.h>
#include <export/tools/iherramientascore.h>

namespace GNC {

	class OpenFileTool : public GNC::GCS::IHistoryTool {
	public:
		static const GNC::GCS::IHistoryTool::UID ID =  HCore_OpenFile;
		OpenFileTool();
		virtual ~OpenFileTool();
		virtual bool Enabled();

		virtual void Execute();
	};
}
#endif
#endif