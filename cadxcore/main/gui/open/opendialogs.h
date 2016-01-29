
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