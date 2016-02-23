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
