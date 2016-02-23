///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "downloadcloudbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

OpenGinkgoCloudDialogBase::OpenGinkgoCloudDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 700,350 ), wxDefaultSize );
	
	wxBoxSizer* bSizerPrincipal;
	bSizerPrincipal = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Ginkgo Cloud") );
	
	bSizerPrincipal->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelSuperior = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelSuperior->SetToolTip( _("Open link") );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText6 = new wxStaticText( m_pPanelSuperior, wxID_ANY, _("File link:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer81->Add( m_staticText6, 0, wxALL, 5 );
	
	m_pFileLink = new wxTextCtrl( m_pPanelSuperior, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer81->Add( m_pFileLink, 1, wxALL|wxEXPAND, 5 );
	
	
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
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_buttonAceptar = new wxButton( m_pFooter, wxID_OK, _("&Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAceptar->SetDefault(); 
	bSizer8->Add( m_buttonAceptar, 0, wxLEFT, 5 );
	
	m_pButtonCancel = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_pButtonCancel, 0, 0, 5 );
	
	
	m_pFooter->SetSizer( bSizer8 );
	m_pFooter->Layout();
	bSizer8->Fit( m_pFooter );
	bSizerPrincipal->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerPrincipal );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ) );
	m_pHeader->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pBody->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pPanelSuperior->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_staticText6->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pFileLink->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pFooter->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_buttonAceptar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( OpenGinkgoCloudDialogBase::OnAccept ), NULL, this );
	m_buttonAceptar->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pButtonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( OpenGinkgoCloudDialogBase::OnCancel ), NULL, this );
	m_pButtonCancel->Connect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
}

OpenGinkgoCloudDialogBase::~OpenGinkgoCloudDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ) );
	m_pHeader->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pBody->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pPanelSuperior->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_staticText6->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pFileLink->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pFooter->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_buttonAceptar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( OpenGinkgoCloudDialogBase::OnAccept ), NULL, this );
	m_buttonAceptar->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	m_pButtonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( OpenGinkgoCloudDialogBase::OnCancel ), NULL, this );
	m_pButtonCancel->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( OpenGinkgoCloudDialogBase::OnKeyUp ), NULL, this );
	
}

