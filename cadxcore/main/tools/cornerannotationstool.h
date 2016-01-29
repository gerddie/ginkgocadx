#pragma once
/*
 *  
 *  $Id: cornerannotationstool.h $
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
#if defined(ENABLE_CORNERANNOTATIONTOOL)
#ifndef CORNERANNOTATIONTOOL_H
#define CORNERANNOTATIONTOOL_H

#include <export/tools/icornerannotationstool.h>
#include <api/api.h>

namespace GNC {

	class EXTAPI CornerAnnotationsTool : public GNC::GCS::ICornerAnnotationsTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		CornerAnnotationsTool();
		~CornerAnnotationsTool();
		
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		bool IsVisible();


	protected:

	};
}
#endif
#endif