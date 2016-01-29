/*
 *  
 *  $Id: panelconfiguracionestacion.h $
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
#include <api/iconfiguracion.h>
#include "panelesconfiguracionginkgobase.h"

namespace GNC {
	namespace GUI {

		class PanelConfiguracionEstacion : public PanelConfiguracionEstacionBase, public IPasoConfiguracion
		{
			public:
				PanelConfiguracionEstacion(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~PanelConfiguracionEstacion();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();

				//endregion


			protected:
				void OnTextoCambiado(wxCommandEvent &event);
				void OnSize(wxSizeEvent& event);
		};
	}
}
