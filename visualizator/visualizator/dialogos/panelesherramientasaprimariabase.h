///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __panelesherramientasaprimariabase__
#define __panelesherramientasaprimariabase__

#include <wx/intl.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#ifdef __VISUALC__
#include <wx/link_additions.h>
#endif //__VISUALC__
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/checklst.h>
#include <wx/stattext.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNKVisualizator
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class PanelResultadosBase
		///////////////////////////////////////////////////////////////////////////////
		class PanelResultadosBase : public wxPanel 
		{
			private:
			
			protected:
				wxBoxSizer* m_pMainSizer;
				wxPropertyGrid* m_pTags;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
				
			
			public:
				
				PanelResultadosBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 334,232 ), long style = wxTAB_TRAVERSAL );
				~PanelResultadosBase();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class PanelOverlaysBase
		///////////////////////////////////////////////////////////////////////////////
		class PanelOverlaysBase : public wxDialog 
		{
			private:
			
			protected:
				wxCheckListBox* m_pCheckListOverlays;
				wxStaticText* m_pLabel;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
				virtual void OnKillFocus( wxFocusEvent& event ) { event.Skip(); }
				virtual void OnItemChecked( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				PanelOverlaysBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 366,177 ), long style = 0 );
				~PanelOverlaysBase();
			
		};
		
	} // namespace GUI
} // namespace GNKVisualizator

#endif //__panelesherramientasaprimariabase__
