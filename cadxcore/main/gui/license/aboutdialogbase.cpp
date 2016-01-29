///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "aboutdialogbase.h"

///////////////////////////////////////////////////////////////////////////

AboutDialogBase::AboutDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 550,400 ), wxSize( -1,-1 ) );
	
	wxBoxSizer* bSizerPrincipal;
	bSizerPrincipal = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Ginkgo CADx") );
	
	bSizerPrincipal->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelSuperior = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelSuperior->SetToolTip( _("Credits") );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	m_pCredits = new wxHtmlWindow( m_pPanelSuperior, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	bSizer81->Add( m_pCredits, 1, wxEXPAND, 5 );
	
	
	m_pPanelSuperior->SetSizer( bSizer81 );
	m_pPanelSuperior->Layout();
	bSizer81->Fit( m_pPanelSuperior );
	bSizer11->Add( m_pPanelSuperior, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer11 );
	m_pBody->Layout();
	bSizer11->Fit( m_pBody );
	bSizerPrincipal->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonLicenciasOS = new wxButton( m_pFooter, wxID_ANY, _("Licenses"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_buttonLicenciasOS, 0, 0, 5 );
	
	m_pActualizacionInfo = new wxStaticText( m_pFooter, wxID_ANY, _("UPDATE_INFO"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_pActualizacionInfo->Wrap( -1 );
	m_pActualizacionInfo->Hide();
	
	bSizer8->Add( m_pActualizacionInfo, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_buttonAceptar = new wxButton( m_pFooter, wxID_CANCEL, _("&Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAceptar->SetDefault(); 
	bSizer8->Add( m_buttonAceptar, 0, 0, 5 );
	
	
	m_pFooter->SetSizer( bSizer8 );
	m_pFooter->Layout();
	bSizer8->Fit( m_pFooter );
	bSizerPrincipal->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerPrincipal );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( AboutDialogBase::OnAnyKeyDown ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( AboutDialogBase::OnSize ) );
	m_pCredits->Connect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutDialogBase::OnTextURL ), NULL, this );
	m_buttonLicenciasOS->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnLicenciasOpenSource ), NULL, this );
}

AboutDialogBase::~AboutDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( AboutDialogBase::OnAnyKeyDown ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( AboutDialogBase::OnSize ) );
	m_pCredits->Disconnect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutDialogBase::OnTextURL ), NULL, this );
	m_buttonLicenciasOS->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnLicenciasOpenSource ), NULL, this );
	
}

DialogoLicenciasOpenSourceBase::DialogoLicenciasOpenSourceBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,300 ), wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Open Source Licenses") );
	
	bSizer5->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListaLicencias = new wxListBox( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE );
	m_pListaLicencias->Append( _("Cairo") );
	m_pListaLicencias->Append( _("DCMTK") );
	m_pListaLicencias->Append( _("FamFamFam Icons") );
	m_pListaLicencias->Append( _("Fugue Icons") );
	m_pListaLicencias->Append( _("ITK") );
	m_pListaLicencias->Append( _("LibJpeg") );
	m_pListaLicencias->Append( _("LibPng") );
	m_pListaLicencias->Append( _("OpenSSL") );
	m_pListaLicencias->Append( _("VTK") );
	m_pListaLicencias->Append( _("WxWidgets") );
	m_pListaLicencias->Append( _("Zlib") );
	m_pListaLicencias->Append( _("SQL Lite") );
	m_pListaLicencias->Append( _("CharLS") );
	m_pListaLicencias->Append( _("Dark Glass Icons") );
	m_pListaLicencias->Append( _("Libcurl") );
	m_pListaLicencias->Append( _("JsonCpp") );
	m_pListaLicencias->Append( _("AIRS") );
	bSizer7->Add( m_pListaLicencias, 0, wxALL|wxEXPAND, 5 );
	
	m_pRichTextTextoLicencia = new wxRichTextCtrl( m_pBody, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer7->Add( m_pRichTextTextoLicencia, 1, wxEXPAND | wxALL, 5 );
	
	
	m_pBody->SetSizer( bSizer7 );
	m_pBody->Layout();
	bSizer7->Fit( m_pBody );
	bSizer5->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( 0, 0, 1, 0, 5 );
	
	m_buttonAceptar = new wxButton( m_pFooter, wxID_CANCEL, _("&Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAceptar->SetDefault(); 
	bSizer8->Add( m_buttonAceptar, 0, wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( bSizer8 );
	m_pFooter->Layout();
	bSizer8->Fit( m_pFooter );
	bSizer5->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pListaLicencias->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DialogoLicenciasOpenSourceBase::OnListaLicencias ), NULL, this );
}

DialogoLicenciasOpenSourceBase::~DialogoLicenciasOpenSourceBase()
{
	// Disconnect Events
	m_pListaLicencias->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DialogoLicenciasOpenSourceBase::OnListaLicencias ), NULL, this );
	
}
