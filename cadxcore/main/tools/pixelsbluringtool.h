/*
 *  
 *  $Id: pixelsbluringtool.h $
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
#if defined(ENABLE_PIXELSBLURING)
#ifndef INTERPOLATETOOL_H
#define INTERPOLATETOOL_H


#include <export/tools/ipixelsbluringtool.h>
#include <api/api.h>

class HerramientaOpcionesGUIBase;

namespace GNC {
	class EXTAPI PixelsBluringTool : public GNC::GCS::IPixelsBluringTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		PixelsBluringTool();
		~PixelsBluringTool();
		
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		bool IsSet();
		bool IsEnabled();


	protected:

	};
}
#endif
#endif