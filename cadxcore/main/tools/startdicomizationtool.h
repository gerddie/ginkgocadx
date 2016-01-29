#pragma once
/*
 *  
 *  $Id: showhistory.h $
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
#include <export/tools/istartdicomizationtool.h>
#include <api/api.h>

#include <map>


namespace GNC {

	class EXTAPI StartDicomizationTool : public GNC::GCS::IStartDicomizationTool {

	protected:

	public:
		virtual GNC::GCS::ITool* NewTool();
		StartDicomizationTool();
		~StartDicomizationTool();
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		bool IsShown();

	protected:

	};
}