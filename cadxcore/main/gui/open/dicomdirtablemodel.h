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
#include <map>
#include <wx/dynarray.h>
#include <wx/dataview.h>
#include <prvext/prvext.h>
#include <api/ilock.h>
#include <api/autoptr.h>
#include <api/dicom/imodelodicom.h>

class wxImageList;
namespace GNC {
	namespace GUI {		
		class DicomDirTableModel: public wxDataViewModel {
		public:
			enum {
				COL_CHECK=0,
				COL_ICON,
				COL_PATIENT_NAME,
				COL_PATIENT_ID,
				COL_MODALITY,
				COL_DESCRIPTION,
				COL_DATE_TIME,
				COL_ACCNUMBER,
				COL_DICOM_DIR_MAX
			};
			DicomDirTableModel();
			~DicomDirTableModel();

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
			typedef std::list<std::string>TListPaths;
			void GetSelectedPaths(TListPaths& paths);

			IModeloDicom& GetModel();
			void ToggleAllSelections();
			bool LoadModel(const std::string& pathDicomDir);
		protected:
			long GetAge(const std::string& patBirthDate, const std::string& studyDateTime) const;
			
			typedef std::map<const IModeloEstudio*, bool> TStudiesChecked;
			TStudiesChecked checkedStudies;
			IModeloDicom dicomModel;
		};
	}
}
