///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __COUNTDOWNMESSAGEBASE_H__
#define __COUNTDOWNMESSAGEBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class HeaderPanel;
class TitledPanel;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class InactivityCountdownMessageBase
		///////////////////////////////////////////////////////////////////////////////
		class InactivityCountdownMessageBase : public wxDialog 
		{
			private:
			
			protected:
				HeaderPanel* m_pHeader;
				BodyPanel* m_pBody;
				TitledPanel* m_pFormPanel;
				wxStaticText* m_pLabelInactivity;
				FooterPanel* m_panel6;
				wxButton* m_pButtonClose;
				wxButton* m_pBotonCancelar;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
				virtual void OnCloseButton( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				InactivityCountdownMessageBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Closing"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
				~InactivityCountdownMessageBase();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class CountdownMessageBase
		///////////////////////////////////////////////////////////////////////////////
		class CountdownMessageBase : public wxDialog 
		{
			private:
			
			protected:
				HeaderPanel* m_pHeader;
				BodyPanel* m_pBody;
				TitledPanel* m_pFormPanel;
				wxStaticText* m_pLabelInactivity;
				FooterPanel* m_panel6;
				wxButton* m_pButtonOk;
				wxButton* m_pBotonCancelar;
			
			public:
				
				CountdownMessageBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
				~CountdownMessageBase();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__COUNTDOWNMESSAGEBASE_H__
