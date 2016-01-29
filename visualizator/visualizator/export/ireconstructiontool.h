/*
 *  
 *  $Id$
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
#include <api/toolsystem/itool.h>
#include "icontratosaprimaria.h"
#include "iherramientasaprimaria.h"
#include <export/iherramientascore.h>

namespace GNKVisualizator {
	class IReconstructionTool: public GNC::GCS::ITool, public GNC::GCS::IContratable<GNKVisualizator::IReconstructionContract> {
	public:
		typedef GNKVisualizator::IReconstructionContract              TReconstructionContract;
		typedef GNC::GCS::IContratable<TReconstructionContract> TReconstructionContratable;

		static const IHerramienta::UID ID = HGNKVisualizator_Reconstruction;
		IReconstructionTool() : IHerramienta(HGNKVisualizator_Reconstruction, TMenu3D, "Reconstruction", SubFamiliaImagenHerramientasRadio) {}
		virtual ~IReconstructionTool() {}

		virtual void SetVistaActiva(GNC::GCS::IVista* pVista) = 0;

		virtual void ConectarContratos(bool) = 0;

		virtual bool ValidaContratos(GNC::GCS::IVista* pVista) {
			return TReconstructionContratable::ValidaContrato(pVista);
		}

	};
}
#endif
