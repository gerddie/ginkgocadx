///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __VISUALIZATORVIEWS_H__
#define __VISUALIZATORVIEWS_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class SubTitledPanel;
class TitledPanelSinBorder;

#undef __DEPRECATED
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <wxVTKRenderWindowInteractor.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/splitter.h>
#include <wx/combobox.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class VistaSimpleBase
///////////////////////////////////////////////////////////////////////////////
class VistaSimpleBase : public wxPanel 
{
	private:
	
	protected:
		wxSlider* m_pSliderTimeInstant;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChar( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnEraseBackground( wxEraseEvent& event ) { event.Skip(); }
		virtual void OnTimeInstantChanged( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		vtkSmartPointer<vtkGinkgoImageViewer> ViewImage2D;
		vtkSmartPointer<wxVTKRenderWindowInteractor> ViewInteractor2D;

		
		VistaSimpleBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCLIP_CHILDREN|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL ); 
		~VistaSimpleBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class VistaComplejaBase
///////////////////////////////////////////////////////////////////////////////
class VistaComplejaBase : public wxPanel 
{
	private:
	
	protected:
		wxBoxSizer* m_pSizerTools;
		wxBoxSizer* m_pSizerView;
		wxGridSizer* m_pSizerSeries;
		wxPanel* m_panel7;
		wxPanel* m_pPanelManipulacion;
		wxBoxSizer* m_pSizerIzquierda;
		wxPanel* m_pPanelScroll;
		wxBoxSizer* m_pSizerScroll;
		wxBoxSizer* m_pSizerDerecho;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnZSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		wxSlider* m_pSliderSlice;
		
		VistaComplejaBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~VistaComplejaBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoSincronizacionBase
///////////////////////////////////////////////////////////////////////////////
class DialogoSincronizacionBase : public wxDialog 
{
	private:
	
	protected:
		BodyPanel* m_pBody;
		TitledPanelSinBorder* m_panel7;
		wxCheckListBox* m_pCheckListVistas;
		FooterPanel* m_pFooter;
		wxButton* m_button2;
		wxButton* m_button3;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAceptarClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancelarClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DialogoSincronizacionBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Synchronize..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,275 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~DialogoSincronizacionBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GWaveformViewBase
///////////////////////////////////////////////////////////////////////////////
class GWaveformViewBase : public wxPanel 
{
	private:
	
	protected:
		wxBoxSizer* m_pSizerTools;
		wxSplitterWindow* m_pSplitterPanel;
		wxPanel* m_pPanelView;
		SubTitledPanel* m_pPanelMetadata;
		wxTextCtrl* m_pTextMeasurements;
		wxTextCtrl* m_pTextDiagnosis;
		wxPanel* m_panel7;
		wxPanel* m_pManipulationPanel;
		wxComboBox* m_pComboGroups;
		wxBoxSizer* m_pSizerRight;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnComboGroup( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		vtkSmartPointer<vtkGinkgoImageViewer> ViewImage2D;
		vtkSmartPointer<wxVTKRenderWindowInteractor> ViewInteractor2D;

		
		GWaveformViewBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~GWaveformViewBase();
		
		void m_pSplitterPanelOnIdle( wxIdleEvent& )
		{
			m_pSplitterPanel->SetSashPosition( 149 );
			m_pSplitterPanel->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GWaveformViewBase::m_pSplitterPanelOnIdle ), NULL, this );
		}
	
};

#endif //__VISUALIZATORVIEWS_H__
