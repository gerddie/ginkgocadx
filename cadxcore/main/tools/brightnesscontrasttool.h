/*
 *  
 *  $Id: brightnesscontrasttool.h $
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
#if defined(ENABLE_MARKERTOOL)
#ifndef BRIGHTNESSCONTRASTOOL_H
#define BRIGHTNESSCONTRASTOOL_H
#include <api/api.h>
#include <export/tools/ibrightnesscontrasttool.h>

namespace GNC {

	namespace GCS {
		namespace Widgets {
			class IWidgetBuilder;
		}
	}

	class EXTAPI BrightnessContrastTool : public GNC::GCS::IBrightnessContrastTool {
	public:

		BrightnessContrastTool();
		~BrightnessContrastTool();

//region "ITool/IContract realization"
		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();
//endregion

	protected:
		GNC::GCS::Widgets::IWidgetBuilder*                 m_pBuilder;
	};
}
#endif
#endif