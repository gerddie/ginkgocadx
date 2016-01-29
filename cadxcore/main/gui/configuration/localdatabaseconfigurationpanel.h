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
	namespace GUI {

		class LocalDatabaseConfigurationPanel : public LocalDatabaseConfigurationPanelBase, public IPasoConfiguracion
		{
			public:
				LocalDatabaseConfigurationPanel(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~LocalDatabaseConfigurationPanel();

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
				void OnTextoCambiado(wxCommandEvent &event);
				void OnSeleccionarDirectorio(wxCommandEvent &event);
				void OnSize(wxSizeEvent& event);
				virtual void OnCheckOlderStudies( wxCommandEvent& event );
				virtual void OnCheckDicomDirSize(wxCommandEvent& event);
				virtual void OnCheckHDSpace(wxCommandEvent& event);
				virtual void OnSpinCtrl(wxSpinEvent &evt);
				virtual void OnOptionModified(wxCommandEvent& evt);
				virtual void OnResetToDefault(wxCommandEvent& evt);

				std::map<int, std::string> m_mapTimePeriods;
		};
	}
}
