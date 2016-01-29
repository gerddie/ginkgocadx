/*
 *  
 *  $Id: parametrosprincipales.h $
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

#include <string>
#include <vector>

#include <api/api.h>
#include <api/autoptr.h>
#include <api/iwizard.h>

#include "pasosexportacionbase.h"
#include "datospersistentesexportacion.h"


namespace GNC {
	namespace GCS {
		class IWidgetsContract;
		class IVista;
		class IEntorno;
	}
	namespace GUI {

		class ParametrosPrincipales: public ParametrosPrincipalesBase,public IPasoWizard
		{
			public:

				ParametrosPrincipales(wxWindow* pParent,IWizard* pWizard, GNC::GCS::IWidgetsContract* WidgetsContract);
				~ParametrosPrincipales();

			protected:			
				GNC::GCS::IWidgetsContract* WidgetsContract;

				void OnChoiceFormatoDestino(wxCommandEvent &event);

				void ExportImage(const wxString& path);

			public:
				
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
