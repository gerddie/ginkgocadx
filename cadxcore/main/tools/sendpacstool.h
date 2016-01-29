#pragma once
/*
 *  
 *  $Id: sendpacstool.h $
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
#if defined(ENABLE_SENDPACSTOOL)
#ifndef SENDPACSTOOL_H
#define SENDPACSTOOL_H
#include <main/controllers/historytoolscontroller.h>
#include <export/tools/iherramientascore.h>

namespace GNC {

	class SendPACSTool : public GNC::GCS::IHistoryTool {
	public:
		static const GNC::GCS::IHistoryTool::UID ID =  HCore_HistoryPACSUpload;
		SendPACSTool();
		virtual ~SendPACSTool();
		virtual bool Enabled();

		virtual void Execute();		
		virtual void Execute(const std::list<long>& seriesPk);
	};
}
#endif
#endif