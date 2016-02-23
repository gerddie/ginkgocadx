///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "historypanel3base.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

HistoryPanel3Base::HistoryPanel3Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
	
	m_pMenuBar = new wxMenuBar( 0 );
	this->SetMenuBar( m_pMenuBar );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerToolBar = new wxBoxSizer( wxVERTICAL );
	
	
	m_pMainSizer->Add( m_pSizerToolBar, 0, wxEXPAND, 5 );
	
	m_pSearchPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* m_pSizerCamposBusqueda;
	m_pSizerCamposBusqueda = new wxFlexGridSizer( 2, 1, 0, 0 );
	m_pSizerCamposBusqueda->AddGrowableCol( 0 );
	m_pSizerCamposBusqueda->AddGrowableRow( 0 );
	m_pSizerCamposBusqueda->AddGrowableRow( 1 );
	m_pSizerCamposBusqueda->SetFlexibleDirection( wxBOTH );
	m_pSizerCamposBusqueda->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* m_pPatientSearchSizer;
	m_pPatientSearchSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pPatientLabelStr = new wxStaticText( m_pSearchPanel, wxID_ANY, _("Patient"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pPatientLabelStr->Wrap( -1 );
	m_pPatientLabelStr->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer33->Add( m_pPatientLabelStr, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPatientCombo = new wxComboBox( m_pSearchPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer33->Add( m_pPatientCombo, 1, wxALL|wxEXPAND, 5 );
	
	m_pAdvancedSearchButton = new wxToggleButton( m_pSearchPanel, wxID_ANY, _("Advanced search"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer33->Add( m_pAdvancedSearchButton, 0, wxALL, 5 );
	
	
	m_pPatientSearchSizer->Add( bSizer33, 1, wxEXPAND, 5 );
	
	
	m_pSizerCamposBusqueda->Add( m_pPatientSearchSizer, 1, wxEXPAND, 2 );
	
	m_pAdvancedSearchPanel = new wxPanel( m_pSearchPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pAdvancedSearchPanel->Hide();
	
	wxBoxSizer* m_pAdvancedSearchSizer;
	m_pAdvancedSearchSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_pAdvancedSearchPanel, wxID_ANY, _("Acquisition date") ), wxVERTICAL );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxVERTICAL );
	
	m_pAnyDate = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Any date"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_pAnyDate->SetValue( true ); 
	bSizer181->Add( m_pAnyDate, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 2 );
	
	m_pTodayAM = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Today AM"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pTodayAM, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pTodayPM = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Today PM"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pTodayPM, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pToday = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Today"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pToday, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	
	bSizer17->Add( bSizer181, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	m_pYesterday = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Yesterday"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pYesterday, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 2 );
	
	m_pLastWeek = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Last 7 days"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pLastWeek, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pLastMonth = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Last 30 days"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pLastMonth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pLastThreeMonths = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Last 3 months"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pLastThreeMonths, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	
	bSizer17->Add( bSizer29, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxVERTICAL );
	
	m_pBetween = new wxRadioButton( m_pAdvancedSearchPanel, wxID_ANY, _("Between:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( m_pBetween, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_pTextControlFechaDesde = new wxDatePickerCtrl( m_pAdvancedSearchPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_DROPDOWN );
	bSizer21->Add( m_pTextControlFechaDesde, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 2 );
	
	
	bSizer201->Add( bSizer21, 0, wxEXPAND|wxLEFT, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_pTextControlFechaHasta = new wxDatePickerCtrl( m_pAdvancedSearchPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_DROPDOWN );
	bSizer22->Add( m_pTextControlFechaHasta, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 2 );
	
	
	bSizer201->Add( bSizer22, 0, wxEXPAND|wxLEFT, 5 );
	
	
	bSizer17->Add( bSizer201, 0, wxEXPAND, 5 );
	
	
	sbSizer2->Add( bSizer17, 1, wxEXPAND|wxALL, 5 );
	
	
	m_pAdvancedSearchSizer->Add( sbSizer2, 1, wxEXPAND|wxRIGHT, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_pAdvancedSearchPanel, wxID_ANY, _("Modalities") ), wxVERTICAL );
	
	m_pModalitySizer = new wxGridSizer( 5, 5, 0, 0 );
	
	
	sbSizer3->Add( m_pModalitySizer, 1, wxEXPAND|wxALL, 5 );
	
	
	m_pAdvancedSearchSizer->Add( sbSizer3, 1, wxEXPAND|wxLEFT, 5 );
	
	
	m_pAdvancedSearchPanel->SetSizer( m_pAdvancedSearchSizer );
	m_pAdvancedSearchPanel->Layout();
	m_pAdvancedSearchSizer->Fit( m_pAdvancedSearchPanel );
	m_pSizerCamposBusqueda->Add( m_pAdvancedSearchPanel, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer20->Add( m_pSizerCamposBusqueda, 1, wxEXPAND, 5 );
	
	
	m_pSearchPanel->SetSizer( bSizer20 );
	m_pSearchPanel->Layout();
	bSizer20->Fit( m_pSearchPanel );
	m_pMainSizer->Add( m_pSearchPanel, 0, wxEXPAND, 5 );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( HistoryPanel3Base::m_splitter1OnIdle ), NULL, this );
	m_splitter1->SetMinimumPaneSize( 20 );
	
	m_pPanelResults = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelResults->SetMinSize( wxSize( -1,100 ) );
	
	m_pTreeListSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_pPanelResults->SetSizer( m_pTreeListSizer );
	m_pPanelResults->Layout();
	m_pTreeListSizer->Fit( m_pPanelResults );
	m_pPanelPreviews = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelPreviews->SetMinSize( wxSize( -1,100 ) );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pThumbnails = new wxThumbnailCtrl(m_pPanelPreviews, -1, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxTH_MULTIPLE_SELECT);
	
	bSizer71->Add( m_pThumbnails, 20, wxEXPAND, 5 );
	
	ViewInteractor2D  = new wxVTKRenderWindowInteractor(m_pPanelPreviews, wxID_ANY,
	wxDefaultPosition,
	wxDefaultSize,
	wxWANTS_CHARS | wxFULL_REPAINT_ON_RESIZE,
	wxT(""));
	
	ViewImage2D = vtkSmartPointer<vtkGinkgoImageViewer> ::New();
	
	vtkSmartPointer<vtkRenderer> rend1 = vtkSmartPointer<vtkRenderer>::New();
	
	ViewImage2D->SetRenderWindow ( this->ViewInteractor2D->GetRenderWindow() );
	ViewImage2D->SetRenderer (rend1);
	
	bSizer71->Add( ViewInteractor2D, 18, wxEXPAND, 5 );
	
	m_pHideInteractorPanel = new wxPanel( m_pPanelPreviews, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHideInteractorPanel->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	m_pHideInteractorPanel->Hide();
	
	bSizer71->Add( m_pHideInteractorPanel, 18, wxEXPAND, 5 );
	
	
	m_pPanelPreviews->SetSizer( bSizer71 );
	m_pPanelPreviews->Layout();
	bSizer71->Fit( m_pPanelPreviews );
	m_splitter1->SplitHorizontally( m_pPanelResults, m_pPanelPreviews, 0 );
	m_pMainSizer->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( HistoryPanel3Base::OnClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( HistoryPanel3Base::OnHistoryPanelIdle ) );
	m_pAdvancedSearchButton->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( HistoryPanel3Base::OnAdvancedSearchToggled ), NULL, this );
	m_pAnyDate->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pAnyDate->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pTodayAM->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pTodayAM->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pTodayPM->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pTodayPM->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pToday->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pToday->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pYesterday->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pYesterday->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pLastWeek->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pLastWeek->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pLastMonth->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pLastMonth->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pLastThreeMonths->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pLastThreeMonths->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pBetween->Connect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pBetween->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pTextControlFechaDesde->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( HistoryPanel3Base::OnDateChanged ), NULL, this );
	m_pTextControlFechaDesde->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaHasta->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( HistoryPanel3Base::OnDateChanged ), NULL, this );
	m_pTextControlFechaHasta->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
}

HistoryPanel3Base::~HistoryPanel3Base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( HistoryPanel3Base::OnClose ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( HistoryPanel3Base::OnHistoryPanelIdle ) );
	m_pAdvancedSearchButton->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( HistoryPanel3Base::OnAdvancedSearchToggled ), NULL, this );
	m_pAnyDate->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pAnyDate->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pTodayAM->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pTodayAM->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pTodayPM->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pTodayPM->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pToday->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pToday->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pYesterday->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pYesterday->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pLastWeek->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pLastWeek->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pLastMonth->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pLastMonth->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pLastThreeMonths->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pLastThreeMonths->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pBetween->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pBetween->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( HistoryPanel3Base::OnSearchChange ), NULL, this );
	m_pTextControlFechaDesde->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( HistoryPanel3Base::OnDateChanged ), NULL, this );
	m_pTextControlFechaDesde->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaHasta->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( HistoryPanel3Base::OnDateChanged ), NULL, this );
	m_pTextControlFechaHasta->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( HistoryPanel3Base::OnKeyDownFormulario ), NULL, this );
	
}

