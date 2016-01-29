///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __VENTANACONTROLHL7BASE_H__
#define __VENTANACONTROLHL7BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/dataview.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class VentanaControlHL7Base
		///////////////////////////////////////////////////////////////////////////////
		class VentanaControlHL7Base : public wxFrame 
		{
			private:
			
			protected:
				wxMenuBar* m_menubar1;
				wxMenu* m_menuMensajes;
				wxMenuItem* m_menuItemPurgarMensajes;
				wxMenuItem* m_menuItemSalir;
				wxMenu* m_menu3;
				wxMenuItem* m_menuItemActualizar;
				wxDataViewListCtrl* m_pListViewCtrl;
				wxObjectDataPtr<wxDataViewListStore> m_pListModel;
				wxPanel* m_panelEstado;
				wxStaticText* m_pLabelEnvio;
				wxStaticText* m_pEstado;
				wxButton* m_pBArrancarParar;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnKeyDown( wxKeyEvent& event ) { event.Skip(); }
				virtual void OnKeydown( wxKeyEvent& event ) { event.Skip(); }
				virtual void OnArrancarPararClick( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				VentanaControlHL7Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("HL7 monitor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,250 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
				
				~VentanaControlHL7Base();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class DialogoMostrarMensajeBase
		///////////////////////////////////////////////////////////////////////////////
		class DialogoMostrarMensajeBase : public wxDialog 
		{
			private:
			
			protected:
				wxStaticText* m_staticText1;
				wxTextCtrl* m_pFecha;
				wxStaticText* m_staticText5;
				wxTextCtrl* m_pDestino;
				wxStaticText* m_staticText6;
				wxTextCtrl* m_pEstado;
				wxChoice* m_pChoiceEstado;
				wxStaticText* m_staticText7;
				wxTextCtrl* m_pContenido;
				wxStaticLine* m_staticline1;
				wxButton* m_button1;
				wxButton* m_CancelButton;
			
			public:
				
				DialogoMostrarMensajeBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Message detail"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,400 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
				~DialogoMostrarMensajeBase();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__VENTANACONTROLHL7BASE_H__
