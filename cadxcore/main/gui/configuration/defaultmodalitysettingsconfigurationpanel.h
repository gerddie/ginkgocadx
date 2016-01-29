/*
 *  
 *  $Id: defaultmodalitysettingsconfigurationpanel.h $
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
		class ModalityPanel;
		class DefaultModalitySettingsConfigurationPanel: public DefaultModalitySettingsConfigurationPanelBase, public IPasoConfiguracion
		{
			public:
				DefaultModalitySettingsConfigurationPanel(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~DefaultModalitySettingsConfigurationPanel();

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
				virtual void OnTreeSelChanged(wxTreeEvent &);
				void LoadCurrent();

				void OnSize(wxSizeEvent &);

				typedef std::map<wxTreeItemId, ModalityPanel*> TModalityMap;
				TModalityMap modalityBrowser;
				ModalityPanel* m_currentModality;

		};
	}
}
