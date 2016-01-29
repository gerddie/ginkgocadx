/*
 *  
 *  $Id: reconstructiontool.h 6 2012-07-27 06:44:15Z tovar $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-10 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
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