GinkgoCloudSearchDialogBase::GinkgoCloudSearchDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,300 ), wxDefaultSize );
	
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Ginkgo Cloud") );
	
	m_pMainSizer->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBodyPanel = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelPACS = new TitledPanel( m_pBodyPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelPACS->SetToolTip( _("Search") );
	
	wxBoxSizer* m_pSizerCamposBusqueda;
	m_pSizerCamposBusqueda = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pPatientLabelStr = new wxStaticText( m_pPanelPACS, wxID_ANY, _("Study"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pPatientLabelStr->Wrap( -1 );
	m_pPatientLabelStr->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer33->Add( m_pPatientLabelStr, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pFieldCombo = new wxComboBox( m_pPanelPACS, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	m_pFieldCombo->Append( _("Description") );
	m_pFieldCombo->Append( _("User name") );
	bSizer23->Add( m_pFieldCombo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSearchPanel = new wxPanel( m_pPanelPACS, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer271;
	bSizer271 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pTextControlField = new wxSearchCtrl( m_pSearchPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	#ifndef __WXMAC__
	m_pTextControlField->ShowSearchButton( true );
	#endif
	m_pTextControlField->ShowCancelButton( false );
	bSizer271->Add( m_pTextControlField, 1, wxALL|wxEXPAND, 5 );
	
	m_pLabelRestricted = new wxStaticText( m_pSearchPanel, wxID_ANY, _("PACS acquisition is restricted"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelRestricted->Wrap( -1 );
	m_pLabelRestricted->Hide();
	
	bSizer271->Add( m_pLabelRestricted, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSizerAnimation = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer271->Add( m_pSizerAnimation, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_pSearchPanel->SetSizer( bSizer271 );
	m_pSearchPanel->Layout();
	bSizer271->Fit( m_pSearchPanel );
	bSizer23->Add( m_pSearchPanel, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer33->Add( bSizer23, 1, wxEXPAND, 5 );
	
	
	bSizer32->Add( bSizer33, 1, wxEXPAND, 5 );
	
	
	m_pSizerCamposBusqueda->Add( bSizer32, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_pPanelPACS, wxID_ANY, _("Date") ), wxVERTICAL );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxVERTICAL );
	
	m_pAnyDate = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Any date"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_pAnyDate->SetValue( true ); 
	bSizer181->Add( m_pAnyDate, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 2 );
	
	m_pToday = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Today"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pToday, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pYesterday = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Yesterday"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pYesterday, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 2 );
	
	
	bSizer17->Add( bSizer181, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	m_pLastWeek = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Last 7 days"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pLastWeek, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pLastMonth = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Last 30 days"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pLastMonth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pLastThreeMonths = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Last 3 months"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pLastThreeMonths, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	
	bSizer17->Add( bSizer29, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxVERTICAL );
	
	m_pBetween = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Between:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( m_pBetween, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_pTextControlFechaDesde = new wxDatePickerCtrl( m_pPanelPACS, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_DROPDOWN );
	bSizer21->Add( m_pTextControlFechaDesde, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 2 );
	
	
	bSizer201->Add( bSizer21, 0, wxEXPAND|wxLEFT, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_pTextControlFechaHasta = new wxDatePickerCtrl( m_pPanelPACS, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_DROPDOWN );
	bSizer22->Add( m_pTextControlFechaHasta, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 2 );
	
	
	bSizer201->Add( bSizer22, 0, wxEXPAND|wxLEFT, 5 );
	
	
	bSizer17->Add( bSizer201, 0, wxEXPAND, 5 );
	
	
	sbSizer2->Add( bSizer17, 1, wxEXPAND|wxALL, 5 );
	
	
	bSizer27->Add( sbSizer2, 1, wxEXPAND|wxRIGHT, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_pPanelPACS, wxID_ANY, _("Modalities") ), wxVERTICAL );
	
	m_pModalitySizer = new wxGridSizer( 0, 5, 0, 0 );
	
	
	sbSizer3->Add( m_pModalitySizer, 1, wxEXPAND|wxALL, 5 );
	
	
	bSizer27->Add( sbSizer3, 1, wxEXPAND|wxLEFT, 5 );
	
	
	m_pSizerCamposBusqueda->Add( bSizer27, 0, wxEXPAND|wxBOTTOM, 5 );
	
	m_pSizerDataGrid = new wxBoxSizer( wxVERTICAL );
	
	
	m_pSizerCamposBusqueda->Add( m_pSizerDataGrid, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer20->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pBDescargar = new wxButton( m_pPanelPACS, wxID_ANY, _("Download"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBDescargar->Enable( false );
	
	bSizer20->Add( m_pBDescargar, 0, wxALIGN_BOTTOM|wxALIGN_RIGHT|wxTOP|wxLEFT, 5 );
	
	
	m_pSizerCamposBusqueda->Add( bSizer20, 0, wxEXPAND, 5 );
	
	
	m_pPanelPACS->SetSizer( m_pSizerCamposBusqueda );
	m_pPanelPACS->Layout();
	m_pSizerCamposBusqueda->Fit( m_pPanelPACS );
	bSizer18->Add( m_pPanelPACS, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	
	m_pBodyPanel->SetSizer( bSizer18 );
	m_pBodyPanel->Layout();
	bSizer18->Fit( m_pBodyPanel );
	m_pMainSizer->Add( m_pBodyPanel, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pFooter->Hide();
	
	wxBoxSizer* m_pSizerBotoneraVentana;
	m_pSizerBotoneraVentana = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_pSizerBotoneraVentana->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pBotonClose = new wxButton( m_pFooter, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSizerBotoneraVentana->Add( m_pBotonClose, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_pFooter->SetSizer( m_pSizerBotoneraVentana );
	m_pFooter->Layout();
	m_pSizerBotoneraVentana->Fit( m_pFooter );
	m_pMainSizer->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GinkgoCloudSearchDialogBase::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ) );
	m_pFieldCombo->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnComboField ), NULL, this );
	m_pTextControlField->Connect( wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnCancelClick ), NULL, this );
	m_pTextControlField->Connect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnBusquedaClick ), NULL, this );
	m_pTextControlField->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnBusquedaClick ), NULL, this );
	m_pAnyDate->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pToday->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pYesterday->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pLastWeek->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pLastMonth->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pLastThreeMonths->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pBetween->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaDesde->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( GinkgoCloudSearchDialogBase::OnFechaDesdeDateChanged ), NULL, this );
	m_pTextControlFechaDesde->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaHasta->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( GinkgoCloudSearchDialogBase::OnFechaHastaDateChanged ), NULL, this );
	m_pTextControlFechaHasta->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pBDescargar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnDescargarClick ), NULL, this );
	m_pBotonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnCloseClick ), NULL, this );
}

GinkgoCloudSearchDialogBase::~GinkgoCloudSearchDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GinkgoCloudSearchDialogBase::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ) );
	m_pFieldCombo->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnComboField ), NULL, this );
	m_pTextControlField->Disconnect( wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnCancelClick ), NULL, this );
	m_pTextControlField->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnBusquedaClick ), NULL, this );
	m_pTextControlField->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnBusquedaClick ), NULL, this );
	m_pAnyDate->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pToday->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pYesterday->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pLastWeek->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pLastMonth->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pLastThreeMonths->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pBetween->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaDesde->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( GinkgoCloudSearchDialogBase::OnFechaDesdeDateChanged ), NULL, this );
	m_pTextControlFechaDesde->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaHasta->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( GinkgoCloudSearchDialogBase::OnFechaHastaDateChanged ), NULL, this );
	m_pTextControlFechaHasta->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( GinkgoCloudSearchDialogBase::OnKeyDownFormulario ), NULL, this );
	m_pBDescargar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnDescargarClick ), NULL, this );
	m_pBotonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GinkgoCloudSearchDialogBase::OnCloseClick ), NULL, this );
	
}

