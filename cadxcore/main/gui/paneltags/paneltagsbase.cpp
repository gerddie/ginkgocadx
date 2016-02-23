///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "paneltagsbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

PanelTagsBase::PanelTagsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 700,500 ), wxDefaultSize );
	
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pTags = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER);
	m_pMainSizer->Add( m_pTags, 1, wxEXPAND | wxALL, 5 );
	
	m_panel1 = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_button1 = new wxButton( m_panel1, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button1->SetDefault(); 
	bSizer2->Add( m_button1, 0, wxALIGN_RIGHT, 5 );
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_pMainSizer->Add( m_panel1, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	m_pMainSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PanelTagsBase::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ) );
	m_pTags->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ), NULL, this );
	m_panel1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ), NULL, this );
	m_button1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ), NULL, this );
}

PanelTagsBase::~PanelTagsBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PanelTagsBase::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ) );
	m_pTags->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ), NULL, this );
	m_panel1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ), NULL, this );
	m_button1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( PanelTagsBase::OnKeyDown ), NULL, this );
	
}