ConfirmDeleteDialogBase::ConfirmDeleteDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 550,220 ), wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_body = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_pTitled = new TitledPanel( m_body, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pTitled->SetToolTip( _("Confirm removal") );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer23->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pLabelEliminar = new wxStaticText( m_pTitled, wxID_ANY, _("Are you sure you want to delete "), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelEliminar->Wrap( 450 );
	bSizer23->Add( m_pLabelEliminar, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer23->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_pTitled->SetSizer( bSizer23 );
	m_pTitled->Layout();
	bSizer23->Fit( m_pTitled );
	bSizer22->Add( m_pTitled, 1, wxEXPAND, 5 );
	
	
	m_body->SetSizer( bSizer22 );
	m_body->Layout();
	bSizer22->Fit( m_body );
	bSizer5->Add( m_body, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pEliminar = new wxButton( m_pFooter, wxID_ANY, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_pEliminar, 0, wxRIGHT, 5 );
	
	m_pCancelar = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pCancelar->SetDefault(); 
	bSizer7->Add( m_pCancelar, 0, wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( bSizer7 );
	m_pFooter->Layout();
	bSizer7->Fit( m_pFooter );
	bSizer5->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pEliminar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfirmDeleteDialogBase::OnEliminarClick ), NULL, this );
	m_pCancelar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfirmDeleteDialogBase::OnCancelarClick ), NULL, this );
}

