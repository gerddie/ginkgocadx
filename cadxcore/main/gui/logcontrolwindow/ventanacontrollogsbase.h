///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __VENTANACONTROLLOGSBASE_H__
#define __VENTANACONTROLLOGSBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class SubTitledPanel;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class VentanaControlLogsBase
		///////////////////////////////////////////////////////////////////////////////
		class VentanaControlLogsBase : public wxFrame 
		{
			private:
			
			protected:
				wxMenuBar* m_menubar1;
				wxMenu* m_menuRegistros;
				wxMenuItem* m_menuItemGuardar;
				wxMenuItem* m_menuItemLimpiar;
				wxMenuItem* m_menuItemSalir;
				wxPanel* m_panel3;
				wxRichTextCtrl* m_pRegistros;
				wxPanel* m_panelNivel;
				wxStaticText* m_pNivel;
				wxChoice* m_pComboNivel;
				wxButton* m_pBLimpiar;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnActivate( wxActivateEvent& event ) { event.Skip(); }
				virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
				virtual void OnKeyDown( wxKeyEvent& event ) { event.Skip(); }
				virtual void OnLimpiarRegistros( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnNivelChoice( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnLimpiarClick( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				VentanaControlLogsBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Log monitor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 650,300 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
				
				~VentanaControlLogsBase();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class ShowPlainTextDialog
		///////////////////////////////////////////////////////////////////////////////
		class ShowPlainTextDialog : public wxDialog 
		{
			private:
			
			protected:
				SubTitledPanel* m_pPanelLog;
				wxButton* m_button2;
			
			public:
				wxTextCtrl* m_pLogText;
				
				ShowPlainTextDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Log node"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 650,300 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
				~ShowPlainTextDialog();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__VENTANACONTROLLOGSBASE_H__
