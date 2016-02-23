/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <string>
#include <vector>

#include <api/autoptr.h>
#include <api/iconfiguracion.h>
#include "panelesconfiguracionginkgobase.h"
#include <main/controllers/dcmtk/dicomservers.h>

namespace GNC {
	namespace GUI {

		class PanelConfiguracionPACS: public PanelConfiguracionPACSBase, public IPasoConfiguracion
		{
			public:
				PanelConfiguracionPACS(wxWindow* pParent, IDialogoConfiguracion* pDialogo);
				~PanelConfiguracionPACS();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion

				typedef struct TipoConfigLocal{
					bool useTLS;
					bool verifyCredentials;
					std::string privateKey;
					std::string certificate;

					TipoConfigLocal() :
							useTLS(false),
							verifyCredentials(false)
					{
					}
				} TipoConfigLocal;
			protected:
				virtual void OnAdvancedLocal(wxCommandEvent &event);

				virtual void OnListaPACSChoice(wxDataViewEvent &event);
				
				virtual void OnListaPACSDClick(wxDataViewEvent &event);

				virtual void OnNuevoClick(wxCommandEvent &event);

				virtual void OnEditarClick(wxCommandEvent &event);

				virtual void OnEliminarClick(wxCommandEvent &event);

				virtual void OnSetDefaultClick(wxCommandEvent &event);

				virtual void OnTextoCambiado(wxCommandEvent &event );

				void OnSize(wxSizeEvent &);

				wxObjectDataPtr<wxDataViewListStore> m_pModel;
				typedef std::vector<GNC::GCS::Ptr<DicomServer> > TipoListaPACS;				
				TipoListaPACS m_servidores;				
				TipoConfigLocal m_configLocal;

		};
	}
}
