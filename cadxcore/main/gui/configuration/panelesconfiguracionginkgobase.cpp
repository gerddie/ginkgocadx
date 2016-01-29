///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "panelesconfiguracionginkgobase.h"

///////////////////////////////////////////////////////////////////////////

DialogoConfiguracionBase::DialogoConfiguracionBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 860,300 ), wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Settings") );
	
	bSizer3->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pImagePanel = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pImagePanel->SetToolTip( _("Settings") );
	
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter2 = new wxSplitterWindow( m_pImagePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( DialogoConfiguracionBase::m_splitter2OnIdle ), NULL, this );
	m_splitter2->SetMinimumPaneSize( 100 );
	
	m_splitter2->SetBackgroundColour( wxColour( 243, 243, 243 ) );
	
	m_panel1 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetMinSize( wxSize( 180,-1 ) );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_pTreeNavegacion = new wxTreeCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer5->Add( m_pTreeNavegacion, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel1->SetSizer( bSizer5 );
	m_panel1->Layout();
	bSizer5->Fit( m_panel1 );
	m_pTitledPanelConfig = new TitledPanelSinBorder( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelConfig = new PanelScrollable( m_pTitledPanelConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pSizerPaneles = new wxBoxSizer( wxVERTICAL );
	
	
	m_pPanelConfig->SetSizer( m_pSizerPaneles );
	m_pPanelConfig->Layout();
	m_pSizerPaneles->Fit( m_pPanelConfig );
	bSizer44->Add( m_pPanelConfig, 1, wxEXPAND, 5 );
	
	
	m_pTitledPanelConfig->SetSizer( bSizer44 );
	m_pTitledPanelConfig->Layout();
	bSizer44->Fit( m_pTitledPanelConfig );
	m_splitter2->SplitVertically( m_panel1, m_pTitledPanelConfig, 245 );
	bSizer43->Add( m_splitter2, 1, wxEXPAND, 5 );
	
	
	m_pImagePanel->SetSizer( bSizer43 );
	m_pImagePanel->Layout();
	bSizer43->Fit( m_pImagePanel );
	bSizer3->Add( m_pImagePanel, 1, wxEXPAND, 5 );
	
	m_panel3 = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button12 = new wxButton( m_panel3, wxID_ANY, _("Export config..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( m_button12, 0, wxRIGHT, 5 );
	
	m_button13 = new wxButton( m_panel3, wxID_ANY, _("Import config..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( m_button13, 0, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer7->Add( bSizer22, 0, 0, 5 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button8 = new wxButton( m_panel3, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button8->SetDefault(); 
	bSizer21->Add( m_button8, 0, wxRIGHT|wxLEFT, 5 );
	
	m_button9 = new wxButton( m_panel3, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button9, 0, wxRIGHT|wxLEFT, 5 );
	
	m_pBAplicar = new wxButton( m_panel3, wxID_ANY, _("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBAplicar->Enable( false );
	
	bSizer21->Add( m_pBAplicar, 0, wxLEFT, 5 );
	
	
	bSizer7->Add( bSizer21, 0, 0, 5 );
	
	
	m_panel3->SetSizer( bSizer7 );
	m_panel3->Layout();
	bSizer7->Fit( m_panel3 );
	bSizer3->Add( m_panel3, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pTreeNavegacion->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( DialogoConfiguracionBase::OnCambioNodoNavegacion ), NULL, this );
	m_button12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnExportarClick ), NULL, this );
	m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnImportarClick ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnAceptarClick ), NULL, this );
	m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnCancelarClick ), NULL, this );
	m_pBAplicar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnAplicarClick ), NULL, this );
}

DialogoConfiguracionBase::~DialogoConfiguracionBase()
{
	// Disconnect Events
	m_pTreeNavegacion->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( DialogoConfiguracionBase::OnCambioNodoNavegacion ), NULL, this );
	m_button12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnExportarClick ), NULL, this );
	m_button13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnImportarClick ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnAceptarClick ), NULL, this );
	m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnCancelarClick ), NULL, this );
	m_pBAplicar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoConfiguracionBase::OnAplicarClick ), NULL, this );
	
}

