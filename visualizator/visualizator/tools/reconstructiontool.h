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
#if defined(ENABLE_RECONSTRUCTIONTOOL)

#include "../exportacion/ireconstructiontool.h"

#include <map>
namespace GNC {
	namespace GCS {
		class IControladorPermisos;
	}
}

namespace GNKVisualizator {

	class ReconstructionTool : public GNKVisualizator::IReconstructionTool {

	protected:

	public:
		ReconstructionTool(GNC::GCS::IControladorPermisos* pControladorPermisos);
		~ReconstructionTool();
		//menu del mapa de color
		virtual bool AppendInMenu(wxWindow* pParent, wxMenu* pMenuParent);

		//region "Realización de la interfaz IHerramienta"

		// Setup de paneles
		virtual void CrearPaneles( wxPanel* panel );
		
		//endregion

		virtual bool SupportsReconstructionMode(int mode);
		virtual void SelectReconstructionMode(int mode);
		virtual void SolicitarActivacion();

		virtual void ConectarContratoFalso(bool activar);
		virtual void SetVistaActiva(GNC::GCS::IVista* pVista);
		virtual void ConectarContratos(bool activar);

	protected:
		GNC::GCS::IControladorPermisos* m_pControladorPermisos;

	};
}
#endif
