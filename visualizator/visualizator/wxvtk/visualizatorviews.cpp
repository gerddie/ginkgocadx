///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "visualizatorviews.h"

///////////////////////////////////////////////////////////////////////////

VistaSimpleBase::VistaSimpleBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 64, 64, 64 ) );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	ViewInteractor2D  = new wxVTKRenderWindowInteractor (this, wxID_ANY,
	wxDefaultPosition,
	wxDefaultSize,
	wxWANTS_CHARS | wxFULL_REPAINT_ON_RESIZE,
	wxT(""));
	
	ViewImage2D = vtkSmartPointer<vtkGinkgoImageViewer> ::New();
	
	vtkSmartPointer<vtkRenderer> rend1 = vtkSmartPointer<vtkRenderer>::New();
	
	ViewImage2D->SetRenderWindow ( this->ViewInteractor2D->GetRenderWindow() );
	ViewImage2D->SetRenderer (rend1);
	
	bSizer7->Add( ViewInteractor2D, 1, wxALL|wxEXPAND, 0 );
	
	m_pSliderTimeInstant = new wxSlider( this, wxID_ANY, 0, 0, 1, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_LEFT|wxSL_VERTICAL );
	m_pSliderTimeInstant->SetForegroundColour( wxColour( 64, 64, 64 ) );
	m_pSliderTimeInstant->Hide();
	
	bSizer7->Add( m_pSliderTimeInstant, 0, wxEXPAND, 1 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	bSizer7->Fit( this );
	
	// Connect Events
	ViewInteractor2D->Connect( wxEVT_CHAR, wxKeyEventHandler( VistaSimpleBase::OnChar ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( VistaSimpleBase::OnEraseBackground ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
}

VistaSimpleBase::~VistaSimpleBase()
{
	// Disconnect Events
	ViewInteractor2D->Disconnect( wxEVT_CHAR, wxKeyEventHandler( VistaSimpleBase::OnChar ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( VistaSimpleBase::OnEraseBackground ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	m_pSliderTimeInstant->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( VistaSimpleBase::OnTimeInstantChanged ), NULL, this );
	
}

VistaComplejaBase::VistaComplejaBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 64, 64, 64 ) );
	this->Hide();
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerTools = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer7->Add( m_pSizerTools, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_pSizerView = new wxBoxSizer( wxHORIZONTAL );
	
	m_pSizerSeries = new wxGridSizer( 1, 1, 1, 1 );
	
	
	m_pSizerView->Add( m_pSizerSeries, 1, wxEXPAND, 2 );
	
	
	bSizer7->Add( m_pSizerView, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,1 ), wxTAB_TRAVERSAL );
	m_panel7->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	m_panel7->SetMinSize( wxSize( -1,1 ) );
	m_panel7->SetMaxSize( wxSize( -1,1 ) );
	
	bSizer7->Add( m_panel7, 0, wxEXPAND|wxRIGHT|wxLEFT, 2 );
	
	m_pPanelManipulacion = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelManipulacion->SetBackgroundColour( wxColour( 64, 64, 64 ) );
	m_pPanelManipulacion->Enable( false );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pSizerIzquierda = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( m_pSizerIzquierda, 0, wxEXPAND, 5 );
	
	m_pPanelScroll = new wxPanel( m_pPanelManipulacion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelScroll->SetBackgroundColour( wxColour( 64, 64, 64 ) );
	
	m_pSizerScroll = new wxBoxSizer( wxHORIZONTAL );
	
	m_pSliderSlice = new wxSlider( m_pPanelScroll, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_BOTH );
	m_pSliderSlice->SetForegroundColour( wxColour( 64, 64, 64 ) );
	
	m_pSizerScroll->Add( m_pSliderSlice, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 0 );
	
	
	m_pPanelScroll->SetSizer( m_pSizerScroll );
	m_pPanelScroll->Layout();
	m_pSizerScroll->Fit( m_pPanelScroll );
	bSizer8->Add( m_pPanelScroll, 1, wxEXPAND | wxALL, 0 );
	
	m_pSizerDerecho = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( m_pSizerDerecho, 0, wxEXPAND, 5 );
	
	
	m_pPanelManipulacion->SetSizer( bSizer8 );
	m_pPanelManipulacion->Layout();
	bSizer8->Fit( m_pPanelManipulacion );
	bSizer7->Add( m_pPanelManipulacion, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 2 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	bSizer7->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( VistaComplejaBase::OnPaint ) );
	m_pSliderSlice->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
}

VistaComplejaBase::~VistaComplejaBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( VistaComplejaBase::OnPaint ) );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	m_pSliderSlice->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( VistaComplejaBase::OnZSliderScroll ), NULL, this );
	
}

