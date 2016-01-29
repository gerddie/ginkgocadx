///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SELECCIONTEXTOBASE_H__
#define __SELECCIONTEXTOBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class TitledPanel;

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class SeleccionTextoBase
///////////////////////////////////////////////////////////////////////////////
class SeleccionTextoBase : public wxDialog 
{
	private:
	
	protected:
		BodyPanel* m_pBody;
		TitledPanel* m_panel4;
		wxTextCtrl* m_pControlTexto;
		FooterPanel* m_pFooter;
		wxButton* m_pBotonAceptar;
		wxButton* m_pBotonCancelar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnActivar( wxActivateEvent& event ) { event.Skip(); }
		virtual void OnDialogoKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnControlTextoKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnControlTextoTextoActualizado( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBotonAceptarClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBotonCancelarClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SeleccionTextoBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,250 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~SeleccionTextoBase();
	
};

#endif //__SELECCIONTEXTOBASE_H__
