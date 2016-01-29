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
#include <main/controllers/historytoolscontroller.h>
#include <export/tools/iherramientascore.h>

namespace GNC {

	class OpenGinkgoCloudLinkTool : public GNC::GCS::IHistoryTool {
	public:
		static const GNC::GCS::IHistoryTool::UID ID =  HCore_OpenGinkgoCloudLinkHistory;
		OpenGinkgoCloudLinkTool();
		virtual ~OpenGinkgoCloudLinkTool();
		virtual bool Enabled();

		virtual void Execute();
	};
}