///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WXWIZARDGINKGOBASEWX_H__
#define __WXWIZARDGINKGOBASEWX_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class HeaderPanel;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class wxWizardGinkgoBaseWx
///////////////////////////////////////////////////////////////////////////////
class wxWizardGinkgoBaseWx : public wxDialog 
{
	private:
	
	protected:
		HeaderPanel* m_pHeader;
		BodyPanel* m_pBodyPanel;
		wxPanel* m_pPanelPrincipal;
		wxBoxSizer* m_pSizerPrincipal;
		FooterPanel* m_pFooter;
		wxStaticText* m_pBottomLeftLabel;
		wxButton* m_pBAnterior;
		wxButton* m_pBSiguiente;
		wxButton* m_pBCerrar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnAnteriorClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSiguienteClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancelarClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		wxWizardGinkgoBaseWx( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Test title"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 880,600 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~wxWizardGinkgoBaseWx();
	
};

#endif //__WXWIZARDGINKGOBASEWX_H__
