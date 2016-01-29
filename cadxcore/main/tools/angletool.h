#pragma once
/*
 *  
 *  $Id: angletool.h $
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
#if defined(ENABLE_ANGLETOOL)
#ifndef ANGLETOOL_H
#define ANGLETOOL_H
#include <api/api.h>
#include <export/tools/iangletool.h>

namespace GNC {

	namespace GCS {
		namespace Widgets {
			class WAnguloBuilder;
		}
	}

	class EXTAPI AngleTool : public GNC::GCS::IAngleTool {
	public:

		typedef enum TipoAngulo {
			TA_Normal,
			TA_Cobb
		} TipoAngulo;

		AngleTool();
		~AngleTool();
//------------------------------------------------------------------------------
//region "ITool/IContract realization"

		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

		virtual bool HasDropDownMenu();
		void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
		void SetTipo(TipoAngulo tipo);
//endregion

	protected:
		GNC::GCS::Widgets::WAnguloBuilder* m_pAnguloBuilder;
		TipoAngulo                         m_TipoAngulo;
	};
}
#endif
#endif