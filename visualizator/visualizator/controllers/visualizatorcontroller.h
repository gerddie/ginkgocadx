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

#include <api/autoptr.h>
#include <api/controllers/imodulecontroller.h>
#include "../vistas/vista2d.h"

namespace GIL{
	class IModeloIntegracion;
}

namespace GNKVisualizator{
	namespace GUI{
		class ContenedorHerramientas;
	}
}
/* Controlador del módulo visor DICOM */
namespace GNKVisualizator {
	class ControladorAtencionPrimaria : public GNC::GCS::IModuleController {
	public:

		/* Constructor */
		ControladorAtencionPrimaria(GNC::GCS::IEntorno* pEntorno);

		/* Destructor */
		~ControladorAtencionPrimaria();

		virtual void RegistrarVistas();
		virtual bool SupportsImportationUID(const std::string &importationUID);

		/* Abre un estudio de manera no interactiva desde un directorio y devuelve su controlador específico */
		virtual GNC::GCS::IVista* OpenView(int modo, GNC::GCS::IHistoryController::SeriesModelList& series, GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hangingProtocol);
		
		// Metodos de inicializacion de la configuracion particular
		virtual bool SoportaImportacion() const;

		void GetPasosImportacion(IWizard* pWizard,std::list<IPasoWizard*> &listaPasos, wxWindow* pParent, std::string &dirTemporal,GNC::GCS::Ptr<GIL::IModeloIntegracion> pModeloIntegracion);

	protected:
	};
}
