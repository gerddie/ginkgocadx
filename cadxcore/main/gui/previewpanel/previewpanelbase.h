///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PREVIEWPANELBASE_H__
#define __PREVIEWPANELBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class HeaderPanelWithButton;
class VerticalHeader;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/scrolwin.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class FirstLevelPanelBase
///////////////////////////////////////////////////////////////////////////////
class FirstLevelPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_pLabel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChar( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnEraseBackground( wxEraseEvent& event ) { event.Skip(); }
		virtual void OnPanelDClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPanelClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnMenuMouse( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnCabeceraClick( wxMouseEvent& event ) { event.Skip(); }
		
	
	public:
		
		FirstLevelPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 100,-1 ), long style = wxNO_BORDER|wxTAB_TRAVERSAL, const wxString& name = wxT("firstlevelpanel") ); 
		~FirstLevelPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class LeafPanelBase
///////////////////////////////////////////////////////////////////////////////
class LeafPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxStaticBitmap* m_pThumbnail;
		wxStaticText* m_pLabel;
		wxStaticText* m_pLabel1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChar( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnEraseBackground( wxEraseEvent& event ) { event.Skip(); }
		virtual void OnLeafDClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeafClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnMenuMouse( wxMouseEvent& event ) { event.Skip(); }
		
	
	public:
		
		LeafPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 100,120 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxT("leafpanel") ); 
		~LeafPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PreviewPanelBase
///////////////////////////////////////////////////////////////////////////////
class PreviewPanelBase : public wxPanel 
{
	private:
	
	protected:
		HeaderPanelWithButton* m_pPreviewTitle;
		wxScrolledWindow* m_pPanelPreviews;
		VerticalHeader* m_pPanelVertical;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChar( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMenuMouse( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnEraseBackground( wxEraseEvent& event ) { event.Skip(); }
		virtual void OnMedicalHistoryClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnShowPreviewsClick( wxMouseEvent& event ) { event.Skip(); }
		
	
	public:
		
		PreviewPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 110,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxT("previewpanel") ); 
		~PreviewPanelBase();
	
};

#endif //__PREVIEWPANELBASE_H__
