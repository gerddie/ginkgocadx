/*
 *  
 *  $Id: pasoperfilimportacion.h $
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

#include "pasosimportacionbase.h"

#include <api/iwizard.h>
#include <api/controllers/ipacscontroller.h>
#include <api/ientorno.h>

#include <string>
#include <vector>

namespace GNC {

	namespace GCS {
		class IEntorno;
		class IModuleController;
	}

	namespace GUI {
		class ImportProfileToolBar;

		class PasoPefilImportacion: public PasoPerfilImportacionBase, public IPasoWizard
		{
			public:
				PasoPefilImportacion(wxWindow* pParent,IWizard* pWizard);
				~PasoPefilImportacion();

				GNC::GCS::IModuleController* GetControladorSeleccionado(){
					return m_pControladorModulo;
				};

			protected:
				GNC::GCS::IModuleController* m_pControladorModulo;
				ImportProfileToolBar* m_pToolBar;

			//region "Metodos heredados de IPasoWizard"
				virtual std::string GetTitle();

				virtual std::string GetSubTitle();

				virtual bool Siguiente();

				virtual bool Anterior();

				virtual bool Cancelar();

				virtual bool Validar();
			//endregion
		};
	}
}
