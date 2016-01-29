/*
 *  
 *  $Id: metricgridtool.h $
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
#if defined(ENABLE_METRICGRIDTOOL)
#ifndef METRICGRIDTOOL_H
#define METRICGRIDTOOL_H
#include <export/tools/imetricgridtool.h>

#include <api/api.h>

#include <map>

namespace GNC {
	class EXTAPI MetricGridTool : public GNC::GCS::IMetricGridTool {

	public:
		virtual GNC::GCS::ITool* NewTool();
		MetricGridTool();
		~MetricGridTool();
		
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		bool IsVisible();
		void ToggleGrid();

		void SetScaleX(double escala);
		void SetScaleY(double escala);

		double GetScaleX();
		double GetScaleY();
	protected:
	};
}
#endif
#endif