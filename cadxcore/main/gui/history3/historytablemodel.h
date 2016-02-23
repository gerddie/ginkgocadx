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

#include <wx/dynarray.h>
#include <wx/dataview.h>
#include <prvext/prvext.h>
#include <api/ilock.h>
#include <api/autoptr.h>
#include <main/controllers/historycontroller.h>


#define COL_ICON			0
#define COL_PATIENT_NAME	1
#define COL_PATIENT_ID		2
#define COL_AGE				3
#define COL_MODALITY		4
#define COL_DESCRIPTION		5
#define COL_DATE_TIME_ACQUIRED		6
#define COL_ACCNUMBER		7
#define COL_DATE_TIME_ADDED		8
#define COL_HISTORY_MAX		9

class wxImageList;
namespace GNC {
	namespace GUI {		
		class HistoryNode: GNC::GCS::ILockable
		{
		public:
			typedef std::list<GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel> > SeriesModelList;
			HistoryNode(const GNC::GCS::IHistoryController::StudyModel& study);
			HistoryNode(const GNC::GCS::IHistoryController::SeriesModel& series, HistoryNode* parent);
			HistoryNode();
			virtual ~HistoryNode();

			bool IsStudyModel() const;
			bool IsSeriesModel() const;

			HistoryNode* GetParent();

			//if creation datetime is < now - 2 h => si se ha recibido antes de dos horas
			bool IsRecent();
			
			void UpdateStudyModel(const GNC::GCS::IHistoryController::StudyModel& studyModel, wxDataViewItemArray& itemsAdded, wxDataViewItemArray& itemsChanged);
			void UpdateSeriesModel(const GNC::GCS::IHistoryController::SeriesModel& seriesModel);
			const GNC::GCS::Ptr<GNC::GCS::IHistoryController::StudyModel>& GetStudyModel();
			const GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel>& GetSeriesModel();

			virtual unsigned int GetChildren(wxDataViewItemArray &array) ;
			virtual void GetSeriesModel(SeriesModelList& seriesList);

			virtual void ReloadSeries();
			virtual void DeleteSeries(long seriesPk);
		protected:			
			GNC::GCS::Ptr<GNC::GCS::IHistoryController::StudyModel> pStudy;
			GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel> pSeries;

			typedef std::map<long, HistoryNode*> TMapIndex;
			std::map<long, HistoryNode*> seriesMap;

			HistoryNode* pParent;			
		};

		class HistoryTableModel: public wxDataViewModel {
		public:
			HistoryTableModel();
			~HistoryTableModel();
			void ClearStudyMap();

			//wxDataViewModel methods
			virtual unsigned int GetColumnCount() const;
			virtual wxString GetColumnType( unsigned int col ) const;
			virtual void GetValue( wxVariant &variant,
                           const wxDataViewItem &item, unsigned int col ) const;
			virtual bool SetValue( const wxVariant &variant,
								   const wxDataViewItem &item, unsigned int col );
			virtual bool IsEnabled( const wxDataViewItem &item, unsigned int col ) const;
			virtual wxDataViewItem GetParent( const wxDataViewItem &item ) const;
			virtual bool IsContainer( const wxDataViewItem &item ) const;
			bool 	HasValue (const wxDataViewItem &item, unsigned col) const;
			virtual bool HasContainerColumns (const wxDataViewItem &item) const ;
			virtual unsigned int GetChildren( const wxDataViewItem &parent,
                                      wxDataViewItemArray &array ) const;
			int Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
                 unsigned int column, bool ascending ) const;
			//

			void ReloadTree(const GNC::GCS::HistoryController::StudyModelList& studyList, bool force);
			void DeleteItems(wxDataViewItemArray& selected);

		protected:
			long GetAge(const std::string& patBirthDate, const std::string& studyDateTime) const;
			typedef std::map<long, HistoryNode*> TMapIndex;
			std::map<long, HistoryNode*> studyMap;
			wxImageList* m_pImageList;
		};
	}
}