PanelConfiguracionEstacionBase::PanelConfiguracionEstacionBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 450,-1 ) );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_panelPhysician = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelPhysician->SetToolTip( _("Physician") );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_panelPhysician, wxID_ANY, _("Physician's name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer2->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pMedicoNombre = new wxTextCtrl( m_panelPhysician, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_pMedicoNombre->SetMaxLength( 0 ); 
	fgSizer2->Add( m_pMedicoNombre, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxBOTTOM|wxLEFT, 5 );
	
	m_staticText11 = new wxStaticText( m_panelPhysician, wxID_ANY, _("Physician's Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pMedicoId = new wxTextCtrl( m_panelPhysician, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pMedicoId->SetMaxLength( 0 ); 
	fgSizer2->Add( m_pMedicoId, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxLEFT, 5 );
	
	
	m_panelPhysician->SetSizer( fgSizer2 );
	m_panelPhysician->Layout();
	fgSizer2->Fit( m_panelPhysician );
	bSizer4->Add( m_panelPhysician, 0, wxEXPAND, 5 );
	
	m_pSep11 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer4->Add( m_pSep11, 0, wxEXPAND, 5 );
	
	m_panelServicio = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelServicio->SetToolTip( _("Service") );
	
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer22->AddGrowableCol( 1 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText39 = new wxStaticText( m_panelServicio, wxID_ANY, _("Service's name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer22->Add( m_staticText39, 0, wxTOP|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pServicioNombre = new wxTextCtrl( m_panelServicio, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pServicioNombre->SetMaxLength( 0 ); 
	fgSizer22->Add( m_pServicioNombre, 0, wxTOP|wxLEFT|wxEXPAND, 5 );
	
	m_staticText12 = new wxStaticText( m_panelServicio, wxID_ANY, _("Service's Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer22->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pServicioId = new wxTextCtrl( m_panelServicio, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_pServicioId->SetMaxLength( 0 ); 
	fgSizer22->Add( m_pServicioId, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	m_panelServicio->SetSizer( fgSizer22 );
	m_panelServicio->Layout();
	fgSizer22->Fit( m_panelServicio );
	bSizer4->Add( m_panelServicio, 0, wxEXPAND, 5 );
	
	m_pSep1 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer4->Add( m_pSep1, 0, wxEXPAND, 5 );
	
	m_pPanelInstitucion = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelInstitucion->SetToolTip( _("Local institution") );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->AddGrowableCol( 1 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText13 = new wxStaticText( m_pPanelInstitucion, wxID_ANY, _("Institution's name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer21->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pCentroNombre = new wxTextCtrl( m_pPanelInstitucion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_pCentroNombre->SetMaxLength( 0 ); 
	fgSizer21->Add( m_pCentroNombre, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxBOTTOM|wxLEFT, 5 );
	
	m_staticText111 = new wxStaticText( m_pPanelInstitucion, wxID_ANY, _("Institution's Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer21->Add( m_staticText111, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	m_pCentroId = new wxTextCtrl( m_pPanelInstitucion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pCentroId->SetMaxLength( 0 ); 
	fgSizer21->Add( m_pCentroId, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxLEFT, 5 );
	
	
	m_pPanelInstitucion->SetSizer( fgSizer21 );
	m_pPanelInstitucion->Layout();
	fgSizer21->Fit( m_pPanelInstitucion );
	bSizer4->Add( m_pPanelInstitucion, 0, wxEXPAND, 5 );
	
	m_pSep12 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer4->Add( m_pSep12, 0, wxEXPAND, 5 );
	
	m_pPanelInstitucion1 = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelInstitucion1->SetToolTip( _("Dicomized files") );
	
	wxFlexGridSizer* fgSizer211;
	fgSizer211 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer211->AddGrowableCol( 1 );
	fgSizer211->SetFlexibleDirection( wxBOTH );
	fgSizer211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText131 = new wxStaticText( m_pPanelInstitucion1, wxID_ANY, _("Charset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	fgSizer211->Add( m_staticText131, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );
	
	m_pComboCharset = new wxComboBox( m_pPanelInstitucion1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_pComboCharset->Append( _("ISO_IR 192") );
	m_pComboCharset->Append( _("ISO_IR 6") );
	m_pComboCharset->Append( _("ISO_IR 100") );
	m_pComboCharset->Append( _("ISO_IR 101") );
	m_pComboCharset->Append( _("ISO_IR 109") );
	m_pComboCharset->Append( _("ISO_IR 110") );
	m_pComboCharset->Append( _("ISO_IR 126") );
	m_pComboCharset->Append( _("ISO_IR 127") );
	m_pComboCharset->Append( _("ISO_IR 138") );
	m_pComboCharset->Append( _("ISO_IR 144") );
	m_pComboCharset->Append( _("ISO_IR 148") );
	fgSizer211->Add( m_pComboCharset, 1, wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	m_pPanelInstitucion1->SetSizer( fgSizer211 );
	m_pPanelInstitucion1->Layout();
	fgSizer211->Fit( m_pPanelInstitucion1 );
	bSizer4->Add( m_pPanelInstitucion1, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	bSizer4->Fit( this );
	
	// Connect Events
	m_pMedicoNombre->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pMedicoId->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pServicioId->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pCentroNombre->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pCentroId->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pComboCharset->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
}

PanelConfiguracionEstacionBase::~PanelConfiguracionEstacionBase()
{
	// Disconnect Events
	m_pMedicoNombre->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pMedicoId->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pServicioId->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pCentroNombre->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pCentroId->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	m_pComboCharset->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( PanelConfiguracionEstacionBase::OnTextoCambiado ), NULL, this );
	
}

PanelConfiguracionGeneralBase::PanelConfiguracionGeneralBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 450,-1 ) );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_pLanguage = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pLanguage->SetToolTip( _("Language") );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText34 = new wxStaticText( m_pLanguage, wxID_ANY, _("Language"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	bSizer32->Add( m_staticText34, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pComboLanguage = new wxComboBox( m_pLanguage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	bSizer32->Add( m_pComboLanguage, 0, wxLEFT, 5 );
	
	
	m_pLanguage->SetSizer( bSizer32 );
	m_pLanguage->Layout();
	bSizer32->Fit( m_pLanguage );
	bSizer4->Add( m_pLanguage, 0, wxEXPAND, 5 );
	
	m_pSep3 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer4->Add( m_pSep3, 0, wxEXPAND, 5 );
	
	m_pUpdate = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pUpdate->SetToolTip( _("Misc") );
	
	wxBoxSizer* bSizer321;
	bSizer321 = new wxBoxSizer( wxVERTICAL );
	
	m_pCheckForUpdates = new wxCheckBox( m_pUpdate, wxID_ANY, _("Automatically check for updates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pCheckForUpdates->SetValue(true); 
	bSizer321->Add( m_pCheckForUpdates, 0, 0, 5 );
	
	m_pCheckAnonUsageStats = new wxCheckBox( m_pUpdate, wxID_ANY, _("Help us sending anonymous usage data (no patient data will be sent)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pCheckAnonUsageStats->SetValue(true); 
	bSizer321->Add( m_pCheckAnonUsageStats, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	m_pUpdate->SetSizer( bSizer321 );
	m_pUpdate->Layout();
	bSizer321->Fit( m_pUpdate );
	bSizer4->Add( m_pUpdate, 0, wxEXPAND, 5 );
	
	m_pSep31 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer4->Add( m_pSep31, 0, wxEXPAND, 5 );
	
	m_pProxy = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pProxy->SetToolTip( _("Proxy settings") );
	
	wxBoxSizer* bSizer3211;
	bSizer3211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pEnableProxy = new wxCheckBox( m_pProxy, wxID_ANY, _("Enable http proxy"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3211->Add( m_pEnableProxy, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pProxySettingsButton = new wxButton( m_pProxy, wxID_ANY, _("Proxy settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pProxySettingsButton->Enable( false );
	
	bSizer3211->Add( m_pProxySettingsButton, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_pProxy->SetSizer( bSizer3211 );
	m_pProxy->Layout();
	bSizer3211->Fit( m_pProxy );
	bSizer4->Add( m_pProxy, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	bSizer4->Fit( this );
	
	// Connect Events
	m_pComboLanguage->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnIdiomaCambiado ), NULL, this );
	m_pCheckForUpdates->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnCheckForUpdates ), NULL, this );
	m_pCheckAnonUsageStats->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnCheckForUpdates ), NULL, this );
	m_pEnableProxy->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnCheckEnableProxySettings ), NULL, this );
	m_pProxySettingsButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnProxySettings ), NULL, this );
}

PanelConfiguracionGeneralBase::~PanelConfiguracionGeneralBase()
{
	// Disconnect Events
	m_pComboLanguage->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnIdiomaCambiado ), NULL, this );
	m_pCheckForUpdates->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnCheckForUpdates ), NULL, this );
	m_pCheckAnonUsageStats->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnCheckForUpdates ), NULL, this );
	m_pEnableProxy->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnCheckEnableProxySettings ), NULL, this );
	m_pProxySettingsButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionGeneralBase::OnProxySettings ), NULL, this );
	
}

ProxySettingsDialogBase::ProxySettingsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,500 ), wxDefaultSize );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Proxy Settings") );
	
	bSizer11->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer85;
	bSizer85 = new wxBoxSizer( wxVERTICAL );
	
	m_panel64 = new SubTitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel64->SetToolTip( _("Server settings") );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pstaticText1 = new wxStaticText( m_panel64, wxID_ANY, _("Host"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstaticText1->Wrap( -1 );
	fgSizer4->Add( m_pstaticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pHost = new wxTextCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_pHost->SetMaxLength( 0 ); 
	fgSizer4->Add( m_pHost, 0, wxALL|wxEXPAND, 5 );
	
	m_pStaticText22 = new wxStaticText( m_panel64, wxID_ANY, _("Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pStaticText22->Wrap( -1 );
	fgSizer4->Add( m_pStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pPort = new wxTextCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_pPort->SetMaxLength( 0 ); 
	fgSizer4->Add( m_pPort, 0, wxALL|wxEXPAND, 5 );
	
	
	m_panel64->SetSizer( fgSizer4 );
	m_panel64->Layout();
	fgSizer4->Fit( m_panel64 );
	bSizer85->Add( m_panel64, 0, wxEXPAND, 5 );
	
	m_panel641 = new SubTitledPanelSeparatorV( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel641->SetToolTip( _("Server settings") );
	
	bSizer85->Add( m_panel641, 0, wxEXPAND, 5 );
	
	m_panel642 = new SubTitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel642->SetToolTip( _("Authentication") );
	
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	m_pCheckAuthentication = new wxCheckBox( m_panel642, wxID_ANY, _("Proxy server requires authentication"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer141->Add( m_pCheckAuthentication, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer42;
	fgSizer42 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer42->AddGrowableCol( 1 );
	fgSizer42->SetFlexibleDirection( wxBOTH );
	fgSizer42->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pstaticText12 = new wxStaticText( m_panel642, wxID_ANY, _("Username"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstaticText12->Wrap( -1 );
	fgSizer42->Add( m_pstaticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pUsername = new wxTextCtrl( m_panel642, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_pUsername->SetMaxLength( 0 ); 
	fgSizer42->Add( m_pUsername, 0, wxALL|wxEXPAND, 5 );
	
	Port2 = new wxStaticText( m_panel642, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
	Port2->Wrap( -1 );
	fgSizer42->Add( Port2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pPassword = new wxTextCtrl( m_panel642, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_PASSWORD );
	m_pPassword->SetMaxLength( 0 ); 
	fgSizer42->Add( m_pPassword, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer141->Add( fgSizer42, 1, wxEXPAND, 5 );
	
	
	m_panel642->SetSizer( bSizer141 );
	m_panel642->Layout();
	bSizer141->Fit( m_panel642 );
	bSizer85->Add( m_panel642, 0, wxEXPAND, 5 );
	
	m_panel6411 = new SubTitledPanelSeparatorV( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6411->SetToolTip( _("Server settings") );
	
	bSizer85->Add( m_panel6411, 0, wxEXPAND, 5 );
	
	m_panel6421 = new SubTitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6421->SetToolTip( _("Exceptions") );
	
	wxBoxSizer* bSizer1411;
	bSizer1411 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText115 = new wxStaticText( m_panel6421, wxID_ANY, _("No proxy for addresses containing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	bSizer1411->Add( m_staticText115, 0, wxTOP|wxBOTTOM, 5 );
	
	m_pExceptions = new wxTextCtrl( m_panel6421, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1411->Add( m_pExceptions, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	m_staticText116 = new wxStaticText( m_panel6421, wxID_ANY, _("Separate addresses with commas."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText116->Wrap( -1 );
	bSizer1411->Add( m_staticText116, 0, wxTOP|wxBOTTOM, 5 );
	
	
	m_panel6421->SetSizer( bSizer1411 );
	m_panel6421->Layout();
	bSizer1411->Fit( m_panel6421 );
	bSizer85->Add( m_panel6421, 0, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer85 );
	m_pBody->Layout();
	bSizer85->Fit( m_pBody );
	bSizer11->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button37 = new wxButton( m_pFooter, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button37->SetDefault(); 
	bSizer31->Add( m_button37, 0, wxALL, 5 );
	
	m_button38 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button38, 0, wxALL, 5 );
	
	
	m_pFooter->SetSizer( bSizer31 );
	m_pFooter->Layout();
	bSizer31->Fit( m_pFooter );
	bSizer11->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer11 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pCheckAuthentication->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ProxySettingsDialogBase::OnCheckAuthentication ), NULL, this );
	m_button37->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProxySettingsDialogBase::OnOkClick ), NULL, this );
	m_button38->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProxySettingsDialogBase::OnCancelClick ), NULL, this );
}

ProxySettingsDialogBase::~ProxySettingsDialogBase()
{
	// Disconnect Events
	m_pCheckAuthentication->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ProxySettingsDialogBase::OnCheckAuthentication ), NULL, this );
	m_button37->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProxySettingsDialogBase::OnOkClick ), NULL, this );
	m_button38->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProxySettingsDialogBase::OnCancelClick ), NULL, this );
	
}

LocalDatabaseConfigurationPanelBase::LocalDatabaseConfigurationPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelDicomDir = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelDicomDir->SetToolTip( _("DICOM Dir") );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextExplicacion = new wxStaticText( m_pPanelDicomDir, wxID_ANY, _("DICOM Dir is the local directory where history is stored"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextExplicacion->Wrap( 440 );
	bSizer30->Add( m_staticTextExplicacion, 0, wxEXPAND|wxBOTTOM|wxRIGHT, 5 );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer9->AddGrowableCol( 1 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText32 = new wxStaticText( m_pPanelDicomDir, wxID_ANY, _("DICOM Dir"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer9->Add( m_staticText32, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_pDICOMDir = new wxTextCtrl( m_pPanelDicomDir, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pDICOMDir->SetMaxLength( 0 ); 
	fgSizer9->Add( m_pDICOMDir, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_buttonSeleccionarDICOMDir = new wxButton( m_pPanelDicomDir, wxID_ANY, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	fgSizer9->Add( m_buttonSeleccionarDICOMDir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer30->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	m_pstaticTextAdvertencia = new wxStaticText( m_pPanelDicomDir, wxID_ANY, _("Warning, to update the directory, you have to close every opened series"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstaticTextAdvertencia->Wrap( 440 );
	m_pstaticTextAdvertencia->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_pstaticTextAdvertencia->SetForegroundColour( wxColour( 210, 0, 0 ) );
	
	bSizer30->Add( m_pstaticTextAdvertencia, 0, wxEXPAND|wxTOP, 5 );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_pPanelDicomDir, wxID_ANY, _("Auto-cleaning") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer20->AddGrowableCol( 0 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pCheckDeleteOlder = new wxCheckBox( m_pPanelDicomDir, wxID_ANY, _("Delete studies with acquisition date older than:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer20->Add( m_pCheckDeleteOlder, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_pChoiceDeleteOlderChoices;
	m_pChoiceDeleteOlder = new wxChoice( m_pPanelDicomDir, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pChoiceDeleteOlderChoices, 0 );
	m_pChoiceDeleteOlder->SetSelection( 0 );
	m_pChoiceDeleteOlder->Enable( false );
	
	fgSizer20->Add( m_pChoiceDeleteOlder, 0, wxALL|wxEXPAND, 5 );
	
	m_pCheckHDFreeSpace = new wxCheckBox( m_pPanelDicomDir, wxID_ANY, _("Delete older studies when hd free space is less than:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer20->Add( m_pCheckHDFreeSpace, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer106;
	bSizer106 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pSpinHDSpace = new wxSpinCtrl( m_pPanelDicomDir, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 2147483647, 10240 );
	m_pSpinHDSpace->Enable( false );
	
	bSizer106->Add( m_pSpinHDSpace, 0, wxALL, 5 );
	
	m_staticText65 = new wxStaticText( m_pPanelDicomDir, wxID_ANY, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText65->Wrap( -1 );
	bSizer106->Add( m_staticText65, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	fgSizer20->Add( bSizer106, 1, wxEXPAND, 5 );
	
	m_pCheckDicomDirSize = new wxCheckBox( m_pPanelDicomDir, wxID_ANY, _("Delete studies when DICOM Dir size bigger than:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer20->Add( m_pCheckDicomDirSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer1061;
	bSizer1061 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pSpinDicomDirSize = new wxSpinCtrl( m_pPanelDicomDir, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 2147483647, 10240 );
	m_pSpinDicomDirSize->Enable( false );
	
	bSizer1061->Add( m_pSpinDicomDirSize, 0, wxALL, 5 );
	
	m_staticText651 = new wxStaticText( m_pPanelDicomDir, wxID_ANY, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText651->Wrap( -1 );
	bSizer1061->Add( m_staticText651, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	fgSizer20->Add( bSizer1061, 1, wxEXPAND, 5 );
	
	
	sbSizer7->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	m_staticText68 = new wxStaticText( m_pPanelDicomDir, wxID_ANY, _("This feature only affects to studies COPIED to DICOM Dir and not in use"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText68->Wrap( -1 );
	m_staticText68->SetFont( wxFont( 7, 70, 90, 90, false, wxEmptyString ) );
	
	sbSizer7->Add( m_staticText68, 0, wxALL, 5 );
	
	
	bSizer30->Add( sbSizer7, 0, wxEXPAND|wxTOP, 5 );
	
	m_button66 = new wxButton( m_pPanelDicomDir, wxID_ANY, _("Reset to default"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( m_button66, 0, wxALIGN_RIGHT|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	m_pPanelDicomDir->SetSizer( bSizer30 );
	m_pPanelDicomDir->Layout();
	bSizer30->Fit( m_pPanelDicomDir );
	bSizer4->Add( m_pPanelDicomDir, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( LocalDatabaseConfigurationPanelBase::OnSize ) );
	m_pDICOMDir->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnTextoCambiado ), NULL, this );
	m_buttonSeleccionarDICOMDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnSeleccionarDirectorio ), NULL, this );
	m_pCheckDeleteOlder->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnCheckOlderStudies ), NULL, this );
	m_pChoiceDeleteOlder->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnOptionModified ), NULL, this );
	m_pCheckHDFreeSpace->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnCheckHDSpace ), NULL, this );
	m_pSpinHDSpace->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocalDatabaseConfigurationPanelBase::OnSpinCtrl ), NULL, this );
	m_pSpinHDSpace->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnOptionModified ), NULL, this );
	m_pCheckDicomDirSize->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnCheckDicomDirSize ), NULL, this );
	m_pSpinDicomDirSize->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocalDatabaseConfigurationPanelBase::OnSpinCtrl ), NULL, this );
	m_pSpinDicomDirSize->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnOptionModified ), NULL, this );
	m_button66->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnResetToDefault ), NULL, this );
}

LocalDatabaseConfigurationPanelBase::~LocalDatabaseConfigurationPanelBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( LocalDatabaseConfigurationPanelBase::OnSize ) );
	m_pDICOMDir->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnTextoCambiado ), NULL, this );
	m_buttonSeleccionarDICOMDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnSeleccionarDirectorio ), NULL, this );
	m_pCheckDeleteOlder->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnCheckOlderStudies ), NULL, this );
	m_pChoiceDeleteOlder->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnOptionModified ), NULL, this );
	m_pCheckHDFreeSpace->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnCheckHDSpace ), NULL, this );
	m_pSpinHDSpace->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocalDatabaseConfigurationPanelBase::OnSpinCtrl ), NULL, this );
	m_pSpinHDSpace->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnOptionModified ), NULL, this );
	m_pCheckDicomDirSize->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnCheckDicomDirSize ), NULL, this );
	m_pSpinDicomDirSize->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocalDatabaseConfigurationPanelBase::OnSpinCtrl ), NULL, this );
	m_pSpinDicomDirSize->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnOptionModified ), NULL, this );
	m_button66->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocalDatabaseConfigurationPanelBase::OnResetToDefault ), NULL, this );
	
}

