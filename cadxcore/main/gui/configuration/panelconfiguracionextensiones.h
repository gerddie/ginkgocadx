/*
 *  
 *  $Id: panelconfiguracionextensiones.h $
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

		class PanelConfiguracionExtensiones: public PanelConfiguracionExtensionesBase, public IPasoConfiguracion
		{
			public:
				PanelConfiguracionExtensiones(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~PanelConfiguracionExtensiones();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion

				virtual void OnPropiedadCambiada();

			protected:
								
				void OnSize(wxSizeEvent &);

				bool m_hasBeenChanged;
		};
	}
}
