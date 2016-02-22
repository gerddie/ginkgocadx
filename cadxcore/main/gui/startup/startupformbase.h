///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __STARTUPFORMBASE_H__
#define __STARTUPFORMBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/hyperlink.h>
#include <wx/font.h>
#include <wx/stattext.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class StartUpFormBase
		///////////////////////////////////////////////////////////////////////////////
		class StartUpFormBase : public wxPanel 
		{
			private:
			
			protected:
				wxBoxSizer* m_pSizerToolBar;
				wxStaticText* m_staticText2;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnEraseBackground( wxEraseEvent& event ) { event.Skip(); }
				virtual void erasdf( wxHyperlinkEvent& event ) { event.Skip(); }
				
			
			public:
				wxBoxSizer* m_pSizerCenter;
				
				StartUpFormBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 817,417 ), long style = wxCLIP_CHILDREN|wxTAB_TRAVERSAL ); 
				~StartUpFormBase();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__STARTUPFORMBASE_H__