PanelConfiguracionPACSBase::PanelConfiguracionPACSBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pPACSLocal = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPACSLocal->SetToolTip( _("Local PACS") );
	
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextExplicacion = new wxStaticText( m_pPACSLocal, wxID_ANY, _("You must configure the local AET, which is used to communicate with the PACS server. The specified port must be opened in the firewall, as it is essential for transfers."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextExplicacion->Wrap( 440 );
	m_staticTextExplicacion->SetMinSize( wxSize( 450,-1 ) );
	
	bSizer36->Add( m_staticTextExplicacion, 0, wxEXPAND|wxBOTTOM, 8 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_pPACSLocal, wxID_ANY, _("Local AET"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer2->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pAETLocal = new wxTextCtrl( m_pPACSLocal, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	m_pAETLocal->SetMaxLength( 0 ); 
	bSizer31->Add( m_pAETLocal, 0, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pAETReal = new wxStaticText( m_pPACSLocal, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pAETReal->Wrap( -1 );
	bSizer31->Add( m_pAETReal, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pHelpBitmap = new wxStaticBitmap( m_pPACSLocal, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_pHelpBitmap->SetToolTip( _("For large-scale implementation in a homogenous environment, you can specify the following wildcards in the local AET:\n\t%IP IP Address\n\t%IP<n> n-th Octet (1~4)\n\t%HOSTNAME Host Name\"") );
	
	bSizer31->Add( m_pHelpBitmap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( bSizer31, 1, wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( m_pPACSLocal, wxID_ANY, _("Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pPuertoLocal = new wxTextCtrl( m_pPACSLocal, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pPuertoLocal->SetMaxLength( 0 ); 
	fgSizer2->Add( m_pPuertoLocal, 1, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pBAdvanced = new wxButton( m_pPACSLocal, wxID_ANY, _("Advanced settings..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_pBAdvanced, 0, wxALIGN_RIGHT|wxTOP, 5 );
	
	
	bSizer36->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	
	m_pPACSLocal->SetSizer( bSizer36 );
	m_pPACSLocal->Layout();
	bSizer36->Fit( m_pPACSLocal );
	bSizer6->Add( m_pPACSLocal, 0, wxEXPAND, 5 );
	
	m_pSep = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer6->Add( m_pSep, 0, wxEXPAND, 5 );
	
	m_pPACSServers = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPACSServers->SetToolTip( _("DICOM Nodes") );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListPacs = new wxDataViewListCtrl(m_pPACSServers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	bSizer25->Add( m_pListPacs, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_button3 = new wxButton( m_pPACSServers, wxID_ANY, _("New ..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_button3, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pEditButton = new wxButton( m_pPACSServers, wxID_ANY, _("Edit ..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pEditButton->Enable( false );
	
	bSizer26->Add( m_pEditButton, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pDelButton = new wxButton( m_pPACSServers, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pDelButton->Enable( false );
	
	bSizer26->Add( m_pDelButton, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer26->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pDefaultButton = new wxButton( m_pPACSServers, wxID_ANY, _("Set Default"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pDefaultButton->Enable( false );
	
	bSizer26->Add( m_pDefaultButton, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer25->Add( bSizer26, 0, wxEXPAND, 5 );
	
	
	m_pPACSServers->SetSizer( bSizer25 );
	m_pPACSServers->Layout();
	bSizer25->Fit( m_pPACSServers );
	bSizer6->Add( m_pPACSServers, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionPACSBase::OnSize ) );
	m_pAETLocal->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnTextoCambiado ), NULL, this );
	m_pHelpBitmap->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( PanelConfiguracionPACSBase::OnHelpClick ), NULL, this );
	m_pPuertoLocal->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnTextoCambiado ), NULL, this );
	m_pBAdvanced->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnAdvancedLocal ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnNuevoClick ), NULL, this );
	m_pEditButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnEditarClick ), NULL, this );
	m_pDelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnEliminarClick ), NULL, this );
	m_pDefaultButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnSetDefaultClick ), NULL, this );
}

PanelConfiguracionPACSBase::~PanelConfiguracionPACSBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionPACSBase::OnSize ) );
	m_pAETLocal->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnTextoCambiado ), NULL, this );
	m_pHelpBitmap->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( PanelConfiguracionPACSBase::OnHelpClick ), NULL, this );
	m_pPuertoLocal->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnTextoCambiado ), NULL, this );
	m_pBAdvanced->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnAdvancedLocal ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnNuevoClick ), NULL, this );
	m_pEditButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnEditarClick ), NULL, this );
	m_pDelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnEliminarClick ), NULL, this );
	m_pDefaultButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPACSBase::OnSetDefaultClick ), NULL, this );
	
}

