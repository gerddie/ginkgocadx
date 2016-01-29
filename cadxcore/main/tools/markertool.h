/*
 *  
 *  $Id: markertool.h $
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
#ifndef MARKERTOOL_H
#define MARKERTOOL_H
#include <api/api.h>
#include <export/tools/imarkertool.h>


namespace GNC {

	namespace GCS {
		namespace Widgets {
			class IWidgetBuilder;
		}
	}

	class EXTAPI MarkerTool : public GNC::GCS::IMarkerTool {
	public:
		typedef enum TipoMarcado {
			TW_Ninguno,
			TW_Flecha,
			TW_Elipse,
			TW_Punto,
			TW_Trapezoide,
			TW_FreeForm,
			TW_Polygon
		} TipoMarcado;

		MarkerTool();
		~MarkerTool();

//region "ITool/IContract realization"
		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

		virtual bool HasDropDownMenu();
		void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
//endregion
		//region "Eventos refinados desde los paneles"

		virtual void SetTipoMarcado(TipoMarcado tipo);

		//endregion

	protected:
		GNC::GCS::Widgets::IWidgetBuilder*                 m_pBuilder;
		TipoMarcado                                        m_MarcadoActivo;
	};
}
#endif
#endif