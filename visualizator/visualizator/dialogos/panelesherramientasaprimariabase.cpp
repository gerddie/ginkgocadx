///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "panelesherramientasaprimariabase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNKVisualizator::GUI;

PanelResultadosBase::PanelResultadosBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 265,200 ) );
	
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pTags = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER);
	m_pMainSizer->Add( m_pTags, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PanelResultadosBase::OnSize ) );
}

PanelResultadosBase::~PanelResultadosBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PanelResultadosBase::OnSize ) );
	
}

PanelOverlaysBase::PanelOverlaysBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 366,177 ), wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_pCheckListOverlaysChoices;
	m_pCheckListOverlays = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pCheckListOverlaysChoices, 0 );
	bSizer2->Add( m_pCheckListOverlays, 1, wxALL|wxEXPAND, 2 );
	
	m_pLabel = new wxStaticText( this, wxID_ANY, _("There are no overlays on this image"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	m_pLabel->Hide();
	
	bSizer2->Add( m_pLabel, 0, wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PanelOverlaysBase::OnClose ) );
	this->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( PanelOverlaysBase::OnKillFocus ) );
	m_pCheckListOverlays->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( PanelOverlaysBase::OnItemChecked ), NULL, this );
}

PanelOverlaysBase::~PanelOverlaysBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PanelOverlaysBase::OnClose ) );
	this->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( PanelOverlaysBase::OnKillFocus ) );
	m_pCheckListOverlays->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( PanelOverlaysBase::OnItemChecked ), NULL, this );
	
}
