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

		class PanelConfiguracionPermisos: public PanelConfiguracionPermisosBase, public IPasoConfiguracion
		{
			public:
				PanelConfiguracionPermisos(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~PanelConfiguracionPermisos();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion


			protected:
				virtual void OnListaPermisosDClick(wxDataViewEvent& event);
				virtual void OnListaPermisosChangeSelection(wxDataViewEvent& event);

				virtual void OnSetActive(wxCommandEvent &event);

				virtual void OnNuevoClick(wxCommandEvent &event);

				virtual void OnEditarClick(wxCommandEvent &event);

				virtual void OnEliminarClick(wxCommandEvent &event);

				virtual void OnTextoCambiado(wxCommandEvent &event );

				virtual void OnPropiedadCambiada();

				void OnSize(wxSizeEvent &);
				
				wxObjectDataPtr<wxDataViewListStore> m_pModel;
				
				bool m_permisosCambiados;

		};
	}
}
