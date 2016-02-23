///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PASOSIMPORTACIONBASE_H__
#define __PASOSIMPORTACIONBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class TitledPanel;
class TitledPanelSeparatorV;
class TitledPanelSinBorder;
class wxScrolledWindow;

#include "wx/scrolwin.h"
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include "../../../wx/thumbnailctrl/thumbnailctrl.h"
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <main/gui/wxprevisualizaciondicom.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/choice.h>
#include <wx/maskedtextctrl/maskedtextctrl.h>
#include <wx/gbsizer.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class SelectImagesImportationBase
		///////////////////////////////////////////////////////////////////////////////
		class SelectImagesImportationBase : public wxScrolledWindow
		{
			private:
			
			protected:
				TitledPanelSinBorder* m_panelStep0;
				wxBoxSizer* m_pSizerButtons;
				wxThumbnailCtrl* m_pImagenes;
				wxPanel* m_pPreviewPanel;
				wxPrevisualizacionDicom* m_pPreview;
				wxButton* m_pButtonPreview;
				wxButton* m_pButtonRemoveAll;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnPreviewClick( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnRemoveAll( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				SelectImagesImportationBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 645,464 ), long style = wxTAB_TRAVERSAL ); 
				~SelectImagesImportationBase();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class PasoPerfilImportacionBase
		///////////////////////////////////////////////////////////////////////////////
		class PasoPerfilImportacionBase : public wxPanel 
		{
			private:
			
			protected:
			
			public:
				
				PasoPerfilImportacionBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
				~PasoPerfilImportacionBase();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class PanelDatosGeneralesBase
		///////////////////////////////////////////////////////////////////////////////
		class PanelDatosGeneralesBase : public wxScrolledWindow
		{
			private:
			
			protected:
				TitledPanel* m_panelDemographicData;
				wxStaticText* m_staticText311;
				wxTextCtrl* m_pPacienteId;
				wxStaticText* m_staticText17;
				wxStaticText* m_staticText23;
				wxTextCtrl* m_pPacienteNombre;
				wxStaticText* m_staticText41;
				wxDatePickerCtrl* m_pPacienteFechaNacimiento;
				wxStaticText* dummy1;
				wxStaticText* dummy2;
				wxStaticText* m_staticText261;
				wxTextCtrl* m_pPatientSurname;
				wxStaticText* m_staticText13;
				wxChoice* m_pPacienteSexo;
				TitledPanelSeparatorV* m_pSep;
				TitledPanel* m_panelStudyDetails;
				wxStaticText* m_staticText22;
				wxStaticText* m_staticText11;
				wxTextCtrl* m_pPacientePeso;
				wxStaticText* m_staticText18;
				wxStaticText* m_staticText15;
				wxTextCtrl* m_pPacienteAltura;
				wxStaticText* m_staticText19;
				wxStaticText* m_staticText221;
				wxStaticText* m_pLabelAccNumber;
				wxTextCtrl* m_pAccessionNumber;
				wxStaticText* m_staticText2;
				wxDatePickerCtrl* m_pEstudioFecha;
				wxStaticText* m_staticText16;
				 wxMaskedTextCtrl *m_pEstudioTime;
				wxStaticText* m_staticText3;
				wxTextCtrl* m_pEstudioDescripcion;
				wxPanel* m_pSeriesPanel;
				wxStaticText* m_staticText231;
				wxStaticText* m_staticText212;
				wxDatePickerCtrl* m_pSerieFecha;
				wxStaticText* m_staticText1611;
				 wxMaskedTextCtrl *m_pSerieTime;
				wxStaticText* m_staticText312;
				wxTextCtrl* m_pSerieDescripcion;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnDateChanged( wxDateEvent& event ) { event.Skip(); }
				virtual void OnKillFocusEstudioTimeControl( wxFocusEvent& event ) { event.Skip(); }
				virtual void OnKillFocusSerieTimeControl( wxFocusEvent& event ) { event.Skip(); }
				
			
			public:
				
				PanelDatosGeneralesBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
				~PanelDatosGeneralesBase();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__PASOSIMPORTACIONBASE_H__
