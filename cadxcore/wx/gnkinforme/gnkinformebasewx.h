///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gnkinformebasewx__
#define __gnkinformebasewx__

#include <wx/intl.h>

class BodyPanel;
class SubTitledPanel;
class SubTitledPanelSeparatorV;
class wxScrolledWindow;

#include "wx/scrolwin.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/button.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class GNKInformeBaseWx
		///////////////////////////////////////////////////////////////////////////////
		class GNKInformeBaseWx : public wxScrolledWindow
		{
			private:
			
			protected:
				wxPanel* m_pPanelReport;
				wxPanel* m_panel21;
				wxStaticText* m_pNombrePaciente;
				wxStaticBitmap* m_pBitmapSexo;
				wxStaticText* m_pIdPaciente;
				BodyPanel* m_pPanelCuerpo;
				SubTitledPanel* m_pPanelDatosDemograficos;
				wxFlexGridSizer* m_pSizerDatosDemograficos;
				SubTitledPanelSeparatorV* m_panel22;
				wxBoxSizer* m_pSizerCustom;
				SubTitledPanel* m_pPanelComentarios;
				wxTextCtrl* m_pComentarios;
				SubTitledPanelSeparatorV* m_panel221;
				SubTitledPanel* m_pPanelConsolidar;
				wxListCtrl* m_pListaConsolidaciones;
				wxPanel* m_pPanelBoton;
				
				wxStaticText* m_pLabelStatus;
				wxButton* m_pBConsolidar;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnComentariosText( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnConsolidarClick( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				GNKInformeBaseWx( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
				~GNKInformeBaseWx();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__gnkinformebasewx__
