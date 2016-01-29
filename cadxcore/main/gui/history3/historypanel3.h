/*
 *  
 *  $Id: historypanel3.h $
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

#include <api/autoptr.h>
#include "historypanel3base.h"

#include <api/observers/ieventsobserver.h>
#include <api/controllers/imodulecontroller.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/controllers/historycontroller.h>

class wxImage;
class wxDataViewCtrl;
namespace GNC {
	class StreamingLoader;
	namespace GCS {
		class HangingProtocol;
	}
	namespace GUI {
		class HistoryToolMenu;
		class HistoryTableModel;
		class ReloadHistoryTimer;
		class StatusBarProgreso;
		class wxPopUpMenuThumbnails;

		class HistoryPanel3: public HistoryPanel3Base, public GNC::GCS::IHistoryPanel, public GNC::GCS::IEventsObserver
		{
			friend class wxPopUpMenuThumbnails;
			public:
				static HistoryPanel3* Instance();
				static void FreeInstance();

			protected:
				HistoryPanel3(wxWindow* pParent);
				~HistoryPanel3();
				static HistoryPanel3* m_pInstance;

			public:
				virtual bool Show(bool show = true);
				virtual void ReloadHistory();
				virtual void ReloadToolBar();	
				virtual void ReloadCombos(bool checkPurgue);
				
				virtual void RefreshSearch(bool force = false);
				virtual void GetSelectedSeriesPk(std::list<long>& listOfPks);
				virtual wxWindow* GetWxWindow() { return this;}

			protected:	
				virtual void OnHistoryPanelIdle( wxIdleEvent& event );
				virtual void doRefresh(bool force = false);
				void RefreshPatientFilter();
				void RefreshModalityFilter();
				void RefreshDateFilter();

				/** Advanced Search button toggled **/
				virtual void OnAdvancedSearchToggled( wxCommandEvent& event );

				virtual void OnClose(wxCloseEvent &event);
				virtual void OnSearchChange( wxCommandEvent& event );
				void OnDateChanged( wxDateEvent& /*event*/ );
				virtual void OnTreeItemActivated(wxDataViewEvent& event);
				virtual void OnTreeItemMenu(wxDataViewEvent& event);
				void ShowPreviewPanel(bool visible);
				virtual void OnTreeSelChanged(wxDataViewEvent& event);
				virtual void OnTreeKeyDown(wxKeyEvent& event);
				virtual void OnThumbnailSelected(wxThumbnailEvent& evt);
				virtual void OnThumbnailDClick(wxThumbnailEvent& evt);
				virtual void OnThumbnailRClick(wxThumbnailEvent& evt);

				virtual void OnOpenMenu(wxCommandEvent& event);
				virtual void OnOpenWithMenu(wxCommandEvent& event);
				virtual void OnDelete(wxCommandEvent& event);
				virtual void OnClearHistory(wxCommandEvent& event);
				virtual void OnSynchronizeSelected(wxCommandEvent& event);
				
				GNC::GCS::IVista* GetViewFromSeries(long pk_series);

		public:
				void OpenSeriesOrStudyWith( long pk_series, bool isSeries);
				void OpenSeriesOrStudy(long pk_series, bool isSeries, bool force = false);
				void OpenSeries(GNC::GCS::HistoryController::SeriesModelList& seriesModel, GNC::GCS::IModuleController* pModulo, int idModo, GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hangingProtocol = NULL);

		public:
				void ClearHistory();
				void RemoveSelected();
				void SelectAll();
				void DeselectAll();
				void SynchronizeSelected();
		protected:
				void LoadPreviewsFromStudy(const GNC::GCS::Ptr<GNC::GCS::IHistoryController::StudyModel>& pModel);
				void LoadPreviewsFromSeries(const GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel>& pModel);

			public:
				virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

		protected:
				typedef std::list<wxCheckBox*>  TModalitiesVector;
				TModalitiesVector m_modalitiesList;
				HistoryToolMenu*	m_pToolBar;

				wxObjectDataPtr<HistoryTableModel> m_pHistoryTableModel;
				wxDataViewCtrl* m_pHistoryTableView;

				ReloadHistoryTimer* m_pTimerReload;
				GNC::GUI::StatusBarProgreso* m_pStatusBar;
				std::string m_currentPreviewedFile;
				//previews...
				GNC::GCS::Ptr<GNC::StreamingLoader> Loader;
				bool m_RefreshOnIdle;

				GNC::GCS::History::StudyPatientFilter m_patientFilter;
				GNC::GCS::History::StudyModalityFilter m_modalityFilter;
				GNC::GCS::History::AcquisitionDateFilter m_dateFilter;
		};
	}
}