FileLinkDialogBase::FileLinkDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 650,350 ), wxDefaultSize );
	
	wxBoxSizer* bSizerPrincipal;
	bSizerPrincipal = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Ginkgo Cloud") );
	
	bSizerPrincipal->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelSuperior = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelSuperior->SetToolTip( _("Ginkgo Cloud link") );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText6 = new wxStaticText( m_pPanelSuperior, wxID_ANY, _("File link:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer81->Add( m_staticText6, 0, wxALL, 5 );
	
	m_pFileLink = new wxTextCtrl( m_pPanelSuperior, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer81->Add( m_pFileLink, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText7 = new wxStaticText( m_pPanelSuperior, wxID_ANY, _("You can copy and paste link to use it in e-mails and web pages, you have to install Ginkgo CADx to download and visualize it."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( 490 );
	m_staticText7->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 93, 90, false, wxEmptyString ) );
	
	bSizer81->Add( m_staticText7, 0, wxALL, 5 );
	
	
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
	
	m_button7 = new wxButton( m_pFooter, wxID_ANY, _("Send link by e-mail"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_button7, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pCopyClipboard = new wxButton( m_pFooter, wxID_ANY, _("Copy to clipboard"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_pCopyClipboard, 0, 0, 5 );
	
	m_buttonAceptar = new wxButton( m_pFooter, wxID_CLOSE, _("&Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAceptar->SetDefault(); 
	bSizer8->Add( m_buttonAceptar, 0, wxLEFT, 5 );
	
	
	m_pFooter->SetSizer( bSizer8 );
	m_pFooter->Layout();
	bSizer8->Fit( m_pFooter );
	bSizerPrincipal->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerPrincipal );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pBody->Connect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_pPanelSuperior->Connect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_pFileLink->Connect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_pFooter->Connect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FileLinkDialogBase::OnSendMail ), NULL, this );
	m_pCopyClipboard->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FileLinkDialogBase::OnCopyClipboard ), NULL, this );
	m_pCopyClipboard->Connect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_buttonAceptar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FileLinkDialogBase::OnAccept ), NULL, this );
	m_buttonAceptar->Connect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
}

FileLinkDialogBase::~FileLinkDialogBase()
{
	// Disconnect Events
	m_pBody->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_pPanelSuperior->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_pFileLink->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_pFooter->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FileLinkDialogBase::OnSendMail ), NULL, this );
	m_pCopyClipboard->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FileLinkDialogBase::OnCopyClipboard ), NULL, this );
	m_pCopyClipboard->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	m_buttonAceptar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FileLinkDialogBase::OnAccept ), NULL, this );
	m_buttonAceptar->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( FileLinkDialogBase::OnKeyUp ), NULL, this );
	
}
