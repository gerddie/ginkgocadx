/*
 *  
 *  $Id: dialogoadquisicion.h $
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
#include <list>
#include <wx/dataview.h>
#include <wx/checkbox.h>

#include <api/autoptr.h>
#include <api/controllers/imodulecontroller.h>
#include <api/imodelointegracion.h>
#include <api/observers/ieventsobserver.h>

#include <main/controllers/pacscontroller.h>
#include <commands/comandopacs.h>

#include "dialogoadquisicionbase.h"

class IModeloDicom;
class wxAnimationCtrl;

namespace GNC {

	namespace GUI {
		class AcquisitionTableModel;

		class DialogoAdquisicion: public DialogoAdquisicionBase, public GNC::GCS::IEventsObserver
		{
			public:
				static DialogoAdquisicion* Instance();
				static void FreeInstance();

				DialogoAdquisicion(wxWindow* pParent);
				~DialogoAdquisicion();
				
				virtual bool Show(bool show = true);
				virtual bool Show(const std::string& idPatient, const std::string& idPACS);
				virtual void RefreshComboQueries();

			//---------------------------------------------------------------------------
			//region Eventos de interfaz
				virtual void OnPACSChanged(wxCommandEvent & event);
				virtual void OnKeyDownFormulario( wxKeyEvent& event );
				virtual void OnFechaDesdeDateChanged( wxDateEvent& event );
				virtual void OnFechaHastaDateChanged( wxDateEvent& event );
				virtual void OnBusquedaClick( wxCommandEvent& event );
				GNC::GCS::Ptr<GNC::GCS::StoredQuery> buildQuery();
				virtual void Search();
				virtual void OnCancelClick( wxCommandEvent& event);
				std::string GetModalities();
				virtual void OnLimpiarClick( wxCommandEvent& event );
				virtual void OnCloseClick( wxCommandEvent& event );
				virtual void OnDescargarClick( wxCommandEvent& event);
				virtual void OnLinkClick( wxCommandEvent& event);

				virtual void OnTreeSelChanged(wxDataViewEvent& event);
				virtual void OnTreeItemActivated(wxDataViewEvent& event);
				virtual void OnTreeItemMenu(wxDataViewEvent& event);
				virtual void OnTreeItemExpanded(wxDataViewEvent& event);

				//stored queries
				virtual void OnComboStoredQueriesClick(wxCommandEvent& evt);
				virtual void OnStoreQuery(wxCommandEvent& evt);
				virtual void OnDeleteQuery(wxCommandEvent& evt);
			
			//---------------------------------------------------------------------------
			//region Helpers
			std::string GetServerSeleccionado();
			bool IsLinkSupported();
			void AddDescarga(bool link = false);
			void ShowProperties();
			bool IsAllowedToDownload(const std::string& modality);
			void ShowAnimation(bool show);
			
			//endregion

			//---------------------------------------------------------------------------

			//para enterarse si ha parado un comando...
			virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);
			//

			/** Init download **/
			bool AddDownload(const std::string& server, const std::string& modality, bool seriesMode, const std::string& studyUID, const std::string& seriesUID, bool link);

			private:
				static DialogoAdquisicion* m_pInstance;

				wxString m_SelectedPACS;
				
				wxAnimationCtrl* m_pAnimation;
								
				wxObjectDataPtr<AcquisitionTableModel> m_pAcquisitionTableModel;
				wxDataViewCtrl*							m_pAcquisitionTableView;

				bool        m_SizeRestriction;
				bool        m_SizeOK;
				
				typedef std::list<wxCheckBox*>  TModalitiesVector;
				TModalitiesVector m_modalitiesList;
				DicomServerList::TServerList ListOfServers;
		};
	}
}