DialogoSincronizacionBase::DialogoSincronizacionBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 450,275 ), wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxVERTICAL );
	
	m_panel7 = new TitledPanelSinBorder( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel7->SetToolTip( _("Select the view you want to synchronize with the current") );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_pCheckListVistasChoices;
	m_pCheckListVistas = new wxCheckListBox( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pCheckListVistasChoices, wxLB_MULTIPLE );
	bSizer11->Add( m_pCheckListVistas, 1, wxEXPAND, 5 );
	
	
	m_panel7->SetSizer( bSizer11 );
	m_panel7->Layout();
	bSizer11->Fit( m_panel7 );
	bSizer101->Add( m_panel7, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer101 );
	m_pBody->Layout();
	bSizer101->Fit( m_pBody );
	bSizer9->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button2 = new wxButton( m_pFooter, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_button2, 0, wxALIGN_RIGHT|wxRIGHT, 5 );
	
	m_button3 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_button3, 0, wxALIGN_RIGHT|wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( bSizer10 );
	m_pFooter->Layout();
	bSizer10->Fit( m_pFooter );
	bSizer9->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer9 );
	this->Layout();
	
	// Connect Events
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoSincronizacionBase::OnAceptarClick ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoSincronizacionBase::OnCancelarClick ), NULL, this );
}

DialogoSincronizacionBase::~DialogoSincronizacionBase()
{
	// Disconnect Events
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoSincronizacionBase::OnAceptarClick ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoSincronizacionBase::OnCancelarClick ), NULL, this );
	
}

GWaveformViewBase::GWaveformViewBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 64, 64, 64 ) );
	this->Hide();
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerTools = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer7->Add( m_pSizerTools, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_pSplitterPanel = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_pSplitterPanel->SetSashGravity( 0.8 );
	m_pSplitterPanel->Connect( wxEVT_IDLE, wxIdleEventHandler( GWaveformViewBase::m_pSplitterPanelOnIdle ), NULL, this );
	m_pSplitterPanel->SetMinimumPaneSize( 100 );
	
	m_pPanelView = new wxPanel( m_pSplitterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	ViewInteractor2D = new wxVTKRenderWindowInteractor (m_pPanelView, wxID_ANY,
	wxDefaultPosition,
	wxDefaultSize,
	wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE,
	wxT(""));
	
	ViewImage2D = vtkGinkgoImageViewer::New();
	
	vtkRenderer* rend1 = vtkRenderer::New();
	
	ViewImage2D->SetRenderWindow ( this->ViewInteractor2D->GetRenderWindow() );
	ViewImage2D->SetRenderer (rend1);
	rend1->Delete();
	
	bSizer15->Add( ViewInteractor2D, 1, wxEXPAND, 5 );
	
	
	m_pPanelView->SetSizer( bSizer15 );
	m_pPanelView->Layout();
	bSizer15->Fit( m_pPanelView );
	m_pPanelMetadata = new SubTitledPanel( m_pSplitterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelMetadata->Hide();
	m_pPanelMetadata->SetToolTip( _("Metadata") );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pTextMeasurements = new wxTextCtrl( m_pPanelMetadata, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_pTextMeasurements->SetMaxLength( 0 ); 
	bSizer14->Add( m_pTextMeasurements, 1, wxEXPAND|wxRIGHT, 5 );
	
	m_pTextDiagnosis = new wxTextCtrl( m_pPanelMetadata, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_pTextDiagnosis->SetMaxLength( 0 ); 
	bSizer14->Add( m_pTextDiagnosis, 2, wxEXPAND|wxLEFT, 5 );
	
	
	m_pPanelMetadata->SetSizer( bSizer14 );
	m_pPanelMetadata->Layout();
	bSizer14->Fit( m_pPanelMetadata );
	m_pSplitterPanel->SplitHorizontally( m_pPanelView, m_pPanelMetadata, 149 );
	bSizer7->Add( m_pSplitterPanel, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,1 ), wxTAB_TRAVERSAL );
	m_panel7->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	m_panel7->SetMinSize( wxSize( -1,1 ) );
	m_panel7->SetMaxSize( wxSize( -1,1 ) );
	
	bSizer7->Add( m_panel7, 0, wxEXPAND|wxRIGHT|wxLEFT, 2 );
	
	m_pManipulationPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pManipulationPanel->SetBackgroundColour( wxColour( 64, 64, 64 ) );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pComboGroups = new wxComboBox( m_pManipulationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	bSizer8->Add( m_pComboGroups, 0, wxALIGN_CENTER_VERTICAL, 2 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 0 );
	
	m_pSizerRight = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( m_pSizerRight, 0, wxEXPAND, 5 );
	
	
	m_pManipulationPanel->SetSizer( bSizer8 );
	m_pManipulationPanel->Layout();
	bSizer8->Fit( m_pManipulationPanel );
	bSizer7->Add( m_pManipulationPanel, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 2 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	bSizer7->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( GWaveformViewBase::OnPaint ) );
	m_pComboGroups->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GWaveformViewBase::OnComboGroup ), NULL, this );
}

GWaveformViewBase::~GWaveformViewBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( GWaveformViewBase::OnPaint ) );
	m_pComboGroups->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GWaveformViewBase::OnComboGroup ), NULL, this );
	
}