ConfirmDeleteDialogBase::~ConfirmDeleteDialogBase()
{
	// Disconnect Events
	m_pEliminar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfirmDeleteDialogBase::OnEliminarClick ), NULL, this );
	m_pCancelar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfirmDeleteDialogBase::OnCancelarClick ), NULL, this );
	
}

OpenWithDialogBase::OpenWithDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,350 ), wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelTitulo = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelTitulo->SetToolTip( _("Choose the view you want to use to open file/s") );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_pListaVistas = new wxListBox( m_pPanelTitulo, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	bSizer16->Add( m_pListaVistas, 1, wxEXPAND, 5 );
	
	m_pCheckUtilizarSiempre = new wxCheckBox( m_pPanelTitulo, wxID_ANY, _("Always use this view to open these files"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_pCheckUtilizarSiempre, 0, wxTOP, 5 );
	
	
	m_pPanelTitulo->SetSizer( bSizer16 );
	m_pPanelTitulo->Layout();
	bSizer16->Fit( m_pPanelTitulo );
	bSizer15->Add( m_pPanelTitulo, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer15 );
	m_pBody->Layout();
	bSizer15->Fit( m_pBody );
	bSizer2->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button3 = new wxButton( m_pFooter, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button3->SetDefault(); 
	bSizer3->Add( m_button3, 0, wxRIGHT, 5 );
	
	m_button4 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button4, 0, wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( bSizer3 );
	m_pFooter->Layout();
	bSizer3->Fit( m_pFooter );
	bSizer2->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	// Connect Events
	m_pListaVistas->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( OpenWithDialogBase::OnAceptarClick ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( OpenWithDialogBase::OnAceptarClick ), NULL, this );
}

OpenWithDialogBase::~OpenWithDialogBase()
{
	// Disconnect Events
	m_pListaVistas->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( OpenWithDialogBase::OnAceptarClick ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( OpenWithDialogBase::OnAceptarClick ), NULL, this );
	
}

SynchronizeDialogBase::SynchronizeDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,250 ), wxDefaultSize );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelMessage = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelMessage->SetToolTip( _("Synchronize files") );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_pLabel = new wxStaticText( m_pPanelMessage, wxID_ANY, _("There are files that has to be downloaded to local database before executing this action, \ndo you want to synchornize files now?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	bSizer4->Add( m_pLabel, 0, wxALL, 5 );
	
	
	m_pPanelMessage->SetSizer( bSizer4 );
	m_pPanelMessage->Layout();
	bSizer4->Fit( m_pPanelMessage );
	bSizer7->Add( m_pPanelMessage, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer7 );
	m_pBody->Layout();
	bSizer7->Fit( m_pBody );
	bSizer15->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button4 = new wxButton( m_pFooter, wxID_OK, _("Synchronize"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button4->SetDefault(); 
	bSizer16->Add( m_button4, 0, wxRIGHT, 5 );
	
	m_button5 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button5, 0, wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( bSizer16 );
	m_pFooter->Layout();
	bSizer16->Fit( m_pFooter );
	bSizer15->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer15 );
	this->Layout();
	bSizer15->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SynchronizeDialogBase::OnSynchronize ), NULL, this );
}

SynchronizeDialogBase::~SynchronizeDialogBase()
{
	// Disconnect Events
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SynchronizeDialogBase::OnSynchronize ), NULL, this );
	
}
