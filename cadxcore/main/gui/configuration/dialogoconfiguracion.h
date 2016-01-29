/*
 *  
 *  $Id: dialogoconfiguracion.h $
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
#include "panelesconfiguracionginkgobase.h"

#include <api/iconfiguracion.h>
#include <api/controllers/imodulecontroller.h>
#include <api/ientorno.h>

#include <string>
#include <vector>

namespace GNC {
	namespace GUI {

		class DialogoConfiguracion: public IDialogoConfiguracion, public DialogoConfiguracionBase
		{
			public:
				DialogoConfiguracion(wxWindow* pParent);
				~DialogoConfiguracion();

				virtual void OnCambioNodoNavegacion(wxTreeEvent &event);

				void CargarCurrent();

				void OnPropiedadCambiada();

				void OnCancelarClick(wxCommandEvent &event);

				void OnAceptarClick(wxCommandEvent &event);

				void OnAplicarClick(wxCommandEvent &event);

				void OnExportarClick(wxCommandEvent &event);

				void OnImportarClick(wxCommandEvent &event);

				typedef std::map<wxTreeItemIdValue ,IPasoConfiguracion*> MapaPasos;

				MapaPasos m_MapaPasos;

				IPasoConfiguracion* m_currentPaso;

		};
	}
}
