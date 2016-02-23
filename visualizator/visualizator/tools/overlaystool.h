/*
 *  Ginkgo CADx Project
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
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
