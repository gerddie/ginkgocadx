///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "selecciontextobase.h"

///////////////////////////////////////////////////////////////////////////

SeleccionTextoBase::SeleccionTextoBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,250 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel4->SetToolTip( _("Note") );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pControlTexto = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_WORDWRAP );
	m_pControlTexto->SetMaxLength( 0 ); 
	m_pControlTexto->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	
	bSizer6->Add( m_pControlTexto, 1, wxEXPAND|wxBOTTOM, 5 );
	
	
	m_panel4->SetSizer( bSizer6 );
	m_panel4->Layout();
	bSizer6->Fit( m_panel4 );
	bSizer5->Add( m_panel4, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer5 );
	m_pBody->Layout();
	bSizer5->Fit( m_pBody );
	m_pMainSizer->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_pSizerBotonera;
	m_pSizerBotonera = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_pSizerBotonera->Add( 0, 0, 1, wxEXPAND, 2 );
	
	m_pBotonAceptar = new wxButton( m_pFooter, wxID_ANY, _("Accept"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_pBotonAceptar->SetDefault(); 
	m_pSizerBotonera->Add( m_pBotonAceptar, 0, wxRIGHT, 5 );
	
	m_pBotonCancelar = new wxButton( m_pFooter, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_pSizerBotonera->Add( m_pBotonCancelar, 0, wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( m_pSizerBotonera );
	m_pFooter->Layout();
	m_pSizerBotonera->Fit( m_pFooter );
	m_pMainSizer->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( SeleccionTextoBase::OnActivar ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( SeleccionTextoBase::OnDialogoKeyDown ) );
	m_pControlTexto->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( SeleccionTextoBase::OnControlTextoKeyDown ), NULL, this );
	m_pControlTexto->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SeleccionTextoBase::OnControlTextoTextoActualizado ), NULL, this );
	m_pBotonAceptar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeleccionTextoBase::OnBotonAceptarClick ), NULL, this );
	m_pBotonCancelar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeleccionTextoBase::OnBotonCancelarClick ), NULL, this );
}

SeleccionTextoBase::~SeleccionTextoBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( SeleccionTextoBase::OnActivar ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( SeleccionTextoBase::OnDialogoKeyDown ) );
	m_pControlTexto->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( SeleccionTextoBase::OnControlTextoKeyDown ), NULL, this );
	m_pControlTexto->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SeleccionTextoBase::OnControlTextoTextoActualizado ), NULL, this );
	m_pBotonAceptar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeleccionTextoBase::OnBotonAceptarClick ), NULL, this );
	m_pBotonCancelar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeleccionTextoBase::OnBotonCancelarClick ), NULL, this );
	
}
