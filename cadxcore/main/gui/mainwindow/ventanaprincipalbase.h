///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __VENTANAPRINCIPALBASE_H__
#define __VENTANAPRINCIPALBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class HeaderPanel;
class TitledPanel;
class TitledPanelSinBorder;

#include <wx/string.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/hyperlink.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class VentanaPrincipalBase
///////////////////////////////////////////////////////////////////////////////
class VentanaPrincipalBase : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* m_pMainSizer;
		wxPanel* m_pPanelActualizacion;
		wxPanel* m_pPanelActualizacionInterno;
		wxHyperlinkCtrl* m_pHipervinculoActualizacion;
		wxBoxSizer* m_pSizerCentral;
		wxPanel* m_pPanelCentral;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnKeydown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnVersionNotifierClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		VentanaPrincipalBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxRESIZE_BORDER|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
		
		~VentanaPrincipalBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoComandosEnEjecucionBase
///////////////////////////////////////////////////////////////////////////////
class DialogoComandosEnEjecucionBase : public wxDialog 
{
	private:
	
	protected:
		BodyPanel* m_panel10;
		TitledPanelSinBorder* m_panel11;
		wxListBox* m_pListacomandos;
		FooterPanel* m_panel9;
		wxButton* m_button1;
		wxButton* m_button2;
	
	public:
		
		DialogoComandosEnEjecucionBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Closing Ginkgo CADx"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~DialogoComandosEnEjecucionBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DetachedDialogBase
///////////////////////////////////////////////////////////////////////////////
class DetachedDialogBase : public wxFrame 
{
	private:
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseDialogo( wxCloseEvent& event ) { event.Skip(); }
		
	
	public:
		
		DetachedDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
		
		~DetachedDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoLoginBase
///////////////////////////////////////////////////////////////////////////////
class DialogoLoginBase : public wxDialog 
{
	private:
	
	protected:
		HeaderPanel* m_pHeader;
		BodyPanel* m_pBody;
		TitledPanel* m_pFormPanel;
		wxStaticText* m_pLabelUser;
		wxTextCtrl* m_pTCUsuario;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_pTCPassword;
		FooterPanel* m_panel6;
		wxButton* m_pBotonCancelar;
		wxButton* m_pButtonLogin;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnLogin( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DialogoLoginBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("User login"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSTAY_ON_TOP ); 
		~DialogoLoginBase();
	
};

#endif //__VENTANAPRINCIPALBASE_H__
