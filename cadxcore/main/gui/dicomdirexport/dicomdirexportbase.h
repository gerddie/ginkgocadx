///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dicomdirexportbase__
#define __dicomdirexportbase__

#include <wx/intl.h>

class BodyPanel;
class FooterPanel;
class HeaderPanel;
class SubTitledPanel;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class ExportDicomBase
		///////////////////////////////////////////////////////////////////////////////
		class ExportDicomBase : public wxDialog 
		{
			private:
			
			protected:
				HeaderPanel* m_pHeader;
				BodyPanel* m_pBody;
				SubTitledPanel* m_pAnonymize;
				wxCheckBox* m_pCheckPatientId;
				wxCheckBox* m_pCheckReferringPhysician;
				wxCheckBox* m_pCheckInstitutions;
				wxCheckBox* m_pCheckPatientName;
				wxCheckBox* m_pCheckComments;
				wxCheckBox* m_pCheckPrivateTags;
				FooterPanel* m_pFooter;
				
				wxButton* m_button1;
				wxButton* m_button2;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnExportClick( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				ExportDicomBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export DICOM"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
				~ExportDicomBase();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__dicomdirexportbase__
