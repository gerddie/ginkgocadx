/*
 *  
 *  $Id: panelconfiguracionpermisos.h $
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

		class SmartRetrieveConfigurationPanel: public SmartRetrieveConfigurationPanelBase, public IPasoConfiguracion
		{
			public:
				SmartRetrieveConfigurationPanel(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~SmartRetrieveConfigurationPanel();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion

			protected:
				virtual void OnItemEdited(wxDataViewEvent& event);
				virtual void OnCheckEnableBackgroundQueries(wxCommandEvent &evt);
				virtual void OnListItemActivated(wxDataViewEvent& event);
				wxObjectDataPtr<wxDataViewListStore> m_pModel;

		};
	}
}
