///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __OPENDIALOGSBASE_H__
#define __OPENDIALOGSBASE_H__

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
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DialogoDicomDirBase
///////////////////////////////////////////////////////////////////////////////
class DialogoDicomDirBase : public wxDialog 
{
	private:
	
	protected:
		HeaderPanel* m_pHeader;
		BodyPanel* m_pBody;
		wxDataViewCtrl* m_pResultsView;
		FooterPanel* m_pFooter;
		wxButton* m_button4;
		wxButton* m_button13;
		wxButton* m_button5;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCopyClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLinkClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DialogoDicomDirBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Acquire from DICOM Dir"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~DialogoDicomDirBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SelectDriveBase
///////////////////////////////////////////////////////////////////////////////
class SelectDriveBase : public wxDialog 
{
	private:
	
	protected:
		BodyPanel* m_pBody;
		TitledPanel* m_pPanelMessage;
		wxStaticText* m_pLabel;
		wxChoice* m_pUnits;
		FooterPanel* m_pFooter;
		wxButton* m_button4;
		wxButton* m_button5;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnEndClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SelectDriveBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select drive"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~SelectDriveBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CopyOrLinkBase
///////////////////////////////////////////////////////////////////////////////
class CopyOrLinkBase : public wxDialog 
{
	private:
	
	protected:
		BodyPanel* m_pBody;
		TitledPanel* m_pPanelMessage;
		wxStaticText* m_pLabel;
		FooterPanel* m_pFooter;
		wxButton* m_button4;
		wxButton* m_button5;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCopy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLink( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CopyOrLinkBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Copy or link"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~CopyOrLinkBase();
	
};

#endif //__OPENDIALOGSBASE_H__
