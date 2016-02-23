///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "calibracionbase.h"

///////////////////////////////////////////////////////////////////////////

CalibracionBase::CalibracionBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 340,180 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelEspera = new wxPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelEspera->Hide();
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pLabelMensaje = new wxStaticText( m_pPanelEspera, wxID_ANY, _("Calibrating..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelMensaje->Wrap( -1 );
	bSizer7->Add( m_pLabelMensaje, 1, wxALL|wxEXPAND, 2 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pPanelEspera->SetSizer( bSizer7 );
	m_pPanelEspera->Layout();
	bSizer7->Fit( m_pPanelEspera );
	bSizer6->Add( m_pPanelEspera, 1, wxEXPAND | wxALL, 5 );
	
	m_panelMedida = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_panelMedida->SetToolTip( _("Real Measure") );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_panelMedida, wxID_ANY, _("Distance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer71->Add( m_staticText1, 0, wxALL, 5 );
	
	m_ControlDistancia = new wxTextCtrl( m_panelMedida, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), 0 );
	bSizer71->Add( m_ControlDistancia, 1, wxALL, 2 );
	
	wxString m_unidadesChoices[] = { _("Microns"), _("Milimeters"), _("Centimeters"), _("Meters") };
	int m_unidadesNChoices = sizeof( m_unidadesChoices ) / sizeof( wxString );
	m_unidades = new wxChoice( m_panelMedida, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_unidadesNChoices, m_unidadesChoices, 0 );
	m_unidades->SetSelection( 0 );
	bSizer71->Add( m_unidades, 0, wxALL, 2 );
	
	m_panelMedida->SetSizer( bSizer71 );
	m_panelMedida->Layout();
	bSizer71->Fit( m_panelMedida );
	bSizer6->Add( m_panelMedida, 1, wxEXPAND, 5 );
	
	m_pBody->SetSizer( bSizer6 );
	m_pBody->Layout();
	bSizer6->Fit( m_pBody );
	m_pMainSizer->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_pSizerBotonera;
	m_pSizerBotonera = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_pSizerBotonera->Add( 0, 0, 1, wxEXPAND, 2 );
	
	m_pBotonAceptar = new wxButton( m_pFooter, wxID_ANY, _("Accept"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_pBotonAceptar->SetDefault(); 
	m_pSizerBotonera->Add( m_pBotonAceptar, 0, wxRIGHT, 2 );
	
	m_pBotonCancelar = new wxButton( m_pFooter, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_pSizerBotonera->Add( m_pBotonCancelar, 0, wxLEFT, 2 );
	
	m_pFooter->SetSizer( m_pSizerBotonera );
	m_pFooter->Layout();
	m_pSizerBotonera->Fit( m_pFooter );
	m_pMainSizer->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	m_pMainSizer->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( CalibracionBase::OnActivate ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CalibracionBase::OnDialogoKeyDown ) );
	m_ControlDistancia->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CalibracionBase::OnControlDistanciaKeyDown ), NULL, this );
	m_pBotonAceptar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalibracionBase::OnBotonAceptarClick ), NULL, this );
	m_pBotonCancelar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalibracionBase::OnBotonCancelarClick ), NULL, this );
}

CalibracionBase::~CalibracionBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( CalibracionBase::OnActivate ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CalibracionBase::OnDialogoKeyDown ) );
	m_ControlDistancia->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CalibracionBase::OnControlDistanciaKeyDown ), NULL, this );
	m_pBotonAceptar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalibracionBase::OnBotonAceptarClick ), NULL, this );
	m_pBotonCancelar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalibracionBase::OnBotonCancelarClick ), NULL, this );
	
}
