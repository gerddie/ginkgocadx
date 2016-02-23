///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "dicomizationcompletebase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

DicomizationCompleteBase::DicomizationCompleteBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,250 ), wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	m_pHeader->SetToolTip( _("Dicomization complete") );
	
	bSizer3->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBodyPanel = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelTitled = new TitledPanel( m_pBodyPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelTitled->SetToolTip( _("Dicomization complete") );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( m_pPanelTitled, wxID_ANY, _("Dicomization finished successfully"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer16->Add( m_staticText8, 0, wxALL, 5 );
	
	m_pPanelTitled->SetSizer( bSizer16 );
	m_pPanelTitled->Layout();
	bSizer16->Fit( m_pPanelTitled );
	bSizer7->Add( m_pPanelTitled, 0, wxEXPAND, 5 );
	
	m_pBodyPanel->SetSizer( bSizer7 );
	m_pBodyPanel->Layout();
	bSizer7->Fit( m_pBodyPanel );
	bSizer3->Add( m_pBodyPanel, 1, wxEXPAND, 5 );
	
	m_panel12 = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pCheckNotShow = new wxCheckBox( m_panel12, wxID_ANY, _("Don't show this message again"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_pCheckNotShow, 0, wxALL, 5 );
	
	
	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button5 = new wxButton( m_panel12, wxID_CANCEL, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button5->SetDefault(); 
	bSizer15->Add( m_button5, 0, wxRIGHT, 5 );
	
	m_panel12->SetSizer( bSizer15 );
	m_panel12->Layout();
	bSizer15->Fit( m_panel12 );
	bSizer3->Add( m_panel12, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DicomizationCompleteBase::OnYesClick ), NULL, this );
}

DicomizationCompleteBase::~DicomizationCompleteBase()
{
	// Disconnect Events
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DicomizationCompleteBase::OnYesClick ), NULL, this );
	
}
