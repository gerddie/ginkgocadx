///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "ventanaprincipalbase.h"

///////////////////////////////////////////////////////////////////////////

VentanaPrincipalBase::VentanaPrincipalBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
	
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelActualizacion = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelActualizacion->SetBackgroundColour( wxColour( 210, 210, 0 ) );
	m_pPanelActualizacion->Hide();
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelActualizacionInterno = new wxPanel( m_pPanelActualizacion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelActualizacionInterno->SetBackgroundColour( wxColour( 255, 255, 225 ) );
	m_pPanelActualizacionInterno->SetToolTip( _("New Update") );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	

	
	bSizer17->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPanelActualizacionInterno->SetSizer( bSizer17 );
	m_pPanelActualizacionInterno->Layout();
	bSizer17->Fit( m_pPanelActualizacionInterno );
	bSizer18->Add( m_pPanelActualizacionInterno, 0, wxEXPAND, 2 );
	
	
	m_pPanelActualizacion->SetSizer( bSizer18 );
	m_pPanelActualizacion->Layout();
	bSizer18->Fit( m_pPanelActualizacion );
	m_pMainSizer->Add( m_pPanelActualizacion, 0, wxEXPAND, 0 );
	
	m_pSizerCentral = new wxBoxSizer( wxHORIZONTAL );
	
	m_pPanelCentral = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelCentral->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	m_pSizerCentral->Add( m_pPanelCentral, 1, wxEXPAND, 1 );
	
	
	m_pMainSizer->Add( m_pSizerCentral, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaPrincipalBase::OnKeydown ) );
}

VentanaPrincipalBase::~VentanaPrincipalBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaPrincipalBase::OnKeydown ) );
}

DialogoComandosEnEjecucionBase::DialogoComandosEnEjecucionBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 510,230 ), wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_panel10 = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_panel11 = new TitledPanelSinBorder( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel11->SetToolTip( _("The following commands will be aborted if you close the application") );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_pListacomandos = new wxListBox( m_panel11, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer16->Add( m_pListacomandos, 1, wxEXPAND, 5 );
	
	
	m_panel11->SetSizer( bSizer16 );
	m_panel11->Layout();
	bSizer16->Fit( m_panel11 );
	bSizer15->Add( m_panel11, 1, wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( bSizer15 );
	m_panel10->Layout();
	bSizer15->Fit( m_panel10 );
	bSizer3->Add( m_panel10, 1, wxEXPAND, 5 );
	
	m_panel9 = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button1 = new wxButton( m_panel9, wxID_OK, _("Close Ginkgo CADx"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button1->SetToolTip( _("Closes the application and aborts pending commands") );
	
	bSizer4->Add( m_button1, 0, wxRIGHT, 5 );
	
	m_button2 = new wxButton( m_panel9, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button2->SetDefault(); 
	m_button2->SetToolTip( _("Do not close the application and keep commands running") );
	
	bSizer4->Add( m_button2, 0, wxLEFT, 5 );
	
	
	m_panel9->SetSizer( bSizer4 );
	m_panel9->Layout();
	bSizer4->Fit( m_panel9 );
	bSizer3->Add( m_panel9, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );
	
	this->Centre( wxBOTH );
}

DialogoComandosEnEjecucionBase::~DialogoComandosEnEjecucionBase()
{
}

DetachedDialogBase::DetachedDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	
	this->SetSizer( bSizer8 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DetachedDialogBase::OnCloseDialogo ) );
}

DetachedDialogBase::~DetachedDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DetachedDialogBase::OnCloseDialogo ) );
	
}

DialogoLoginBase::DialogoLoginBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,270 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("User login") );
	
	m_pMainSizer->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	m_pFormPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pFormPanel->SetToolTip( _("System authentication") );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_pFormSizer;
	m_pFormSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_pFormSizer->AddGrowableCol( 1 );
	m_pFormSizer->SetFlexibleDirection( wxBOTH );
	m_pFormSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pLabelUser = new wxStaticText( m_pFormPanel, wxID_ANY, _("User name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelUser->Wrap( -1 );
	m_pFormSizer->Add( m_pLabelUser, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxBOTTOM|wxRIGHT, 5 );
	
	m_pTCUsuario = new wxTextCtrl( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pTCUsuario->SetMaxLength( 0 ); 
	m_pFormSizer->Add( m_pTCUsuario, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxBOTTOM|wxLEFT, 5 );
	
	m_staticText3 = new wxStaticText( m_pFormPanel, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_pFormSizer->Add( m_staticText3, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	m_pTCPassword = new wxTextCtrl( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_pTCPassword->SetMaxLength( 0 ); 
	m_pFormSizer->Add( m_pTCPassword, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxLEFT, 5 );
	
	
	bSizer19->Add( m_pFormSizer, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_pFormPanel->SetSizer( bSizer19 );
	m_pFormPanel->Layout();
	bSizer19->Fit( m_pFormPanel );
	bSizer141->Add( m_pFormPanel, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer141 );
	m_pBody->Layout();
	bSizer141->Fit( m_pBody );
	m_pMainSizer->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_panel6 = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pBotonCancelar = new wxButton( m_panel6, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_pBotonCancelar, 0, wxALIGN_CENTER_VERTICAL, 0 );
	
	
	bSizer14->Add( 0, 0, 1, 0, 0 );
	
	m_pButtonLogin = new wxButton( m_panel6, wxID_OK, _("Login"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pButtonLogin->SetDefault(); 
	bSizer14->Add( m_pButtonLogin, 0, 0, 0 );
	
	
	m_panel6->SetSizer( bSizer14 );
	m_panel6->Layout();
	bSizer14->Fit( m_panel6 );
	m_pMainSizer->Add( m_panel6, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	m_pMainSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pButtonLogin->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoLoginBase::OnLogin ), NULL, this );
}

DialogoLoginBase::~DialogoLoginBase()
{
	// Disconnect Events
	m_pButtonLogin->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoLoginBase::OnLogin ), NULL, this );
	
}
