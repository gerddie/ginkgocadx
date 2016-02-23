///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "pasosexportacionbase.h"

///////////////////////////////////////////////////////////////////////////

ParametrosPrincipalesBase::ParametrosPrincipalesBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelProfile = new TitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelProfile->SetToolTip( _("Profile") );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText15 = new wxStaticText( m_pPanelProfile, wxID_ANY, _("Format"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer61->Add( m_staticText15, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxString m_pFormatChoices[] = { _("DICOM (native format)"), _("Image (jpg, bmp, png)") };
	int m_pFormatNChoices = sizeof( m_pFormatChoices ) / sizeof( wxString );
	m_pFormat = new wxChoice( m_pPanelProfile, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pFormatNChoices, m_pFormatChoices, 0 );
	m_pFormat->SetSelection( 0 );
	bSizer61->Add( m_pFormat, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pAllFiles = new wxCheckBox( m_pPanelProfile, wxID_ANY, _("Export all files of opened tab"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_pAllFiles, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer6->Add( bSizer61, 1, wxEXPAND, 5 );
	
	
	m_pPanelProfile->SetSizer( bSizer6 );
	m_pPanelProfile->Layout();
	bSizer6->Fit( m_pPanelProfile );
	bSizer14->Add( m_pPanelProfile, 0, wxEXPAND, 5 );
	
	m_pSep = new TitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer14->Add( m_pSep, 0, wxEXPAND, 5 );
	
	m_pPanelJpeg = new TitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelJpeg->Hide();
	m_pPanelJpeg->SetToolTip( _("JPEG") );
	
	wxBoxSizer* bSizer6111;
	bSizer6111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( m_pPanelJpeg, wxID_ANY, _("Quality"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer6111->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pSliderCalidad = new wxSlider( m_pPanelJpeg, wxID_ANY, 95, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS|wxSL_TOP );
	bSizer6111->Add( m_pSliderCalidad, 1, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer6111->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_pPanelJpeg->SetSizer( bSizer6111 );
	m_pPanelJpeg->Layout();
	bSizer6111->Fit( m_pPanelJpeg );
	bSizer14->Add( m_pPanelJpeg, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer14 );
	this->Layout();
	
	// Connect Events
	m_pFormat->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ParametrosPrincipalesBase::OnChoiceFormatoDestino ), NULL, this );
}

ParametrosPrincipalesBase::~ParametrosPrincipalesBase()
{
	// Disconnect Events
	m_pFormat->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ParametrosPrincipalesBase::OnChoiceFormatoDestino ), NULL, this );
	
}