DialogoServidorPACSBase::DialogoServidorPACSBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 450,550 ), wxDefaultSize );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("DICOM Node") );
	
	bSizer11->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel6 = new BodyPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer85;
	bSizer85 = new wxBoxSizer( wxVERTICAL );
	
	m_panel64 = new SubTitledPanel( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel64->SetToolTip( _("Settings") );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pstaticText1 = new wxStaticText( m_panel64, wxID_ANY, _("Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstaticText1->Wrap( -1 );
	fgSizer4->Add( m_pstaticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pId = new wxTextCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_pId->SetMaxLength( 0 ); 
	fgSizer4->Add( m_pId, 0, wxALL|wxEXPAND, 5 );
	
	m_pstaticText2 = new wxStaticText( m_panel64, wxID_ANY, _("AET"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstaticText2->Wrap( -1 );
	fgSizer4->Add( m_pstaticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pAET = new wxTextCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_pAET->SetMaxLength( 0 ); 
	fgSizer4->Add( m_pAET, 0, wxALL|wxEXPAND, 5 );
	
	m_pstaticText3 = new wxStaticText( m_panel64, wxID_ANY, _("Host"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstaticText3->Wrap( -1 );
	fgSizer4->Add( m_pstaticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pHost = new wxTextCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_pHost->SetMaxLength( 0 ); 
	fgSizer4->Add( m_pHost, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_pstaticText4 = new wxStaticText( m_panel64, wxID_ANY, _("Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstaticText4->Wrap( -1 );
	fgSizer4->Add( m_pstaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pPuerto = new wxTextCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pPuerto->SetMaxLength( 0 ); 
	fgSizer4->Add( m_pPuerto, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText33 = new wxStaticText( m_panel64, wxID_ANY, _("PDU (KB)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer4->Add( m_staticText33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pPDU = new wxSpinCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 4096, 16384, 16384 );
	fgSizer4->Add( m_pPDU, 0, wxALL, 5 );
	
	m_staticText48 = new wxStaticText( m_panel64, wxID_ANY, _("Retrieve method"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer4->Add( m_staticText48, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_pRetrieveMethodChoices[] = { _("Move"), _("Get"), _("WADO") };
	int m_pRetrieveMethodNChoices = sizeof( m_pRetrieveMethodChoices ) / sizeof( wxString );
	m_pRetrieveMethod = new wxChoice( m_panel64, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pRetrieveMethodNChoices, m_pRetrieveMethodChoices, 0 );
	m_pRetrieveMethod->SetSelection( 0 );
	fgSizer4->Add( m_pRetrieveMethod, 0, wxALL, 5 );
	
	m_staticText59 = new wxStaticText( m_panel64, wxID_ANY, _("WADO URL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer4->Add( m_staticText59, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pWadoURL = new wxTextCtrl( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pWadoURL->SetMaxLength( 0 ); 
	m_pWadoURL->Enable( false );
	
	fgSizer4->Add( m_pWadoURL, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText60 = new wxStaticText( m_panel64, wxID_ANY, _("Retrieve mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	fgSizer4->Add( m_staticText60, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_pRetrieveModeChoices[] = { _("Series"), _("Studies") };
	int m_pRetrieveModeNChoices = sizeof( m_pRetrieveModeChoices ) / sizeof( wxString );
	m_pRetrieveMode = new wxChoice( m_panel64, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pRetrieveModeNChoices, m_pRetrieveModeChoices, 0 );
	m_pRetrieveMode->SetSelection( 0 );
	fgSizer4->Add( m_pRetrieveMode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText57 = new wxStaticText( m_panel64, wxID_ANY, _("Reuse connection"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer4->Add( m_staticText57, 0, wxALL, 5 );
	
	m_pReuseConnection = new wxCheckBox( m_panel64, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pReuseConnection->SetToolTip( _("Reuse connections when is needed to make more than one queries") );
	
	fgSizer4->Add( m_pReuseConnection, 0, wxALL, 5 );
	
	
	m_panel64->SetSizer( fgSizer4 );
	m_panel64->Layout();
	fgSizer4->Fit( m_panel64 );
	bSizer85->Add( m_panel64, 0, wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer85 );
	m_panel6->Layout();
	bSizer85->Fit( m_panel6 );
	m_notebook1->AddPage( m_panel6, _("DICOM Node"), true );
	m_pWorkListPanel = new BodyPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer( wxVERTICAL );
	
	m_panel83 = new SubTitledPanel( m_pWorkListPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel83->SetToolTip( _("Worklists") );
	
	wxBoxSizer* bSizer106;
	bSizer106 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText611 = new wxStaticText( m_panel83, wxID_ANY, _("Select how do you want to handle MPPS items"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText611->Wrap( -1 );
	bSizer106->Add( m_staticText611, 0, wxALL, 5 );
	
	m_pCheckDICOMMPPS = new wxCheckBox( m_panel83, wxID_ANY, _("DICOM MPPS"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pCheckDICOMMPPS->SetValue(true); 
	bSizer106->Add( m_pCheckDICOMMPPS, 0, wxALL, 5 );
	
	m_pCheckHL7 = new wxCheckBox( m_panel83, wxID_ANY, _("HL7 messages"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer106->Add( m_pCheckHL7, 0, wxALL, 5 );
	
	m_pPanelHL7 = new wxPanel( m_panel83, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pSizerHL7 = new wxStaticBoxSizer( new wxStaticBox( m_pPanelHL7, wxID_ANY, _("HL7") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer18->AddGrowableCol( 1 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pCheckAbort = new wxCheckBox( m_pPanelHL7, wxID_ANY, _("Abort message"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pCheckAbort, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pTextAbort = new wxTextCtrl( m_pPanelHL7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_pTextAbort->SetMaxLength( 0 ); 
	fgSizer18->Add( m_pTextAbort, 1, wxALL|wxEXPAND, 5 );
	
	m_pCheckCompleted = new wxCheckBox( m_pPanelHL7, wxID_ANY, _("Completed message"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pCheckCompleted, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pTextCompleted = new wxTextCtrl( m_pPanelHL7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_pTextCompleted->SetMaxLength( 0 ); 
	fgSizer18->Add( m_pTextCompleted, 0, wxALL|wxEXPAND, 5 );
	
	m_pCheckInProgress = new wxCheckBox( m_pPanelHL7, wxID_ANY, _("In progress message"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pCheckInProgress, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pTextProgress = new wxTextCtrl( m_pPanelHL7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_pTextProgress->SetMaxLength( 0 ); 
	fgSizer18->Add( m_pTextProgress, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button62 = new wxButton( m_pPanelHL7, wxID_ANY, _("Default values"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_button62, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	
	m_pSizerHL7->Add( fgSizer18, 1, wxEXPAND, 5 );
	
	
	m_pPanelHL7->SetSizer( m_pSizerHL7 );
	m_pPanelHL7->Layout();
	m_pSizerHL7->Fit( m_pPanelHL7 );
	bSizer106->Add( m_pPanelHL7, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel83->SetSizer( bSizer106 );
	m_panel83->Layout();
	bSizer106->Fit( m_panel83 );
	bSizer104->Add( m_panel83, 0, wxEXPAND, 5 );
	
	
	m_pWorkListPanel->SetSizer( bSizer104 );
	m_pWorkListPanel->Layout();
	bSizer104->Fit( m_pWorkListPanel );
	m_notebook1->AddPage( m_pWorkListPanel, _("Worklist"), false );
	m_pPanelSeguridad = new BodyPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	m_pSSLTLS = new SubTitledPanel( m_pPanelSeguridad, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pSSLTLS->SetToolTip( _("SSL/TLS") );
	
	wxBoxSizer* bSizer87;
	bSizer87 = new wxBoxSizer( wxVERTICAL );
	
	m_pVerifyServer = new wxCheckBox( m_pSSLTLS, wxID_ANY, _("Verify server credentials"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer87->Add( m_pVerifyServer, 0, wxALL, 5 );
	
	m_pUseSSL = new wxCheckBox( m_pSSLTLS, wxID_ANY, _("Use SSL/TLS"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer87->Add( m_pUseSSL, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_pSSLTLS, wxID_ANY, _("Certificate") ), wxHORIZONTAL );
	
	m_staticText55 = new wxStaticText( m_pSSLTLS, wxID_ANY, _("Certificate to use in this connection"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	sbSizer6->Add( m_staticText55, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pBCert = new wxButton( m_pSSLTLS, wxID_ANY, _("Advanced Configuration ..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_pBCert, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer87->Add( sbSizer6, 0, wxEXPAND|wxTOP, 5 );
	
	
	m_pSSLTLS->SetSizer( bSizer87 );
	m_pSSLTLS->Layout();
	bSizer87->Fit( m_pSSLTLS );
	bSizer81->Add( m_pSSLTLS, 0, wxEXPAND, 5 );
	
	m_panelSep = new SubTitledPanelSeparatorV( m_pPanelSeguridad, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer81->Add( m_panelSep, 0, wxEXPAND, 5 );
	
	m_panelUserPass = new SubTitledPanel( m_pPanelSeguridad, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelUserPass->SetToolTip( _("User/Password") );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->AddGrowableCol( 1 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText61 = new wxStaticText( m_panelUserPass, wxID_ANY, _("User"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer21->Add( m_staticText61, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pUser = new wxTextCtrl( m_panelUserPass, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pUser->SetMaxLength( 0 ); 
	fgSizer21->Add( m_pUser, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_staticText62 = new wxStaticText( m_panelUserPass, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	fgSizer21->Add( m_staticText62, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPassword = new wxTextCtrl( m_panelUserPass, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_pPassword->SetMaxLength( 0 ); 
	fgSizer21->Add( m_pPassword, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	m_panelUserPass->SetSizer( fgSizer21 );
	m_panelUserPass->Layout();
	fgSizer21->Fit( m_panelUserPass );
	bSizer81->Add( m_panelUserPass, 0, wxEXPAND, 5 );
	
	m_panelSep1 = new SubTitledPanelSeparatorV( m_pPanelSeguridad, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer81->Add( m_panelSep1, 0, wxEXPAND, 5 );
	
	m_pPanelDefaultCharset = new SubTitledPanel( m_pPanelSeguridad, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelDefaultCharset->SetToolTip( _("Default charset") );
	
	wxFlexGridSizer* fgSizer211;
	fgSizer211 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer211->AddGrowableCol( 1 );
	fgSizer211->SetFlexibleDirection( wxBOTH );
	fgSizer211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText131 = new wxStaticText( m_pPanelDefaultCharset, wxID_ANY, _("Default charset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	fgSizer211->Add( m_staticText131, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );
	
	m_pComboCharset = new wxComboBox( m_pPanelDefaultCharset, wxID_ANY, _("ISO_IR 100"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_pComboCharset->Append( _("ISO_IR 192") );
	m_pComboCharset->Append( _("ISO_IR 6") );
	m_pComboCharset->Append( _("ISO_IR 100") );
	m_pComboCharset->Append( _("ISO_IR 101") );
	m_pComboCharset->Append( _("ISO_IR 109") );
	m_pComboCharset->Append( _("ISO_IR 110") );
	m_pComboCharset->Append( _("ISO_IR 126") );
	m_pComboCharset->Append( _("ISO_IR 127") );
	m_pComboCharset->Append( _("ISO_IR 138") );
	m_pComboCharset->Append( _("ISO_IR 144") );
	m_pComboCharset->Append( _("ISO_IR 148") );
	fgSizer211->Add( m_pComboCharset, 1, wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	m_pPanelDefaultCharset->SetSizer( fgSizer211 );
	m_pPanelDefaultCharset->Layout();
	fgSizer211->Fit( m_pPanelDefaultCharset );
	bSizer81->Add( m_pPanelDefaultCharset, 0, wxEXPAND, 5 );
	
	
	m_pPanelSeguridad->SetSizer( bSizer81 );
	m_pPanelSeguridad->Layout();
	bSizer81->Fit( m_pPanelSeguridad );
	m_notebook1->AddPage( m_pPanelSeguridad, _("Advanced"), false );
	
	bSizer11->Add( m_notebook1, 1, wxEXPAND|wxTOP, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button20 = new wxButton( m_pFooter, wxID_ANY, _("Test connection"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button20, 0, wxALIGN_RIGHT|wxALL|wxBOTTOM|wxLEFT|wxTOP, 5 );
	
	
	bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button37 = new wxButton( m_pFooter, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button37, 0, wxALL, 5 );
	
	m_button38 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button38, 0, wxALL, 5 );
	
	
	m_pFooter->SetSizer( bSizer31 );
	m_pFooter->Layout();
	bSizer31->Fit( m_pFooter );
	bSizer11->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer11 );
	this->Layout();
	
	// Connect Events
	m_pRetrieveMethod->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogoServidorPACSBase::OnRetrieveMethodChoice ), NULL, this );
	m_pCheckHL7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_pCheckAbort->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_pCheckCompleted->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_pCheckInProgress->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_button62->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnMessageDefaultValues ), NULL, this );
	m_pBCert->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnAdvancedConfiguration ), NULL, this );
	m_pComboCharset->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoServidorPACSBase::OnTextoCambiado ), NULL, this );
	m_button20->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnProbarClick ), NULL, this );
	m_button37->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnOkClick ), NULL, this );
	m_button38->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnCancelClick ), NULL, this );
}

DialogoServidorPACSBase::~DialogoServidorPACSBase()
{
	// Disconnect Events
	m_pRetrieveMethod->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogoServidorPACSBase::OnRetrieveMethodChoice ), NULL, this );
	m_pCheckHL7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_pCheckAbort->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_pCheckCompleted->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_pCheckInProgress->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnChecWL ), NULL, this );
	m_button62->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnMessageDefaultValues ), NULL, this );
	m_pBCert->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnAdvancedConfiguration ), NULL, this );
	m_pComboCharset->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoServidorPACSBase::OnTextoCambiado ), NULL, this );
	m_button20->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnProbarClick ), NULL, this );
	m_button37->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnOkClick ), NULL, this );
	m_button38->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoServidorPACSBase::OnCancelClick ), NULL, this );
	
}

DialogoLocalPACSBase::DialogoLocalPACSBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 346,300 ), wxDefaultSize );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );
	
	m_panel51 = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer80;
	bSizer80 = new wxBoxSizer( wxVERTICAL );
	
	m_panel67 = new TitledPanel( m_panel51, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel67->SetToolTip( _("Security") );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	m_pVerifyServer = new wxCheckBox( m_panel67, wxID_ANY, _("Verify client credentials"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer81->Add( m_pVerifyServer, 0, wxALL, 5 );
	
	m_pUseTLS = new wxCheckBox( m_panel67, wxID_ANY, _("Use SSL/TLS"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer81->Add( m_pUseTLS, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panel67, wxID_ANY, _("Local AET certificate") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer36;
	fgSizer36 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer36->AddGrowableCol( 0 );
	fgSizer36->SetFlexibleDirection( wxBOTH );
	fgSizer36->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText55 = new wxStaticText( m_panel67, wxID_ANY, _("Certificate used by Ginkgo CADx"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	fgSizer36->Add( m_staticText55, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_pBCert = new wxButton( m_panel67, wxID_ANY, _("Import ..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer36->Add( m_pBCert, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	sbSizer5->Add( fgSizer36, 1, wxEXPAND, 5 );
	
	
	bSizer81->Add( sbSizer5, 0, wxEXPAND|wxTOP, 5 );
	
	
	m_panel67->SetSizer( bSizer81 );
	m_panel67->Layout();
	bSizer81->Fit( m_panel67 );
	bSizer80->Add( m_panel67, 1, wxEXPAND, 5 );
	
	
	m_panel51->SetSizer( bSizer80 );
	m_panel51->Layout();
	bSizer80->Fit( m_panel51 );
	bSizer71->Add( m_panel51, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button8 = new wxButton( m_pFooter, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_button8, 0, wxRIGHT, 5 );
	
	m_button9 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_button9, 0, wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( bSizer19 );
	m_pFooter->Layout();
	bSizer19->Fit( m_pFooter );
	bSizer71->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer71 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pBCert->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoLocalPACSBase::OnEditCertificate ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoLocalPACSBase::OnAceptarClick ), NULL, this );
}

DialogoLocalPACSBase::~DialogoLocalPACSBase()
{
	// Disconnect Events
	m_pBCert->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoLocalPACSBase::OnEditCertificate ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoLocalPACSBase::OnAceptarClick ), NULL, this );
	
}

PanelConfiguracionHCEBase::PanelConfiguracionHCEBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 450,-1 ) );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pHCE = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHCE->SetToolTip( _("Gingo CADx Application ID used to fill MSH.3 field") );
	
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->AddGrowableCol( 1 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pstIdOrganizacionReceptora = new wxStaticText( m_pHCE, wxID_ANY, _("Receiver organization Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstIdOrganizacionReceptora->Wrap( -1 );
	m_pstIdOrganizacionReceptora->SetToolTip( _("Unique identifier of the organization receiving the message.") );
	
	fgSizer21->Add( m_pstIdOrganizacionReceptora, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pIdOrganizacionReceptora = new wxTextCtrl( m_pHCE, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_pIdOrganizacionReceptora->SetMaxLength( 0 ); 
	m_pIdOrganizacionReceptora->SetToolTip( _("Unique identifier of the organization receiving the message.") );
	
	fgSizer21->Add( m_pIdOrganizacionReceptora, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pstIdSistemaPeticionario = new wxStaticText( m_pHCE, wxID_ANY, _("Petitioner System Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstIdSistemaPeticionario->Wrap( -1 );
	m_pstIdSistemaPeticionario->SetToolTip( _("Unique identifier of the requesting system.") );
	
	fgSizer21->Add( m_pstIdSistemaPeticionario, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pIdSistemaPeticionario = new wxTextCtrl( m_pHCE, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pIdSistemaPeticionario->SetMaxLength( 0 ); 
	m_pIdSistemaPeticionario->SetToolTip( _("Unique identifier of the requesting system.") );
	
	fgSizer21->Add( m_pIdSistemaPeticionario, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pstIdAplicacionRellenadora = new wxStaticText( m_pHCE, wxID_ANY, _("Filler Application Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstIdAplicacionRellenadora->Wrap( -1 );
	m_pstIdAplicacionRellenadora->SetToolTip( _("Unique identifier of the filler application (SIID/Ginkgo CADx).") );
	
	fgSizer21->Add( m_pstIdAplicacionRellenadora, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pIdAplicacionRellenadora = new wxTextCtrl( m_pHCE, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pIdAplicacionRellenadora->SetMaxLength( 0 ); 
	m_pIdAplicacionRellenadora->SetToolTip( _("Unique identifier of the filler application (SIID/Ginkgo).") );
	
	fgSizer21->Add( m_pIdAplicacionRellenadora, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pstIdAplicacionPACS = new wxStaticText( m_pHCE, wxID_ANY, _("Ginkgo CADx Application Id"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstIdAplicacionPACS->Wrap( -1 );
	m_pstIdAplicacionPACS->SetToolTip( _("Unique identifier of the Picture Archiving and Communication System Application (SIID/PACS).") );
	
	fgSizer21->Add( m_pstIdAplicacionPACS, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	m_pIdGinkgoAplication = new wxTextCtrl( m_pHCE, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pIdGinkgoAplication->SetMaxLength( 0 ); 
	m_pIdGinkgoAplication->SetToolTip( _("Unique identifier of the Picture Archiving and Communication System Application (SIID/PACS).") );
	
	fgSizer21->Add( m_pIdGinkgoAplication, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxLEFT, 5 );
	
	
	bSizer38->Add( fgSizer21, 1, wxEXPAND, 5 );
	
	
	m_pHCE->SetSizer( bSizer38 );
	m_pHCE->Layout();
	bSizer38->Fit( m_pHCE );
	bSizer6->Add( m_pHCE, 0, wxEXPAND, 5 );
	
	m_pSep = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer6->Add( m_pSep, 0, wxEXPAND, 5 );
	
	m_pIntegrationEngine = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pIntegrationEngine->SetToolTip( _("Integration engine") );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer20->AddGrowableCol( 1 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText121 = new wxStaticText( m_pIntegrationEngine, wxID_ANY, _("Hostname / Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	m_staticText121->SetToolTip( _("Hostname (IP or DNS) integration engine") );
	
	fgSizer20->Add( m_staticText121, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer74;
	bSizer74 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pHostnameMI = new wxTextCtrl( m_pIntegrationEngine, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_pHostnameMI->SetMaxLength( 0 ); 
	bSizer74->Add( m_pHostnameMI, 3, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxALL, 5 );
	
	m_staticText60 = new wxStaticText( m_pIntegrationEngine, wxID_ANY, _("/"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	bSizer74->Add( m_staticText60, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPuertoMI = new wxTextCtrl( m_pIntegrationEngine, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pPuertoMI->SetMaxLength( 0 ); 
	bSizer74->Add( m_pPuertoMI, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	fgSizer20->Add( bSizer74, 1, wxEXPAND, 5 );
	
	m_staticText38 = new wxStaticText( m_pIntegrationEngine, wxID_ANY, _("Protocol"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	m_staticText38->SetToolTip( _("Integration Engine's Transport Layer Protocol") );
	
	fgSizer20->Add( m_staticText38, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer75;
	bSizer75 = new wxBoxSizer( wxHORIZONTAL );
	
	wxString m_pProtocoloChoices[] = { _("MLLP") };
	int m_pProtocoloNChoices = sizeof( m_pProtocoloChoices ) / sizeof( wxString );
	m_pProtocolo = new wxChoice( m_pIntegrationEngine, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pProtocoloNChoices, m_pProtocoloChoices, 0 );
	m_pProtocolo->SetSelection( 0 );
	bSizer75->Add( m_pProtocolo, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );
	
	
	bSizer75->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_staticText39 = new wxStaticText( m_pIntegrationEngine, wxID_ANY, _("Format"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	m_staticText39->SetToolTip( _("Message's format") );
	
	bSizer75->Add( m_staticText39, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_pFormatoChoices[] = { _("ER7") };
	int m_pFormatoNChoices = sizeof( m_pFormatoChoices ) / sizeof( wxString );
	m_pFormato = new wxChoice( m_pIntegrationEngine, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pFormatoNChoices, m_pFormatoChoices, 0 );
	m_pFormato->SetSelection( 0 );
	bSizer75->Add( m_pFormato, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer75->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button15 = new wxButton( m_pIntegrationEngine, wxID_ANY, _("Test engine"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button15->SetToolTip( _("Test Integration Engine connectivity") );
	
	bSizer75->Add( m_button15, 0, wxTOP|wxLEFT|wxALIGN_RIGHT, 5 );
	
	
	fgSizer20->Add( bSizer75, 1, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer24->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	
	m_pIntegrationEngine->SetSizer( bSizer24 );
	m_pIntegrationEngine->Layout();
	bSizer24->Fit( m_pIntegrationEngine );
	bSizer6->Add( m_pIntegrationEngine, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionHCEBase::OnSize ) );
	m_pIdOrganizacionReceptora->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pIdSistemaPeticionario->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pIdAplicacionRellenadora->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pIdGinkgoAplication->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pHostnameMI->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pPuertoMI->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_button15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTestClick ), NULL, this );
}

PanelConfiguracionHCEBase::~PanelConfiguracionHCEBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionHCEBase::OnSize ) );
	m_pIdOrganizacionReceptora->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pIdSistemaPeticionario->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pIdAplicacionRellenadora->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pIdGinkgoAplication->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pHostnameMI->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_pPuertoMI->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTextoCambiado ), NULL, this );
	m_button15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionHCEBase::OnTestClick ), NULL, this );
	
}

PanelConfiguracionExtensionesBase::PanelConfiguracionExtensionesBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 450,-1 ) );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pExtensions = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pExtensions->SetToolTip( _("Extensions") );
	
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer85;
	bSizer85 = new wxBoxSizer( wxVERTICAL );
	
	m_pScrollPanelExtensions = new wxScrolledWindow( m_pExtensions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_pScrollPanelExtensions->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer84;
	bSizer84 = new wxBoxSizer( wxVERTICAL );
	
	
	m_pScrollPanelExtensions->SetSizer( bSizer84 );
	m_pScrollPanelExtensions->Layout();
	bSizer84->Fit( m_pScrollPanelExtensions );
	bSizer85->Add( m_pScrollPanelExtensions, 1, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	
	bSizer44->Add( bSizer85, 1, wxEXPAND, 5 );
	
	
	m_pExtensions->SetSizer( bSizer44 );
	m_pExtensions->Layout();
	bSizer44->Fit( m_pExtensions );
	bSizer6->Add( m_pExtensions, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionExtensionesBase::OnEraseBackground ) );
	m_pExtensions->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionExtensionesBase::OnEraseBackground ), NULL, this );
}

PanelConfiguracionExtensionesBase::~PanelConfiguracionExtensionesBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionExtensionesBase::OnEraseBackground ) );
	m_pExtensions->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionExtensionesBase::OnEraseBackground ), NULL, this );
	
}

ExtensionPanelBase::ExtensionPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : GinkgoBoxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer80;
	bSizer80 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pThumbnail = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_pThumbnail->SetMinSize( wxSize( 50,50 ) );
	
	bSizer80->Add( m_pThumbnail, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	m_pLabelTitle = new wxStaticText( this, wxID_ANY, _("Ginkgo Extension Title"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelTitle->Wrap( -1 );
	m_pLabelTitle->SetFont( wxFont( 12, 70, 90, 92, false, wxEmptyString ) );
	m_pLabelTitle->SetForegroundColour( wxColour( 63, 63, 63 ) );
	
	bSizer81->Add( m_pLabelTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer80->Add( bSizer81, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	
	this->SetSizer( bSizer80 );
	this->Layout();
	bSizer80->Fit( this );
}

ExtensionPanelBase::~ExtensionPanelBase()
{
}

DialogoEditarCertificadoBase::DialogoEditarCertificadoBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 450,550 ), wxDefaultSize );
	
	wxBoxSizer* bSizer83;
	bSizer83 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer86;
	bSizer86 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelDefault = new SubTitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelDefault->SetToolTip( _("Edit public key") );
	
	wxBoxSizer* bSizer872;
	bSizer872 = new wxBoxSizer( wxVERTICAL );
	
	m_pCheckDefault = new wxCheckBox( m_pPanelDefault, wxID_ANY, _("Use default certificate (certificate configured in local AET settings)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer872->Add( m_pCheckDefault, 0, 0, 5 );
	
	
	m_pPanelDefault->SetSizer( bSizer872 );
	m_pPanelDefault->Layout();
	bSizer872->Fit( m_pPanelDefault );
	bSizer86->Add( m_pPanelDefault, 0, wxEXPAND, 5 );
	
	m_pSepDefault = new SubTitledPanelSeparatorV( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer86->Add( m_pSepDefault, 0, wxEXPAND | wxALL, 5 );
	
	m_pPanelPublic = new SubTitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelPublic->SetToolTip( _("Public key") );
	
	wxBoxSizer* bSizer87;
	bSizer87 = new wxBoxSizer( wxVERTICAL );
	
	m_pLabel = new wxStaticText( m_pPanelPublic, wxID_ANY, _("Public key in PEM format"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	bSizer87->Add( m_pLabel, 0, wxBOTTOM|wxRIGHT, 5 );
	
	m_pTextoCertificado = new wxTextCtrl( m_pPanelPublic, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_pTextoCertificado->SetMaxLength( 0 ); 
	bSizer87->Add( m_pTextoCertificado, 1, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	m_button62 = new wxButton( m_pPanelPublic, wxID_ANY, _("Load ..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer87->Add( m_button62, 0, wxALIGN_RIGHT|wxTOP, 5 );
	
	
	m_pPanelPublic->SetSizer( bSizer87 );
	m_pPanelPublic->Layout();
	bSizer87->Fit( m_pPanelPublic );
	bSizer86->Add( m_pPanelPublic, 1, wxEXPAND, 5 );
	
	m_panel60 = new SubTitledPanelSeparatorV( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer86->Add( m_panel60, 0, wxEXPAND, 5 );
	
	m_pPanelPrivate = new SubTitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelPrivate->SetToolTip( _("Private key") );
	
	wxBoxSizer* bSizer871;
	bSizer871 = new wxBoxSizer( wxVERTICAL );
	
	m_pLabel11 = new wxStaticText( m_pPanelPrivate, wxID_ANY, _("Private key in PEM format"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel11->Wrap( -1 );
	bSizer871->Add( m_pLabel11, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pTextoPrivateKey = new wxTextCtrl( m_pPanelPrivate, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_pTextoPrivateKey->SetMaxLength( 0 ); 
	bSizer871->Add( m_pTextoPrivateKey, 1, wxEXPAND|wxTOP, 5 );
	
	m_button621 = new wxButton( m_pPanelPrivate, wxID_ANY, _("Load ..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer871->Add( m_button621, 0, wxALIGN_RIGHT|wxTOP, 5 );
	
	
	m_pPanelPrivate->SetSizer( bSizer871 );
	m_pPanelPrivate->Layout();
	bSizer871->Fit( m_pPanelPrivate );
	bSizer86->Add( m_pPanelPrivate, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer86 );
	m_pBody->Layout();
	bSizer86->Fit( m_pBody );
	bSizer83->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button48 = new wxButton( m_pFooter, wxID_ANY, _("Reset to default"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_button48, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button8 = new wxButton( m_pFooter, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_button8, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button9 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_button9, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_pFooter->SetSizer( bSizer19 );
	m_pFooter->Layout();
	bSizer19->Fit( m_pFooter );
	bSizer83->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer83 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pCheckDefault->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnCheckDefault ), NULL, this );
	m_button62->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnLoadPublicClick ), NULL, this );
	m_button621->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnLoadPrivateClick ), NULL, this );
	m_button48->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnResetToDefault ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnAceptarClick ), NULL, this );
}

DialogoEditarCertificadoBase::~DialogoEditarCertificadoBase()
{
	// Disconnect Events
	m_pCheckDefault->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnCheckDefault ), NULL, this );
	m_button62->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnLoadPublicClick ), NULL, this );
	m_button621->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnLoadPrivateClick ), NULL, this );
	m_button48->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnResetToDefault ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoEditarCertificadoBase::OnAceptarClick ), NULL, this );
	
}

ProfileDialogBase::ProfileDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 770,540 ), wxDefaultSize );
	
	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxVERTICAL );
	
	m_pBasePanel = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );
	
	m_pTitledPanel = new TitledPanel( m_pBasePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pTitledPanel->SetToolTip( _("Profile") );
	
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer14->AddGrowableCol( 1 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText47 = new wxStaticText( m_pTitledPanel, wxID_ANY, _("Profile name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	fgSizer14->Add( m_staticText47, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pName = new wxTextCtrl( m_pTitledPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pName->SetMaxLength( 0 ); 
	fgSizer14->Add( m_pName, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	bSizer43->Add( fgSizer14, 0, wxEXPAND|wxBOTTOM, 5 );
	
	m_pGridPermisos = new wxPropertyGrid(
	m_pTitledPanel,
	wxID_ANY,
	wxDefaultPosition,
	wxDefaultSize,
	wxPG_AUTO_SORT |
	wxPG_SPLITTER_AUTO_CENTER|
	wxPG_DEFAULT_STYLE
	);
	bSizer43->Add( m_pGridPermisos, 1, wxALL|wxEXPAND, 5 );
	
	
	m_pTitledPanel->SetSizer( bSizer43 );
	m_pTitledPanel->Layout();
	bSizer43->Fit( m_pTitledPanel );
	bSizer44->Add( m_pTitledPanel, 1, wxEXPAND, 5 );
	
	
	m_pBasePanel->SetSizer( bSizer44 );
	m_pBasePanel->Layout();
	bSizer44->Fit( m_pBasePanel );
	bSizer53->Add( m_pBasePanel, 1, wxEXPAND, 5 );
	
	m_pFooterPanel = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer54;
	bSizer54 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer54->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button28 = new wxButton( m_pFooterPanel, wxID_ANY, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button28->SetDefault(); 
	bSizer54->Add( m_button28, 0, wxRIGHT, 5 );
	
	m_button29 = new wxButton( m_pFooterPanel, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer54->Add( m_button29, 0, wxLEFT, 5 );
	
	
	m_pFooterPanel->SetSizer( bSizer54 );
	m_pFooterPanel->Layout();
	bSizer54->Fit( m_pFooterPanel );
	bSizer53->Add( m_pFooterPanel, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer53 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button28->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProfileDialogBase::OnOkClick ), NULL, this );
	m_button29->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProfileDialogBase::OnCancelClick ), NULL, this );
}

ProfileDialogBase::~ProfileDialogBase()
{
	// Disconnect Events
	m_button28->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProfileDialogBase::OnOkClick ), NULL, this );
	m_button29->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProfileDialogBase::OnCancelClick ), NULL, this );
	
}

PanelConfiguracionPermisosBase::PanelConfiguracionPermisosBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 450,-1 ) );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pBasePanel = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pBasePanel->SetToolTip( _("Profiles") );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListProfiles = new wxDataViewListCtrl(m_pBasePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	bSizer25->Add( m_pListProfiles, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_button3 = new wxButton( m_pBasePanel, wxID_ANY, _("New ..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_button3, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pBEdit = new wxButton( m_pBasePanel, wxID_ANY, _("Edit ..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBEdit->Enable( false );
	
	bSizer26->Add( m_pBEdit, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pBDelete = new wxButton( m_pBasePanel, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBDelete->Enable( false );
	
	bSizer26->Add( m_pBDelete, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer26->Add( 0, 0, 1, wxEXPAND|wxRIGHT, 5 );
	
	m_pBActive = new wxButton( m_pBasePanel, wxID_ANY, _("Set active"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_pBActive, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer25->Add( bSizer26, 0, wxEXPAND, 5 );
	
	
	m_pBasePanel->SetSizer( bSizer25 );
	m_pBasePanel->Layout();
	bSizer25->Fit( m_pBasePanel );
	bSizer6->Add( m_pBasePanel, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionPermisosBase::OnSize ) );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnNuevoClick ), NULL, this );
	m_pBEdit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnEditarClick ), NULL, this );
	m_pBDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnEliminarClick ), NULL, this );
	m_pBActive->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnSetActive ), NULL, this );
}

PanelConfiguracionPermisosBase::~PanelConfiguracionPermisosBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionPermisosBase::OnSize ) );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnNuevoClick ), NULL, this );
	m_pBEdit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnEditarClick ), NULL, this );
	m_pBDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnEliminarClick ), NULL, this );
	m_pBActive->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionPermisosBase::OnSetActive ), NULL, this );
	
}

PanelConfiguracionUbicacionesBase::PanelConfiguracionUbicacionesBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 450,-1 ) );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pUbicaciones = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pUbicaciones->SetToolTip( _("Locations") );
	
	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer76;
	bSizer76 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextExplicacion = new wxStaticText( m_pUbicaciones, wxID_ANY, _("Locations are predefined paths to set locations from well known paths (ex: remote shared disk)"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticTextExplicacion->Wrap( 430 );
	bSizer76->Add( m_staticTextExplicacion, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer63->Add( bSizer76, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListUbicaciones = new wxDataViewListCtrl(m_pUbicaciones, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	m_pMainSizer->Add( m_pListUbicaciones, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_pSizerBotonera;
	m_pSizerBotonera = new wxBoxSizer( wxVERTICAL );
	
	m_pBotonNuevo = new wxButton( m_pUbicaciones, wxID_ANY, _("New ..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSizerBotonera->Add( m_pBotonNuevo, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pBotonEdit = new wxButton( m_pUbicaciones, wxID_ANY, _("Edit ..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBotonEdit->Enable( false );
	
	m_pSizerBotonera->Add( m_pBotonEdit, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pBotonDelete = new wxButton( m_pUbicaciones, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBotonDelete->Enable( false );
	
	m_pSizerBotonera->Add( m_pBotonDelete, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	m_pSizerBotonera->Add( 0, 0, 1, wxEXPAND|wxRIGHT, 5 );
	
	
	m_pMainSizer->Add( m_pSizerBotonera, 0, wxEXPAND, 5 );
	
	
	bSizer63->Add( m_pMainSizer, 1, wxEXPAND, 5 );
	
	
	m_pUbicaciones->SetSizer( bSizer63 );
	m_pUbicaciones->Layout();
	bSizer63->Fit( m_pUbicaciones );
	bSizer6->Add( m_pUbicaciones, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionUbicacionesBase::OnSize ) );
	m_pBotonNuevo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionUbicacionesBase::OnNuevoClick ), NULL, this );
	m_pBotonEdit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionUbicacionesBase::OnEditarClick ), NULL, this );
	m_pBotonDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionUbicacionesBase::OnEliminarClick ), NULL, this );
}

PanelConfiguracionUbicacionesBase::~PanelConfiguracionUbicacionesBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PanelConfiguracionUbicacionesBase::OnSize ) );
	m_pBotonNuevo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionUbicacionesBase::OnNuevoClick ), NULL, this );
	m_pBotonEdit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionUbicacionesBase::OnEditarClick ), NULL, this );
	m_pBotonDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionUbicacionesBase::OnEliminarClick ), NULL, this );
	
}

DialogoAddLocationBase::DialogoAddLocationBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 450,450 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_pBodySizer;
	m_pBodySizer = new wxBoxSizer( wxVERTICAL );
	
	m_pPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanel->SetToolTip( _("Configure location") );
	
	wxBoxSizer* m_pPanelSizer;
	m_pPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* m_pTableSizer;
	m_pTableSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_pTableSizer->AddGrowableCol( 1 );
	m_pTableSizer->SetFlexibleDirection( wxBOTH );
	m_pTableSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pSTTitle = new wxStaticText( m_pPanel, wxID_ANY, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSTTitle->Wrap( -1 );
	m_pTableSizer->Add( m_pSTTitle, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );
	
	m_pTETitle = new wxTextCtrl( m_pPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pTETitle->SetMaxLength( 8 ); 
	m_pTableSizer->Add( m_pTETitle, 0, wxEXPAND|wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSTLocation = new wxStaticText( m_pPanel, wxID_ANY, _("Location"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSTLocation->Wrap( -1 );
	m_pTableSizer->Add( m_pSTLocation, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxBoxSizer* m_pDirSizer;
	m_pDirSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_pTELocation = new wxTextCtrl( m_pPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pTELocation->SetMaxLength( 0 ); 
	m_pDirSizer->Add( m_pTELocation, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );
	
	m_pOpenDir = new wxButton( m_pPanel, wxID_ANY, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_pDirSizer->Add( m_pOpenDir, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM, 5 );
	
	
	m_pTableSizer->Add( m_pDirSizer, 0, wxTOP|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSTDesc = new wxStaticText( m_pPanel, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSTDesc->Wrap( -1 );
	m_pTableSizer->Add( m_pSTDesc, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5 );
	
	m_pTEDesc = new wxTextCtrl( m_pPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pTEDesc->SetMaxLength( 100 ); 
	m_pTableSizer->Add( m_pTEDesc, 0, wxEXPAND|wxTOP|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_pPanelSizer->Add( m_pTableSizer, 0, wxEXPAND|wxBOTTOM, 5 );
	
	m_pMonitorize = new wxCheckBox( m_pPanel, wxID_ANY, _("Monitorize"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pPanelSizer->Add( m_pMonitorize, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pCleanAfter = new wxCheckBox( m_pPanel, wxID_ANY, _("Clean directory after dicomization"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pPanelSizer->Add( m_pCleanAfter, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pCleanBefore = new wxCheckBox( m_pPanel, wxID_ANY, _("Clean directory before dicomization"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pPanelSizer->Add( m_pCleanBefore, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	m_pPanel->SetSizer( m_pPanelSizer );
	m_pPanel->Layout();
	m_pPanelSizer->Fit( m_pPanel );
	m_pBodySizer->Add( m_pPanel, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( m_pBodySizer );
	m_pBody->Layout();
	m_pBodySizer->Fit( m_pBody );
	m_pMainSizer->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_pFooterSizer;
	m_pFooterSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_pFooterSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pAccept = new wxButton( m_pFooter, wxID_ANY, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pAccept->SetDefault(); 
	m_pFooterSizer->Add( m_pAccept, 0, wxRIGHT, 5 );
	
	m_pCancel = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pFooterSizer->Add( m_pCancel, 0, wxRIGHT, 5 );
	
	
	m_pFooter->SetSizer( m_pFooterSizer );
	m_pFooter->Layout();
	m_pFooterSizer->Fit( m_pFooter );
	m_pMainSizer->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	// Connect Events
	m_pOpenDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAddLocationBase::OnOpenDirClick ), NULL, this );
	m_pAccept->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAddLocationBase::OnAceptarClick ), NULL, this );
}

DialogoAddLocationBase::~DialogoAddLocationBase()
{
	// Disconnect Events
	m_pOpenDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAddLocationBase::OnOpenDirClick ), NULL, this );
	m_pAccept->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAddLocationBase::OnAceptarClick ), NULL, this );
	
}

PanelConfiguracionSeguridadBase::PanelConfiguracionSeguridadBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer( wxVERTICAL );
	
	m_pSecurity = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pSecurity->SetToolTip( _("Security") );
	
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer57;
	bSizer57 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText47 = new wxStaticText( m_pSecurity, wxID_ANY, _("Security type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	bSizer57->Add( m_staticText47, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_pSecTypeChoices;
	m_pSecType = new wxChoice( m_pSecurity, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pSecTypeChoices, 0 );
	m_pSecType->SetSelection( 0 );
	bSizer57->Add( m_pSecType, 0, wxALL, 5 );
	
	
	bSizer44->Add( bSizer57, 1, wxEXPAND, 5 );
	
	
	m_pSecurity->SetSizer( bSizer44 );
	m_pSecurity->Layout();
	bSizer44->Fit( m_pSecurity );
	bSizer48->Add( m_pSecurity, 0, wxEXPAND, 5 );
	
	m_pSep = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer48->Add( m_pSep, 0, wxEXPAND, 5 );
	
	m_pPanelUsuarios = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelUsuarios->SetToolTip( _("Users") );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListaUsuarios = new wxListCtrl( m_pPanelUsuarios, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_SINGLE_SEL|wxLC_SMALL_ICON );
	bSizer25->Add( m_pListaUsuarios, 1, wxEXPAND|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_button3 = new wxButton( m_pPanelUsuarios, wxID_ANY, _("New ..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_button3, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5 );
	
	m_pBCambiarPassword = new wxButton( m_pPanelUsuarios, wxID_ANY, _("Change Password ..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBCambiarPassword->Enable( false );
	
	bSizer26->Add( m_pBCambiarPassword, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pBDelete = new wxButton( m_pPanelUsuarios, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBDelete->Enable( false );
	
	bSizer26->Add( m_pBDelete, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer26->Add( 0, 0, 1, wxEXPAND|wxRIGHT, 5 );
	
	
	bSizer25->Add( bSizer26, 0, wxEXPAND, 5 );
	
	
	m_pPanelUsuarios->SetSizer( bSizer25 );
	m_pPanelUsuarios->Layout();
	bSizer25->Fit( m_pPanelUsuarios );
	bSizer48->Add( m_pPanelUsuarios, 1, wxEXPAND, 5 );
	
	m_pPanelPassword = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelPassword->SetToolTip( _("Set main password") );
	
	wxBoxSizer* bSizer251;
	bSizer251 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabelPassword = new wxStaticText( m_pPanelPassword, wxID_ANY, _("Password is not setted"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelPassword->Wrap( -1 );
	bSizer251->Add( m_pLabelPassword, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pButtonSetPassword = new wxButton( m_pPanelPassword, wxID_ANY, _("Set Password"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer251->Add( m_pButtonSetPassword, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_pPanelPassword->SetSizer( bSizer251 );
	m_pPanelPassword->Layout();
	bSizer251->Fit( m_pPanelPassword );
	bSizer48->Add( m_pPanelPassword, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer48 );
	this->Layout();
	
	// Connect Events
	m_pSecurity->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionSeguridadBase::OnEraseBackground ), NULL, this );
	m_pSecType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnSecTypeChoice ), NULL, this );
	m_pPanelUsuarios->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionSeguridadBase::OnEraseBackground ), NULL, this );
	m_pListaUsuarios->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PanelConfiguracionSeguridadBase::OnListaUsuariosItemDeSelected ), NULL, this );
	m_pListaUsuarios->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PanelConfiguracionSeguridadBase::OnListaUsuariosItemSelected ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnNuevoClick ), NULL, this );
	m_pBCambiarPassword->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnCambiarPasswordClick ), NULL, this );
	m_pBDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnEliminarClick ), NULL, this );
	m_pPanelPassword->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionSeguridadBase::OnEraseBackground ), NULL, this );
	m_pButtonSetPassword->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnSetMainPassword ), NULL, this );
}

PanelConfiguracionSeguridadBase::~PanelConfiguracionSeguridadBase()
{
	// Disconnect Events
	m_pSecurity->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionSeguridadBase::OnEraseBackground ), NULL, this );
	m_pSecType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnSecTypeChoice ), NULL, this );
	m_pPanelUsuarios->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionSeguridadBase::OnEraseBackground ), NULL, this );
	m_pListaUsuarios->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PanelConfiguracionSeguridadBase::OnListaUsuariosItemDeSelected ), NULL, this );
	m_pListaUsuarios->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PanelConfiguracionSeguridadBase::OnListaUsuariosItemSelected ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnNuevoClick ), NULL, this );
	m_pBCambiarPassword->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnCambiarPasswordClick ), NULL, this );
	m_pBDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnEliminarClick ), NULL, this );
	m_pPanelPassword->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PanelConfiguracionSeguridadBase::OnEraseBackground ), NULL, this );
	m_pButtonSetPassword->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PanelConfiguracionSeguridadBase::OnSetMainPassword ), NULL, this );
	
}

UserDialogBase::UserDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 423,292 ), wxDefaultSize );
	
	wxBoxSizer* bSizer67;
	bSizer67 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Edit User") );
	
	bSizer67->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer68;
	bSizer68 = new wxBoxSizer( wxVERTICAL );
	
	m_pFormPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pFormPanel->SetToolTip( _("Edit User") );
	
	wxBoxSizer* bSizer75;
	bSizer75 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer75->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer18->AddGrowableCol( 1 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pLabelUser = new wxStaticText( m_pFormPanel, wxID_ANY, _("User Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelUser->Wrap( -1 );
	fgSizer18->Add( m_pLabelUser, 0, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pTCUsuario = new wxTextCtrl( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pTCUsuario->SetMaxLength( 0 ); 
	fgSizer18->Add( m_pTCUsuario, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxEXPAND, 5 );
	
	m_pstat2 = new wxStaticText( m_pFormPanel, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstat2->Wrap( -1 );
	fgSizer18->Add( m_pstat2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pTCPassword0 = new wxTextCtrl( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_pTCPassword0->SetMaxLength( 0 ); 
	fgSizer18->Add( m_pTCPassword0, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pstat21 = new wxStaticText( m_pFormPanel, wxID_ANY, _("Confirm Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pstat21->Wrap( -1 );
	fgSizer18->Add( m_pstat21, 0, wxTOP|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pTCPassword1 = new wxTextCtrl( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_pTCPassword1->SetMaxLength( 0 ); 
	fgSizer18->Add( m_pTCPassword1, 0, wxTOP|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	bSizer75->Add( fgSizer18, 0, wxEXPAND, 5 );
	
	
	bSizer75->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_pFormPanel->SetSizer( bSizer75 );
	m_pFormPanel->Layout();
	bSizer75->Fit( m_pFormPanel );
	bSizer68->Add( m_pFormPanel, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer68 );
	m_pBody->Layout();
	bSizer68->Fit( m_pBody );
	bSizer67->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_panel54 = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer69->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button42 = new wxButton( m_panel54, wxID_ANY, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button42->SetDefault(); 
	bSizer69->Add( m_button42, 0, wxRIGHT, 5 );
	
	m_button41 = new wxButton( m_panel54, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer69->Add( m_button41, 0, wxLEFT, 5 );
	
	
	m_panel54->SetSizer( bSizer69 );
	m_panel54->Layout();
	bSizer69->Fit( m_panel54 );
	bSizer67->Add( m_panel54, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer67 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button42->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UserDialogBase::OnOkClick ), NULL, this );
}

UserDialogBase::~UserDialogBase()
{
	// Disconnect Events
	m_button42->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UserDialogBase::OnOkClick ), NULL, this );
	
}

PanelConfigurationAnonymizationBase::PanelConfigurationAnonymizationBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer77;
	bSizer77 = new wxBoxSizer( wxVERTICAL );
	
	m_pAnonimization = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pAnonimization->SetToolTip( _("Properties") );
	
	wxBoxSizer* bSizer78;
	bSizer78 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer79;
	bSizer79 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText52 = new wxStaticText( m_pAnonimization, wxID_ANY, _("Default value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText52->Wrap( -1 );
	bSizer79->Add( m_staticText52, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pTextDefaultValue = new wxTextCtrl( m_pAnonimization, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_pTextDefaultValue->SetMaxLength( 0 ); 
	bSizer79->Add( m_pTextDefaultValue, 0, wxALL, 5 );
	
	
	bSizer78->Add( bSizer79, 0, wxEXPAND, 5 );
	
	
	m_pAnonimization->SetSizer( bSizer78 );
	m_pAnonimization->Layout();
	bSizer78->Fit( m_pAnonimization );
	bSizer77->Add( m_pAnonimization, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer77 );
	this->Layout();
}

PanelConfigurationAnonymizationBase::~PanelConfigurationAnonymizationBase()
{
}

AddWLDialogBase::AddWLDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 350,250 ), wxDefaultSize );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelMessage = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelMessage->SetToolTip( _("Add DICOM tag") );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer17->AddGrowableCol( 1 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText79 = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Label"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText79->Wrap( -1 );
	fgSizer17->Add( m_staticText79, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pLabel = new wxTextCtrl( m_pPanelMessage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->SetMaxLength( 0 ); 
	fgSizer17->Add( m_pLabel, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText59 = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer17->Add( m_staticText59, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pWindow = new wxTextCtrl( m_pPanelMessage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pWindow->SetMaxLength( 0 ); 
	m_pWindow->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &windowValidator ) );
	
	fgSizer17->Add( m_pWindow, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText60 = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	fgSizer17->Add( m_staticText60, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pLevel = new wxTextCtrl( m_pPanelMessage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLevel->SetMaxLength( 0 ); 
	m_pLevel->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &levelValidator ) );
	
	fgSizer17->Add( m_pLevel, 1, wxALL|wxEXPAND, 5 );
	
	
	m_pPanelMessage->SetSizer( fgSizer17 );
	m_pPanelMessage->Layout();
	fgSizer17->Fit( m_pPanelMessage );
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

AddWLDialogBase::~AddWLDialogBase()
{
}

EditCornerAnnotationsDialogBase::EditCornerAnnotationsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,500 ), wxDefaultSize );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pMainPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pMainPanel->SetToolTip( _("Edit corner annotations") );
	
	wxBoxSizer* bSizer130;
	bSizer130 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer136;
	bSizer136 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText84 = new wxStaticText( m_pMainPanel, wxID_ANY, _("To setup corner annotations you can use dicom tags with syntax ${GGGG|EEEE:DefaultValue:Format}  where GGGG is the tag's group in hexadecimal and EEEE is the tag's element. DefaultValue and Format are optional values, Format admit two special formats (date and time)."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText84->Wrap( 600 );
	bSizer136->Add( m_staticText84, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_staticText85 = new wxStaticText( m_pMainPanel, wxID_ANY, _("You can also use special tags listed in the tooltip over the question button."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText85->Wrap( 600 );
	bSizer136->Add( m_staticText85, 0, wxALL, 5 );
	
	
	bSizer131->Add( bSizer136, 1, wxEXPAND, 5 );
	
	m_pHelpBitmap = new wxStaticBitmap( m_pMainPanel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_pHelpBitmap->SetToolTip( _("${NEWLINE}\n${GINKGO.VERSION.FULLTEXT}\n${GINKGO.ARCH}             \n\n${VOLUME.IDX}\n${SLICE.WIDTH}\n${SLICE.HEIGHT}\n${SLICE.WL.FULLTEXT}\n${SLICE.IDX}\n${SLICE.COUNT}\n${SLICE.IDX.FULLTEXT}\n${SLICE.LATERALITY}\n${SLICE.VOXEL.SIZE}\n${SLICE.THICKNESS}\n${SLICE.THICKNESS.FULLTEXT}\n${SLICE.LOCATION}\n${SLICE.LOCATION.FULLTEXT}\n${VIEW.SCALE}") );
	
	bSizer131->Add( m_pHelpBitmap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer130->Add( bSizer131, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_pMainPanel, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_pCornerAnnotationsSizer = new wxGridSizer( 0, 2, 0, 0 );
	
	
	sbSizer10->Add( m_pCornerAnnotationsSizer, 1, wxEXPAND, 5 );
	
	
	bSizer130->Add( sbSizer10, 1, wxEXPAND, 5 );
	
	
	m_pMainPanel->SetSizer( bSizer130 );
	m_pMainPanel->Layout();
	bSizer130->Fit( m_pMainPanel );
	bSizer7->Add( m_pMainPanel, 1, wxEXPAND, 5 );
	
	
	m_pBody->SetSizer( bSizer7 );
	m_pBody->Layout();
	bSizer7->Fit( m_pBody );
	bSizer15->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button70 = new wxButton( m_pFooter, wxID_ANY, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button70, 0, 0, 5 );
	
	
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
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button70->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditCornerAnnotationsDialogBase::OnSetDefaultValues ), NULL, this );
}

EditCornerAnnotationsDialogBase::~EditCornerAnnotationsDialogBase()
{
	// Disconnect Events
	m_button70->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditCornerAnnotationsDialogBase::OnSetDefaultValues ), NULL, this );
	
}

ModalityPanelBase::ModalityPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer125;
	bSizer125 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Window/level") ), wxVERTICAL );
	
	wxBoxSizer* bSizer129;
	bSizer129 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListWL = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	m_pListWL->SetMinSize( wxSize( -1,150 ) );
	
	bSizer129->Add( m_pListWL, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer126;
	bSizer126 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer127;
	bSizer127 = new wxBoxSizer( wxVERTICAL );
	
	m_button63 = new wxButton( this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer127->Add( m_button63, 0, wxTOP|wxBOTTOM|wxEXPAND, 5 );
	
	m_button64 = new wxButton( this, wxID_ANY, _("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer127->Add( m_button64, 0, wxTOP|wxBOTTOM|wxEXPAND, 5 );
	
	m_button65 = new wxButton( this, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer127->Add( m_button65, 0, wxEXPAND, 5 );
	
	
	bSizer126->Add( bSizer127, 1, wxEXPAND, 5 );
	
	
	bSizer129->Add( bSizer126, 0, wxEXPAND, 5 );
	
	
	sbSizer12->Add( bSizer129, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText68 = new wxStaticText( this, wxID_ANY, _("Default Window/Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText68->Wrap( -1 );
	fgSizer20->Add( m_staticText68, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_pDefaultWindowLevelChoices;
	m_pDefaultWindowLevel = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pDefaultWindowLevelChoices, 0 );
	m_pDefaultWindowLevel->SetSelection( 0 );
	fgSizer20->Add( m_pDefaultWindowLevel, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	sbSizer12->Add( fgSizer20, 0, wxEXPAND, 5 );
	
	
	bSizer125->Add( sbSizer12, 0, wxEXPAND|wxBOTTOM, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Corner annotations") ), wxHORIZONTAL );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, _("Setup corner annotations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	sbSizer9->Add( m_staticText71, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button67 = new wxButton( this, wxID_ANY, _("Setup..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( m_button67, 0, wxALL, 5 );
	
	
	bSizer125->Add( sbSizer9, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer13;
	sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Other settings") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText69 = new wxStaticText( this, wxID_ANY, _("Interpolate pixel data"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	fgSizer21->Add( m_staticText69, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_pChoiceInterpolateChoices;
	m_pChoiceInterpolate = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pChoiceInterpolateChoices, 0 );
	m_pChoiceInterpolate->SetSelection( 0 );
	fgSizer21->Add( m_pChoiceInterpolate, 0, wxALL, 5 );
	
	
	sbSizer13->Add( fgSizer21, 1, wxEXPAND, 5 );
	
	
	bSizer125->Add( sbSizer13, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer125 );
	this->Layout();
	bSizer125->Fit( this );
	
	// Connect Events
	m_button63->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnAddWL ), NULL, this );
	m_button64->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnEditWL ), NULL, this );
	m_button65->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnDeleteWL ), NULL, this );
	m_pDefaultWindowLevel->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ModalityPanelBase::OnChoiceChanged ), NULL, this );
	m_button67->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnSetupCornerAnnotations ), NULL, this );
	m_pChoiceInterpolate->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ModalityPanelBase::OnChoiceChanged ), NULL, this );
}

ModalityPanelBase::~ModalityPanelBase()
{
	// Disconnect Events
	m_button63->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnAddWL ), NULL, this );
	m_button64->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnEditWL ), NULL, this );
	m_button65->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnDeleteWL ), NULL, this );
	m_pDefaultWindowLevel->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ModalityPanelBase::OnChoiceChanged ), NULL, this );
	m_button67->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModalityPanelBase::OnSetupCornerAnnotations ), NULL, this );
	m_pChoiceInterpolate->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ModalityPanelBase::OnChoiceChanged ), NULL, this );
	
}

EditHangingProtocolDialogBase::EditHangingProtocolDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,600 ), wxDefaultSize );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Hanging protocol") );
	
	bSizer11->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pMainNoteBook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_FLAT );
	m_pGeneralPanel = new wxPanel( m_pMainNoteBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer109;
	bSizer109 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer25;
	fgSizer25 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer25->AddGrowableCol( 1 );
	fgSizer25->SetFlexibleDirection( wxBOTH );
	fgSizer25->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText87 = new wxStaticText( m_pGeneralPanel, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText87->Wrap( -1 );
	fgSizer25->Add( m_staticText87, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textName = new wxTextCtrl( m_pGeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textName->SetMaxLength( 0 ); 
	fgSizer25->Add( m_textName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText91 = new wxStaticText( m_pGeneralPanel, wxID_ANY, _("Opening mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	fgSizer25->Add( m_staticText91, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textOpeningMode = new wxTextCtrl( m_pGeneralPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textOpeningMode->SetMaxLength( 0 ); 
	m_textOpeningMode->Enable( false );
	
	fgSizer25->Add( m_textOpeningMode, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer109->Add( fgSizer25, 0, wxEXPAND|wxTOP, 5 );
	
	m_pSizerStudyCriteria = new wxStaticBoxSizer( new wxStaticBox( m_pGeneralPanel, wxID_ANY, _("Study opening criteria") ), wxVERTICAL );
	
	m_pSizerStudyCriteria->SetMinSize( wxSize( -1,250 ) ); 
	
	bSizer109->Add( m_pSizerStudyCriteria, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	
	m_pGeneralPanel->SetSizer( bSizer109 );
	m_pGeneralPanel->Layout();
	bSizer109->Fit( m_pGeneralPanel );
	m_pMainNoteBook->AddPage( m_pGeneralPanel, _("General"), false );
	m_pWorkspacePanel = new wxPanel( m_pMainNoteBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1102;
	bSizer1102 = new wxBoxSizer( wxVERTICAL );
	
	m_pNoteBookWindows = new wxAuiNotebook( m_pWorkspacePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_ON_ACTIVE_TAB|wxAUI_NB_DEFAULT_STYLE );
	m_pAdddPanel = new wxPanel( m_pNoteBookWindows, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pNoteBookWindows->AddPage( m_pAdddPanel, _("+"), false, wxNullBitmap );
	
	bSizer1102->Add( m_pNoteBookWindows, 1, wxEXPAND|wxALL, 5 );
	
	
	m_pWorkspacePanel->SetSizer( bSizer1102 );
	m_pWorkspacePanel->Layout();
	bSizer1102->Fit( m_pWorkspacePanel );
	m_pMainNoteBook->AddPage( m_pWorkspacePanel, _("Workspace"), true );
	
	bSizer11->Add( m_pMainNoteBook, 1, wxEXPAND|wxTOP, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button37 = new wxButton( m_pFooter, wxID_OK, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button37, 0, wxALL, 5 );
	
	m_button38 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button38, 0, wxALL, 5 );
	
	
	m_pFooter->SetSizer( bSizer31 );
	m_pFooter->Layout();
	bSizer31->Fit( m_pFooter );
	bSizer11->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer11 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pNoteBookWindows->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING, wxAuiNotebookEventHandler( EditHangingProtocolDialogBase::OnPageChanging ), NULL, this );
	m_pNoteBookWindows->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( EditHangingProtocolDialogBase::OnPageClose ), NULL, this );
	m_button37->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditHangingProtocolDialogBase::OnOkClick ), NULL, this );
	m_button38->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditHangingProtocolDialogBase::OnCancelClick ), NULL, this );
}

EditHangingProtocolDialogBase::~EditHangingProtocolDialogBase()
{
	// Disconnect Events
	m_pNoteBookWindows->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING, wxAuiNotebookEventHandler( EditHangingProtocolDialogBase::OnPageChanging ), NULL, this );
	m_pNoteBookWindows->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( EditHangingProtocolDialogBase::OnPageClose ), NULL, this );
	m_button37->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditHangingProtocolDialogBase::OnOkClick ), NULL, this );
	m_button38->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditHangingProtocolDialogBase::OnCancelClick ), NULL, this );
	
}

HangingProtocolConfigurationPanelBase::HangingProtocolConfigurationPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_pProtocols = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pProtocols->SetToolTip( _("Hanging protocols") );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListHanging = new wxDataViewListCtrl(m_pProtocols, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	bSizer25->Add( m_pListHanging, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_button3 = new wxButton( m_pProtocols, wxID_ANY, _("New ..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_button3, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5 );
	
	m_pBEdit = new wxButton( m_pProtocols, wxID_ANY, _("Edit ..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBEdit->Enable( false );
	
	bSizer26->Add( m_pBEdit, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5 );
	
	m_pBDelete = new wxButton( m_pProtocols, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBDelete->Enable( false );
	
	bSizer26->Add( m_pBDelete, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer26->Add( 0, 0, 1, wxEXPAND|wxRIGHT, 5 );
	
	m_pBActive = new wxButton( m_pProtocols, wxID_ANY, _("Enable/Disable"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_pBActive, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer25->Add( bSizer26, 0, wxEXPAND, 5 );
	
	
	m_pProtocols->SetSizer( bSizer25 );
	m_pProtocols->Layout();
	bSizer25->Fit( m_pProtocols );
	bSizer6->Add( m_pProtocols, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	
	// Connect Events
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnNuevoClick ), NULL, this );
	m_pBEdit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnEditarClick ), NULL, this );
	m_pBDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnEliminarClick ), NULL, this );
	m_pBActive->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnSetActive ), NULL, this );
}

HangingProtocolConfigurationPanelBase::~HangingProtocolConfigurationPanelBase()
{
	// Disconnect Events
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnNuevoClick ), NULL, this );
	m_pBEdit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnEditarClick ), NULL, this );
	m_pBDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnEliminarClick ), NULL, this );
	m_pBActive->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HangingProtocolConfigurationPanelBase::OnSetActive ), NULL, this );
	
}

DefaultModalitySettingsConfigurationPanelBase::DefaultModalitySettingsConfigurationPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer120;
	bSizer120 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter4 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter4->Connect( wxEVT_IDLE, wxIdleEventHandler( DefaultModalitySettingsConfigurationPanelBase::m_splitter4OnIdle ), NULL, this );
	m_splitter4->SetMinimumPaneSize( 100 );
	
	m_panel90 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );
	
	m_pWLList = new wxTreeCtrl( m_panel90, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer121->Add( m_pWLList, 1, wxALL|wxEXPAND, 0 );
	
	
	m_panel90->SetSizer( bSizer121 );
	m_panel90->Layout();
	bSizer121->Fit( m_panel90 );
	m_pTitledPanelModalities = new SubTitledPanelSinBorder( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer122;
	bSizer122 = new wxBoxSizer( wxVERTICAL );
	
	m_pModalityPanel = new PanelScrollable( m_pTitledPanelModalities, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pSizerModalities = new wxBoxSizer( wxVERTICAL );
	
	
	m_pModalityPanel->SetSizer( m_pSizerModalities );
	m_pModalityPanel->Layout();
	m_pSizerModalities->Fit( m_pModalityPanel );
	bSizer122->Add( m_pModalityPanel, 1, wxEXPAND, 5 );
	
	
	m_pTitledPanelModalities->SetSizer( bSizer122 );
	m_pTitledPanelModalities->Layout();
	bSizer122->Fit( m_pTitledPanelModalities );
	m_splitter4->SplitVertically( m_panel90, m_pTitledPanelModalities, 150 );
	bSizer120->Add( m_splitter4, 1, wxEXPAND|wxTOP, 5 );
	
	
	this->SetSizer( bSizer120 );
	this->Layout();
	
	// Connect Events
	m_pWLList->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( DefaultModalitySettingsConfigurationPanelBase::OnTreeSelChanged ), NULL, this );
}

DefaultModalitySettingsConfigurationPanelBase::~DefaultModalitySettingsConfigurationPanelBase()
{
	// Disconnect Events
	m_pWLList->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( DefaultModalitySettingsConfigurationPanelBase::OnTreeSelChanged ), NULL, this );
	
}

CornerAnnotationPanel::CornerAnnotationPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer126;
	bSizer126 = new wxBoxSizer( wxVERTICAL );
	
	m_pLabel = new wxStaticText( this, wxID_ANY, _("Top left corner annotations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	bSizer126->Add( m_pLabel, 0, wxALL, 5 );
	
	m_pLine1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLine1->SetMaxLength( 0 ); 
	bSizer126->Add( m_pLine1, 0, wxALL|wxEXPAND, 5 );
	
	m_pLine2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLine2->SetMaxLength( 0 ); 
	bSizer126->Add( m_pLine2, 0, wxALL|wxEXPAND, 5 );
	
	m_pLine3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLine3->SetMaxLength( 0 ); 
	bSizer126->Add( m_pLine3, 0, wxALL|wxEXPAND, 5 );
	
	m_pLine4 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLine4->SetMaxLength( 0 ); 
	bSizer126->Add( m_pLine4, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer126 );
	this->Layout();
	bSizer126->Fit( this );
}

CornerAnnotationPanel::~CornerAnnotationPanel()
{
}

StudyWindowBase::StudyWindowBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	m_pConfigWindow = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pConfigWindow->SetToolTip( _("Setup window") );
	
	wxBoxSizer* bSizer901;
	bSizer901 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText72 = new wxStaticText( m_pConfigWindow, wxID_ANY, _("Push configuration button and place window where you want"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	bSizer901->Add( m_staticText72, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer901->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button62 = new wxButton( m_pConfigWindow, wxID_ANY, _("Set default"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button62->SetToolTip( _("Open in main window") );
	
	bSizer901->Add( m_button62, 0, wxRIGHT|wxLEFT, 5 );
	
	m_pSetupPositionButton = new wxButton( m_pConfigWindow, wxID_ANY, _("Setup position..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSetupPositionButton->SetToolTip( _("Setup undocked window position") );
	
	bSizer901->Add( m_pSetupPositionButton, 0, wxRIGHT|wxLEFT, 5 );
	
	
	m_pConfigWindow->SetSizer( bSizer901 );
	m_pConfigWindow->Layout();
	bSizer901->Fit( m_pConfigWindow );
	bSizer81->Add( m_pConfigWindow, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_panel64111 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel64111->SetToolTip( _("Settings") );
	
	bSizer81->Add( m_panel64111, 0, wxEXPAND, 5 );
	
	m_pLayoutPreview = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,220 ), wxTAB_TRAVERSAL );
	m_pLayoutPreview->SetToolTip( _("Layout") );
	m_pLayoutPreview->SetMinSize( wxSize( -1,220 ) );
	
	wxBoxSizer* bSizer113;
	bSizer113 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_pLayoutPreview, wxID_ANY, _("Current layout") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer119;
	bSizer119 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bsizer91;
	bsizer91 = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerLayoutTools = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_pSizerLayoutTools->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText56 = new wxStaticText( m_pLayoutPreview, wxID_ANY, _("Rows"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText56->Wrap( -1 );
	m_pSizerLayoutTools->Add( m_staticText56, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSpinRows = new wxSpinCtrl( m_pLayoutPreview, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 1, 4, 1 );
	m_pSizerLayoutTools->Add( m_pSpinRows, 0, wxALL, 5 );
	
	m_staticText57 = new wxStaticText( m_pLayoutPreview, wxID_ANY, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	m_pSizerLayoutTools->Add( m_staticText57, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSpinCols = new wxSpinCtrl( m_pLayoutPreview, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 1, 4, 1 );
	m_pSizerLayoutTools->Add( m_pSpinCols, 0, wxALL, 5 );
	
	
	bsizer91->Add( m_pSizerLayoutTools, 1, wxEXPAND|wxBOTTOM, 5 );
	
	
	bSizer119->Add( bsizer91, 0, wxEXPAND, 5 );
	
	m_pPreviewSizer = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer119->Add( m_pPreviewSizer, 1, wxEXPAND, 5 );
	
	
	sbSizer9->Add( bSizer119, 1, wxEXPAND, 5 );
	
	m_pSizerSeriesCriteria = new wxBoxSizer( wxVERTICAL );
	
	m_staticText65 = new wxStaticText( m_pLayoutPreview, wxID_ANY, _("Series criteria"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText65->Wrap( -1 );
	m_pSizerSeriesCriteria->Add( m_staticText65, 0, wxALL, 5 );
	
	
	sbSizer9->Add( m_pSizerSeriesCriteria, 1, wxEXPAND|wxLEFT, 5 );
	
	
	bSizer113->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	
	m_pLayoutPreview->SetSizer( bSizer113 );
	m_pLayoutPreview->Layout();
	bSizer81->Add( m_pLayoutPreview, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	m_panel6411 = new SubTitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6411->SetToolTip( _("Settings") );
	
	bSizer81->Add( m_panel6411, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	
	this->SetSizer( bSizer81 );
	this->Layout();
	
	// Connect Events
	m_button62->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyWindowBase::OnSetDefaultWindowPosition ), NULL, this );
	m_pSetupPositionButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyWindowBase::OnSetupWindowPositionClick ), NULL, this );
	m_pSpinRows->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StudyWindowBase::OnSpinRows ), NULL, this );
	m_pSpinCols->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StudyWindowBase::OnSpinCols ), NULL, this );
}

StudyWindowBase::~StudyWindowBase()
{
	// Disconnect Events
	m_button62->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyWindowBase::OnSetDefaultWindowPosition ), NULL, this );
	m_pSetupPositionButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyWindowBase::OnSetupWindowPositionClick ), NULL, this );
	m_pSpinRows->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StudyWindowBase::OnSpinRows ), NULL, this );
	m_pSpinCols->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StudyWindowBase::OnSpinCols ), NULL, this );
	
}

EditCriteriaPanelBase::EditCriteriaPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_pGeneralSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_pListCriteria= new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	m_pGeneralSizer->Add( m_pListCriteria, 1, wxALL|wxEXPAND, 5 );
	
	m_pButtonSizer = new wxBoxSizer( wxVERTICAL );
	
	m_button3 = new wxButton( this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pButtonSizer->Add( m_button3, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_pBDelete = new wxButton( this, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBDelete->Enable( false );
	
	m_pButtonSizer->Add( m_pBDelete, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	m_pGeneralSizer->Add( m_pButtonSizer, 0, wxEXPAND|wxRIGHT, 5 );
	
	
	this->SetSizer( m_pGeneralSizer );
	this->Layout();
	
	// Connect Events
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditCriteriaPanelBase::OnAddClick ), NULL, this );
	m_pBDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditCriteriaPanelBase::OnDeleteClick ), NULL, this );
}

EditCriteriaPanelBase::~EditCriteriaPanelBase()
{
	// Disconnect Events
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditCriteriaPanelBase::OnAddClick ), NULL, this );
	m_pBDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditCriteriaPanelBase::OnDeleteClick ), NULL, this );
	
}

SelectOpeningModeBase::SelectOpeningModeBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 525,180 ), wxDefaultSize );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelMessage = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelMessage->SetToolTip( _("Select opening mode") );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabel = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Select opening mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	bSizer4->Add( m_pLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_pOpeningModeChoices;
	m_pOpeningMode = new wxChoice( m_pPanelMessage, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pOpeningModeChoices, 0 );
	m_pOpeningMode->SetSelection( 0 );
	bSizer4->Add( m_pOpeningMode, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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

SelectOpeningModeBase::~SelectOpeningModeBase()
{
}

AddTagDialogBase::AddTagDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,220 ), wxDefaultSize );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelMessage = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelMessage->SetToolTip( _("Add DICOM tag") );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText59 = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Tag"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer17->Add( m_staticText59, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_staticText60 = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	fgSizer17->Add( m_staticText60, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString m_pChoiceTagChoices;
	m_pChoiceTag = new wxChoice( m_pPanelMessage, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pChoiceTagChoices, 0 );
	m_pChoiceTag->SetSelection( 0 );
	fgSizer17->Add( m_pChoiceTag, 0, wxALL, 5 );
	
	m_pTextValue = new wxTextCtrl( m_pPanelMessage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pTextValue->SetMaxLength( 0 ); 
	fgSizer17->Add( m_pTextValue, 0, wxALL, 5 );
	
	
	m_pPanelMessage->SetSizer( fgSizer17 );
	m_pPanelMessage->Layout();
	fgSizer17->Fit( m_pPanelMessage );
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

AddTagDialogBase::~AddTagDialogBase()
{
}

SetupWindowPositionBase::SetupWindowPositionBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );
	
	wxBoxSizer* bSizer113;
	bSizer113 = new wxBoxSizer( wxVERTICAL );
	
	m_panel97 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel97->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* bSizer114;
	bSizer114 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText73 = new wxStaticText( m_panel97, wxID_ANY, _("Setup position and size of the window where you want to open the study"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	bSizer114->Add( m_staticText73, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer115;
	bSizer115 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button64 = new wxButton( m_panel97, wxID_OK, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button64->SetDefault(); 
	bSizer115->Add( m_button64, 0, wxALL, 5 );
	
	m_button65 = new wxButton( m_panel97, wxID_CLOSE, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer115->Add( m_button65, 0, wxALL, 5 );
	
	
	bSizer114->Add( bSizer115, 0, wxEXPAND, 5 );
	
	
	m_panel97->SetSizer( bSizer114 );
	m_panel97->Layout();
	bSizer114->Fit( m_panel97 );
	bSizer113->Add( m_panel97, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer113 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

SetupWindowPositionBase::~SetupWindowPositionBase()
{
}

SmartRetrieveConfigurationPanelBase::SmartRetrieveConfigurationPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxVERTICAL );
	
	m_pCheckEnable = new wxCheckBox( this, wxID_ANY, _("Enable background queries"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_pCheckEnable, 0, wxALL, 5 );
	
	m_pPanelQueries = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelQueries->SetToolTip( _("Stored queries") );
	
	wxBoxSizer* bSizer134;
	bSizer134 = new wxBoxSizer( wxVERTICAL );
	
	m_pListQueries = new wxDataViewListCtrl(m_pPanelQueries, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	bSizer134->Add( m_pListQueries, 1, wxALL|wxEXPAND, 5 );
	
	
	m_pPanelQueries->SetSizer( bSizer134 );
	m_pPanelQueries->Layout();
	bSizer134->Fit( m_pPanelQueries );
	bSizer131->Add( m_pPanelQueries, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer131 );
	this->Layout();
	
	// Connect Events
	m_pCheckEnable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SmartRetrieveConfigurationPanelBase::OnCheckEnableBackgroundQueries ), NULL, this );
}

SmartRetrieveConfigurationPanelBase::~SmartRetrieveConfigurationPanelBase()
{
	// Disconnect Events
	m_pCheckEnable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SmartRetrieveConfigurationPanelBase::OnCheckEnableBackgroundQueries ), NULL, this );
	
}
