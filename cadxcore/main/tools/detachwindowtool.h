#pragma once
/*
 *  
 *  $Id: detachwindowtool.h $
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
#if defined(ENABLE_DETACHWINDOWTOOL)
#ifndef DETACHWINDOWTOOL_H
#define DETACHWINDOWTOOL_H
#include <export/tools/idetachwindowtool.h>
#include <api/api.h>

#include <map>


namespace GNC {

	class EXTAPI DetachWindowTool : public GNC::GCS::IDetachWindowTool {

	protected:

	public:
		virtual GNC::GCS::ITool* NewTool();
		DetachWindowTool();
		~DetachWindowTool();
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		bool IsDetached();
		void doDetach();

	protected:

	};
}
#endif
#endif