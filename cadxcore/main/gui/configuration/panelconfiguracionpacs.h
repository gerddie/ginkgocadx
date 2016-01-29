/*
 *  
 *  $Id: panelconfiguracionpacs.h $
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
