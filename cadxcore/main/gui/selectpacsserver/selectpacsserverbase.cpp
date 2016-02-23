///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "selectpacsserverbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

SelectPacsServerBase::SelectPacsServerBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 380,200 ), wxDefaultSize );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelMessage = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelMessage->SetToolTip( _("Select PACS") );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabel = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Select PACS server"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	bSizer4->Add( m_pLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_pServerChoices;
	m_pServer = new wxChoice( m_pPanelMessage, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pServerChoices, 0 );
	m_pServer->SetSelection( 0 );
	m_pServer->SetMaxSize( wxSize( 160,-1 ) );
	
	bSizer4->Add( m_pServer, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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
	
	m_pOkButton = new wxButton( m_pFooter, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pOkButton->SetDefault(); 
	bSizer16->Add( m_pOkButton, 0, wxRIGHT, 5 );
	
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
}

SelectPacsServerBase::~SelectPacsServerBase()
{
}
