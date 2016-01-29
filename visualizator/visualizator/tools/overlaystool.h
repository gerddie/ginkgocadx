/*
 *  
 *  $Id: overlaystool.h 1032 2013-09-10 08:28:42Z carlos $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once

#ifndef OVERLAYSTOOL_H
#define OVERLAYSTOOL_H

#include <list>

#include <visualizator/export/tools/ioverlaystool.h>
#include <visualizator/estudios/overlay.h>
#include <api/api.h>

namespace GNKVisualizator {

	class OverlaysTool : public GNKVisualizator::IOverlaysTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		OverlaysTool();
		~OverlaysTool();

		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		
		//cuando hacen click en ella...
		GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& GetListaOverlays();
		GNKVisualizator::TEstadoOverlay* GetEstadoOverlays();
		virtual void ToggleOverlay(int idOverlay);		

	protected:

	};
}
#endif

#if defined(ENABLE_OVERLAYSNTOOL)

#include "../exportacion/iherramientaoverlays.h"

#include <map>


namespace GNKVisualizator {

	class HerramientaOverlays : public GNKVisualizator::IHerramientaOverlays {

	protected:

	public:
		HerramientaOverlays();
		~HerramientaOverlays();
		//menu del mapa de color
		virtual bool AppendInMenu(wxWindow* pParent, wxMenu* pMenuParent);

		//region "Realización de la interfaz IHerramienta"

		// Setup de paneles
		virtual void CrearPaneles( wxPanel* panel );

		//endregion

		//cuando hacen click en ella...
		GNKVisualizator::TListaOverlays* GetListaOverlays();
		GNKVisualizator::TEstadoOverlay* GetEstadoOverlays();
		virtual void ToggleOverlay(int idOverlay);
		virtual void SolicitarActivacion();
		
		virtual void ConectarContratoFalso(bool activar);
		virtual void SetVistaActiva(GNC::GCS::IVista* pVista);
		virtual void ConectarContratos(bool activar);


	protected:

	};
}
#endif
