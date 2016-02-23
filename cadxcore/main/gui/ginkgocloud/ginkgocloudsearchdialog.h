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
#include <list>

#include <api/autoptr.h>
#include <api/controllers/imodulecontroller.h>
#include <api/imodelointegracion.h>
#include <api/observers/ieventsobserver.h>
#include <commands/cloudcommand.h>
#include "downloadcloudbase.h"
#include <wx/dataview.h>
#include <wx/checkbox.h>

class wxAnimationCtrl;
class wxDataViewListCtrl;
class wxDataViewListStore;

namespace GNC {
	namespace GCS {
		class CloudCommand;
	}

	namespace GUI {
		class GinkgoCloudSearchDialog: public GinkgoCloudSearchDialogBase, public GNC::GCS::IEventsObserver
		{
			public:
				static GinkgoCloudSearchDialog* Instance();
				static void FreeInstance();

				GinkgoCloudSearchDialog(wxWindow* pParent);
				~GinkgoCloudSearchDialog();
				
				virtual bool Show(bool show = true);

			//---------------------------------------------------------------------------
			//region Eventos de interfaz
				virtual void OnKeyDownFormulario( wxKeyEvent& event );
				virtual void OnFechaDesdeDateChanged( wxDateEvent& event );
				virtual void OnFechaHastaDateChanged( wxDateEvent& event );
				virtual void OnBusquedaClick( wxCommandEvent& event );
				virtual void Search();
				virtual void OnCancelClick( wxCommandEvent& event);
				std::string GetModalities();
				virtual void OnCloseClick( wxCommandEvent& event );
				virtual void OnDescargarClick( wxCommandEvent& event);

				virtual void OnDataSelChanged(wxDataViewEvent& evt);
				virtual void OnDataItemActivated(wxDataViewEvent& evt);
				virtual void OnDataItemMenu(wxDataViewEvent& evt);	
			//---------------------------------------------------------------------------
			//region Helpers
			void AddDescarga();
			void ShowStudyLink();
			void ShowAnimation(bool show);
			
			void LimpiarBusquedas();
			void LoadResults(const std::string& bucketId, const GNC::GCS::CloudFindCommandParams::Results& results);
			//endregion

			//---------------------------------------------------------------------------
			
			//para enterarse si ha parado un comando...
			virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);
			//


			private:
				static GinkgoCloudSearchDialog* m_pInstance;
				wxDataViewListCtrl* m_pResultsView;
				wxObjectDataPtr<wxDataViewListStore> m_pResultsModel;
				
				wxAnimationCtrl* m_pAnimation;

				GNC::GCS::CloudCommand*           m_pCommand;
				bool m_reverseOrder;
				
				
				GNC::GCS::ILockable m_TreeListResultadosLocker; // Necesario para evitar problemas con el treelistcontrol.

				typedef std::list<wxCheckBox*>  TModalitiesVector;
				TModalitiesVector m_modalitiesList;

		};
	}
}
