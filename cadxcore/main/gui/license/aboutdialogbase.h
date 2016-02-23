///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ABOUTDIALOGBASE_H__
#define __ABOUTDIALOGBASE_H__

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
#include <wx/html/htmlwin.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/richtext/richtextctrl.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialogBase
///////////////////////////////////////////////////////////////////////////////
class AboutDialogBase : public wxDialog 
{
	private:
	
	protected:
		HeaderPanel* m_pHeader;
		BodyPanel* m_pBody;
		TitledPanel* m_pPanelSuperior;
		wxHtmlWindow* m_pCredits;
		FooterPanel* m_pFooter;
		wxButton* m_buttonLicenciasOS;
		wxButton* m_buttonAceptar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAnyKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnTextURL( wxHtmlLinkEvent& event ) { event.Skip(); }
		virtual void OnLicenciasOpenSource( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxStaticText* m_pActualizacionInfo;
		
		AboutDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 660,500 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~AboutDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoLicenciasOpenSourceBase
///////////////////////////////////////////////////////////////////////////////
class DialogoLicenciasOpenSourceBase : public wxDialog 
{
	private:
	
	protected:
		HeaderPanel* m_pHeader;
		BodyPanel* m_pBody;
		wxListBox* m_pListaLicencias;
		wxRichTextCtrl* m_pRichTextTextoLicencia;
		FooterPanel* m_pFooter;
		wxButton* m_buttonAceptar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnListaLicencias( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DialogoLicenciasOpenSourceBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Open Source Licenses"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,476 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DialogoLicenciasOpenSourceBase();
	
};

#endif //__ABOUTDIALOGBASE_H__
