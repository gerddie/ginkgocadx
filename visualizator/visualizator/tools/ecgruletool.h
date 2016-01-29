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
#ifndef ECGRULETOOL_H
#define ECGRULETOOL_H
#include <visualizator/export/tools/iecgruletool.h>
#include <api/api.h>

namespace GNKVisualizator
{
	namespace Widgets {
		class WECGRuleBuilder;
	}

	class EXTAPI ECGRuleTool : public GNKVisualizator::IECGRuleTool {
	public:
		ECGRuleTool();
		~ECGRuleTool();


//------------------------------------------------------------------------------
//region "ITool/IContract realization"

		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

//endregion

	protected:
		GNKVisualizator::Widgets::WECGRuleBuilder* m_pRuleBuilder;
	};
}
#endif