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


#include <wx/dialog.h>
#include <main/gui/open/opendialogsbase.h>


namespace GNC {
	namespace GUI {
		////////////////////////////////////////////////////
		////////////DIALOGO DICOMDIR////////////////////////
		////////////////////////////////////////////////////
		
		class DicomDirTableModel;
		class DialogoDicomDir: public DialogoDicomDirBase
		{
		public:
			DialogoDicomDir(wxWindow* pParent, const std::string& pathDicomDir);

			~DialogoDicomDir();
			void OnCopyClick(wxCommandEvent&);
			void OnLinkClick(wxCommandEvent&);
			void OnItemActivation(wxDataViewEvent& event);
			void OnColumnHeaderClick(wxDataViewEvent& event);
			void OnListChar(wxKeyEvent&event);
		protected:
			void Acquire(bool copy);

			wxObjectDataPtr<DicomDirTableModel> m_pDicomDirTableModel;
		};

		class SelectDrive: public SelectDriveBase
		{
		public:
			typedef struct TDrive {
				std::string id;
				std::string label;
				TDrive(const std::string& id, const std::string& label) {
					this->id = id;
					this->label = label;
				}
			} TDrive;
			typedef std::list<TDrive> TListOfDrives;
			TListOfDrives m_drives;

			SelectDrive(wxWindow* pParent, const TListOfDrives& drives);
			~SelectDrive();

			void SetMessage(const wxString& label);
			std::string GetSelectedDrive();
		};

		class CopyOrLink: public CopyOrLinkBase
		{
		public:
			CopyOrLink(wxWindow* pParent);
			~CopyOrLink();

			bool HasSelectedCopy();
		protected:
			void OnCopy(wxCommandEvent &event);
			void OnLink(wxCommandEvent &event);
			bool m_copy;
		};
	}
}
