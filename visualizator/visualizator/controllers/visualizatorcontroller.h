/*
 *  
 *  $Id: visualizatorcontroller.h $
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
