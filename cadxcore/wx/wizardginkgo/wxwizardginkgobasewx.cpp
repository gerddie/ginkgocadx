///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "wxwizardginkgobasewx.h"

///////////////////////////////////////////////////////////////////////////

wxWizardGinkgoBaseWx::wxWizardGinkgoBaseWx( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,300 ), wxDefaultSize );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer28->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBodyPanel = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelPrincipal = new wxPanel( m_pBodyPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pSizerPrincipal = new wxBoxSizer( wxVERTICAL );
	
	
	m_pPanelPrincipal->SetSizer( m_pSizerPrincipal );
	m_pPanelPrincipal->Layout();
	m_pSizerPrincipal->Fit( m_pPanelPrincipal );
	bSizer6->Add( m_pPanelPrincipal, 1, wxEXPAND, 0 );
	
	
	m_pBodyPanel->SetSizer( bSizer6 );
	m_pBodyPanel->Layout();
	bSizer6->Fit( m_pBodyPanel );
	bSizer28->Add( m_pBodyPanel, 1, wxEXPAND, 0 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_pArchSizerPanelPropiedades;
	m_pArchSizerPanelPropiedades = new wxBoxSizer( wxHORIZONTAL );
	
	m_pBottomLeftLabel = new wxStaticText( m_pFooter, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pBottomLeftLabel->Wrap( -1 );
	m_pArchSizerPanelPropiedades->Add( m_pBottomLeftLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	
	m_pArchSizerPanelPropiedades->Add( 0, 0, 1, wxEXPAND, 2 );
	
	m_pBAnterior = new wxButton( m_pFooter, wxID_ANY, _("< &Previous"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pArchSizerPanelPropiedades->Add( m_pBAnterior, 0, wxRIGHT|wxLEFT, 5 );
	
	m_pBSiguiente = new wxButton( m_pFooter, wxID_OK, _("&Next >"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBSiguiente->SetDefault(); 
	m_pArchSizerPanelPropiedades->Add( m_pBSiguiente, 0, wxRIGHT|wxLEFT, 5 );
	
	
	m_pArchSizerPanelPropiedades->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_pBCerrar = new wxButton( m_pFooter, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pArchSizerPanelPropiedades->Add( m_pBCerrar, 0, wxRIGHT|wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( m_pArchSizerPanelPropiedades );
	m_pFooter->Layout();
	m_pArchSizerPanelPropiedades->Fit( m_pFooter );
	bSizer28->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer28 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxWizardGinkgoBaseWx::OnClose ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( wxWizardGinkgoBaseWx::OnSize ) );
	m_pBAnterior->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxWizardGinkgoBaseWx::OnAnteriorClick ), NULL, this );
	m_pBSiguiente->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxWizardGinkgoBaseWx::OnSiguienteClick ), NULL, this );
	m_pBCerrar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxWizardGinkgoBaseWx::OnCancelarClick ), NULL, this );
}

wxWizardGinkgoBaseWx::~wxWizardGinkgoBaseWx()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxWizardGinkgoBaseWx::OnClose ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( wxWizardGinkgoBaseWx::OnSize ) );
	m_pBAnterior->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxWizardGinkgoBaseWx::OnAnteriorClick ), NULL, this );
	m_pBSiguiente->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxWizardGinkgoBaseWx::OnSiguienteClick ), NULL, this );
	m_pBCerrar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxWizardGinkgoBaseWx::OnCancelarClick ), NULL, this );
	
}
