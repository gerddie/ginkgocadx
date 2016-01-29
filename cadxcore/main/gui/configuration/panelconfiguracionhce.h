/*
 *  
 *  $Id: panelconfiguracionhce.h $
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
#include <api/iconfiguracion.h>
#include "panelesconfiguracionginkgobase.h"
#include <string>
#include <vector>

namespace GNC {
	namespace GUI {

		class PanelConfiguracionHCE : public PanelConfiguracionHCEBase, public IPasoConfiguracion
		{
			public:
				PanelConfiguracionHCE(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~PanelConfiguracionHCE();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion


			protected:
				virtual void OnSize(wxSizeEvent &){
					Layout();
				}
				virtual void OnTextoCambiado(wxCommandEvent &event);

				virtual void OnCambioSistema(wxCommandEvent &event);

				virtual void OnTestClick(wxCommandEvent &event);

				virtual void OnCheckXMLRPC(wxCommandEvent&event);
		};
	}
}
