/*
 *  
 *  $Id: panelconfiguraciongeneral.h $
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
#include <map>
#include <api/iconfiguracion.h>
#include "panelesconfiguracionginkgobase.h"

namespace GNC {
	namespace GCS {
		class ProxySettings;
	}
	namespace GUI {

		class PanelConfiguracionGeneral : public PanelConfiguracionGeneralBase, public IPasoConfiguracion
		{
			public:
				PanelConfiguracionGeneral(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~PanelConfiguracionGeneral();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion

			protected:
				wxString m_pathDicomDir;
				void OnCheckForUpdates(wxCommandEvent &event);
				void OnTextoCambiado(wxCommandEvent &event);
				void OnIdiomaCambiado(wxCommandEvent &event);
				virtual void OnProxySettings(wxCommandEvent &event);
				virtual void OnCheckEnableProxySettings(wxCommandEvent &event);

				bool m_idiomaCambiado;
				typedef std::map<std::string, int> TMapLanguages;
				TMapLanguages m_mapLanguages;
				GNC::GCS::ProxySettings* pProxySettings;
		};
	}
}
