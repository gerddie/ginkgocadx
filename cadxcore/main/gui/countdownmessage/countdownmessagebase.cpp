///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "countdownmessagebase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

InactivityCountdownMessageBase::InactivityCountdownMessageBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,270 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Inactivity detected") );
	
	m_pMainSizer->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	m_pFormPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pFormPanel->SetToolTip( _("Inactivity detected") );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabelInactivity = new wxStaticText( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelInactivity->Wrap( 380 );
	bSizer19->Add( m_pLabelInactivity, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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
	
	
	bSizer14->Add( 0, 0, 1, 0, 0 );
	
	m_pButtonClose = new wxButton( m_panel6, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_pButtonClose, 0, wxRIGHT, 5 );
	
	m_pBotonCancelar = new wxButton( m_panel6, wxID_OK, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBotonCancelar->SetDefault(); 
	bSizer14->Add( m_pBotonCancelar, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	
	m_panel6->SetSizer( bSizer14 );
	m_panel6->Layout();
	bSizer14->Fit( m_panel6 );
	m_pMainSizer->Add( m_panel6, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	m_pMainSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( InactivityCountdownMessageBase::OnClose ) );
	m_pButtonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InactivityCountdownMessageBase::OnCloseButton ), NULL, this );
	m_pBotonCancelar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InactivityCountdownMessageBase::OnCancel ), NULL, this );
}

InactivityCountdownMessageBase::~InactivityCountdownMessageBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( InactivityCountdownMessageBase::OnClose ) );
	m_pButtonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InactivityCountdownMessageBase::OnCloseButton ), NULL, this );
	m_pBotonCancelar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InactivityCountdownMessageBase::OnCancel ), NULL, this );
	
}

CountdownMessageBase::CountdownMessageBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 415,300 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Info") );
	
	m_pMainSizer->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	m_pFormPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pFormPanel->SetToolTip( _("Inactivity detected") );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabelInactivity = new wxStaticText( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelInactivity->Wrap( -1 );
	bSizer19->Add( m_pLabelInactivity, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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
	
	
	bSizer14->Add( 0, 0, 1, 0, 0 );
	
	m_pButtonOk = new wxButton( m_panel6, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pButtonOk->SetDefault(); 
	bSizer14->Add( m_pButtonOk, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pBotonCancelar = new wxButton( m_panel6, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_pBotonCancelar, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_panel6->SetSizer( bSizer14 );
	m_panel6->Layout();
	bSizer14->Fit( m_panel6 );
	m_pMainSizer->Add( m_panel6, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	m_pMainSizer->Fit( this );
	
	this->Centre( wxBOTH );
}

CountdownMessageBase::~CountdownMessageBase()
{
}
