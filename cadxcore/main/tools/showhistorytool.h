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
#ifndef SHOWHISTORYTOOL_H
#define SHOWHISTORYTOOL_H
#include <export/tools/ishowhistorytool.h>
#include <api/api.h>

#include <map>


namespace GNC {

	class EXTAPI ShowHistoryTool : public GNC::GCS::IShowHistoryTool {

	protected:

	public:
		virtual GNC::GCS::ITool* NewTool();
		ShowHistoryTool();
		~ShowHistoryTool();
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		bool IsShown();

	protected:

	};
}
#endif