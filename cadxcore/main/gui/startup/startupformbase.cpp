///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "startupformbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

StartUpFormBase::StartUpFormBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	this->SetMinSize( wxSize( 548,288 ) );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerToolBar = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer5->Add( m_pSizerToolBar, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_hyperlink2 = new wxHyperlinkCtrl( this, wxID_ANY, _("http://ginkgo-cadx.com"), wxT("http://ginkgo-cadx.com"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer13->Add( m_hyperlink2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSizerCenter = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("No suitable for diagnostic usage. Please consider upgrade to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( 360 );
	m_staticText2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText2->SetForegroundColour( wxColour( 255, 0, 0 ) );
	
	bSizer6->Add( m_staticText2, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_hyperlink31 = new wxHyperlinkCtrl( this, wxID_ANY, _("Ginkgo CADx Pro"), wxT("http://ginkgo-cadx.com/en/downloads"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_hyperlink31->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6->Add( m_hyperlink31, 0, wxALL, 5 );
	
	
	bSizer51->Add( bSizer6, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Remember that when you buy Ginkgo CADx Pro, you are contributing to development efforts"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText3->Wrap( 350 );
	m_staticText3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer51->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	m_pSizerCenter->Add( bSizer51, 1, wxEXPAND, 5 );
	
	
	bSizer13->Add( m_pSizerCenter, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Powered by"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer13->Add( m_staticText9, 0, wxALIGN_RIGHT|wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_hyperlink3 = new wxHyperlinkCtrl( this, wxID_ANY, _("MetaEmotion Healthcare"), wxT("http://healthcare.metaemotion.com"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_hyperlink3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer13->Add( m_hyperlink3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer5->Add( bSizer13, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 20 );
	
	
	bSizer5->Add( 0, 15, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( StartUpFormBase::OnEraseBackground ) );
	m_hyperlink3->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( StartUpFormBase::erasdf ), NULL, this );
}

StartUpFormBase::~StartUpFormBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( StartUpFormBase::OnEraseBackground ) );
	m_hyperlink3->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( StartUpFormBase::erasdf ), NULL, this );
	
}
