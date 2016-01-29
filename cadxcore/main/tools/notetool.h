/*
 *  
 *  $Id: notetool.h $
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
#if defined(ENABLE_NOTETOOL)
#ifndef NOTETOOL_H
#define NOTETOOL_H
#include <export/tools/inotetool.h>
#include <api/api.h>


namespace GNC {

	namespace GCS {
		namespace Widgets {
			class WNotaBuilder;
		}
	}

	class EXTAPI NoteTool : public GNC::GCS::INoteTool {
	public:
		NoteTool();
		~NoteTool();
//------------------------------------------------------------------------------
//region "ITool/IContract realization"
		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

//endregion

	protected:
		GNC::GCS::Widgets::WNotaBuilder* m_pBuilder;
	};
}
#endif
#endif