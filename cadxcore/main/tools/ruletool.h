#pragma once
/*
 *  
 *  $Id: ruletool.h $
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
#if defined(ENABLE_RULETOOL)
#ifndef RULETOOL_H
#define RULETOOL_H
#include <export/tools/iruletool.h>
#include <api/api.h>

namespace GNC {

	namespace GCS {
		namespace Widgets {
			class WRuleBuilder;
		}
	}

	class EXTAPI RuleTool : public GNC::GCS::IRuleTool {
	public:
		RuleTool();
		~RuleTool();


//------------------------------------------------------------------------------
//region "ITool/IContract realization"

		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

//endregion

	protected:
		GNC::GCS::Widgets::WRuleBuilder* m_pRuleBuilder;
	};
}
#endif
#endif