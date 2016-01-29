/*
 *  
 *  $Id$
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
#include <api/autoptr.h>
#include <wx/wxsqlite3/wxsqlite3.h>

namespace GNC {
	namespace GUI {

		class PanelConfiguracionSeguridad: public PanelConfiguracionSeguridadBase, public IPasoConfiguracion
		{
			public:
				PanelConfiguracionSeguridad(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~PanelConfiguracionSeguridad();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion


			protected:
				virtual void OnSecTypeChoice(wxCommandEvent &event);

				virtual void OnListaUsuariosItemSelected(wxListEvent &event);

				virtual void OnListaUsuariosItemDeSelected(wxListEvent &event);
				
				virtual void OnNuevoClick(wxCommandEvent &event);

				virtual void OnCambiarPasswordClick(wxCommandEvent &event);

				virtual void OnEliminarClick(wxCommandEvent & event);

				virtual void OnSetMainPassword(wxCommandEvent &event);
			
				wxImageList* m_pImageList;
				GNC::GCS::Ptr<wxSQLite3Database> m_pSesionControlador;

		};
	}
}
