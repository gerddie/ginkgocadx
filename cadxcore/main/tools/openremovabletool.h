/*
 *  
 *  $Id: openremovabletool.h $
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
#pragma once
#include "enabled_tools.h"
#if defined(ENABLE_OPENREMOVABLETOOL)
#ifndef OPENREMOVABLETOOL_H
#define OPENREMOVABLETOOL_H
#include <main/controllers/historytoolscontroller.h>
#include <export/tools/iherramientascore.h>

namespace GNC {

	class OpenRemovableTool : public GNC::GCS::IHistoryTool {
	public:
		static const GNC::GCS::IHistoryTool::UID ID =  HCore_OpenRemovable;
		OpenRemovableTool();
		virtual ~OpenRemovableTool();
		virtual bool Enabled();

		virtual void Execute();
	};
}
#endif
#endif