///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "dialogoadquisicionbase.h"

///////////////////////////////////////////////////////////////////////////

DialogoAdquisicionBase::DialogoAdquisicionBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,300 ), wxDefaultSize );
	
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("PACS") );
	
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
	
	m_pPatientLabelStr = new wxStaticText( m_pPanelPACS, wxID_ANY, _("Patient"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pPatientLabelStr->Wrap( -1 );
	m_pPatientLabelStr->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer33->Add( m_pPatientLabelStr, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pFieldCombo = new wxComboBox( m_pPanelPACS, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	m_pFieldCombo->Append( _("Id") );
	m_pFieldCombo->Append( _("Name") );
	m_pFieldCombo->Append( _("Acc#") );
	m_pFieldCombo->Append( _("Study UID") );
	m_pFieldCombo->SetMinSize( wxSize( 80,-1 ) );
	
	bSizer23->Add( m_pFieldCombo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSearchPanel = new wxPanel( m_pPanelPACS, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pTextControlField = new wxSearchCtrl( m_pSearchPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	#ifndef __WXMAC__
	m_pTextControlField->ShowSearchButton( true );
	#endif
	m_pTextControlField->ShowCancelButton( false );
	bSizer28->Add( m_pTextControlField, 1, wxALL|wxEXPAND, 5 );
	
	m_pLabelRestricted = new wxStaticText( m_pSearchPanel, wxID_ANY, _("PACS acquisition is restricted"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelRestricted->Wrap( -1 );
	m_pLabelRestricted->Hide();
	
	bSizer28->Add( m_pLabelRestricted, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSizerAnimation = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer28->Add( m_pSizerAnimation, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	m_pSearchPanel->SetSizer( bSizer28 );
	m_pSearchPanel->Layout();
	bSizer28->Fit( m_pSearchPanel );
	bSizer23->Add( m_pSearchPanel, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer33->Add( bSizer23, 1, wxEXPAND, 5 );
	
	
	bSizer32->Add( bSizer33, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pServerLabel = new wxStaticText( m_pPanelPACS, wxID_ANY, _("Server"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pServerLabel->Wrap( -1 );
	m_pServerLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer31->Add( m_pServerLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPACSListaServidores = new wxComboBox( m_pPanelPACS, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	bSizer31->Add( m_pPACSListaServidores, 1, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer32->Add( bSizer31, 1, wxEXPAND, 5 );
	
	
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
	
	m_pTodayAM = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Today AM"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pTodayAM, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pTodayPM = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Today PM"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pTodayPM, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_pToday = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Today"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( m_pToday, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	
	bSizer17->Add( bSizer181, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	m_pYesterday = new wxRadioButton( m_pPanelPACS, wxID_ANY, _("Yesterday"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_pYesterday, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 2 );
	
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
	
	m_pDataViewSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_pSizerCamposBusqueda->Add( m_pDataViewSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( m_pPanelPACS, wxID_ANY, _("Stored queries"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer20->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pComboQueries = new wxComboBox( m_pPanelPACS, wxID_ANY, _("Select stored queries..."), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	m_pComboQueries->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer20->Add( m_pComboQueries, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pStoredQueriesButtonBar = new wxGinkgoToolBar(m_pPanelPACS, wxID_ANY);
	bSizer20->Add( m_pStoredQueriesButtonBar, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer20->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pBLink = new wxButton( m_pPanelPACS, wxID_ANY, _("Link"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBLink->Enable( false );
	m_pBLink->SetToolTip( _("Available only in WADO servers") );
	
	bSizer20->Add( m_pBLink, 0, wxALIGN_CENTER_VERTICAL|wxTOP, 5 );
	
	m_pBDescargar = new wxButton( m_pPanelPACS, wxID_ANY, _("Download"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBDescargar->Enable( false );
	
	bSizer20->Add( m_pBDescargar, 0, wxALIGN_BOTTOM|wxALIGN_RIGHT|wxTOP|wxLEFT, 5 );
	
	m_pBotonClose = new wxButton( m_pPanelPACS, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBotonClose->Hide();
	
	bSizer20->Add( m_pBotonClose, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_pSizerCamposBusqueda->Add( bSizer20, 0, wxEXPAND, 5 );
	
	
	m_pPanelPACS->SetSizer( m_pSizerCamposBusqueda );
	m_pPanelPACS->Layout();
	m_pSizerCamposBusqueda->Fit( m_pPanelPACS );
	bSizer18->Add( m_pPanelPACS, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	
	m_pBodyPanel->SetSizer( bSizer18 );
	m_pBodyPanel->Layout();
	bSizer18->Fit( m_pBodyPanel );
	m_pMainSizer->Add( m_pBodyPanel, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DialogoAdquisicionBase::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ) );
	m_pFieldCombo->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoAdquisicionBase::OnComboField ), NULL, this );
	m_pTextControlField->Connect( wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler( DialogoAdquisicionBase::OnCancelClick ), NULL, this );
	m_pTextControlField->Connect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( DialogoAdquisicionBase::OnBusquedaClick ), NULL, this );
	m_pTextControlField->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogoAdquisicionBase::OnBusquedaClick ), NULL, this );
	m_pPACSListaServidores->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoAdquisicionBase::OnPACSChanged ), NULL, this );
	m_pAnyDate->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTodayAM->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTodayPM->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pToday->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pYesterday->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pLastWeek->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pLastMonth->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pLastThreeMonths->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pBetween->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaDesde->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( DialogoAdquisicionBase::OnFechaDesdeDateChanged ), NULL, this );
	m_pTextControlFechaDesde->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaHasta->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( DialogoAdquisicionBase::OnFechaHastaDateChanged ), NULL, this );
	m_pTextControlFechaHasta->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pComboQueries->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoAdquisicionBase::OnComboStoredQueriesClick ), NULL, this );
	m_pBLink->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAdquisicionBase::OnLinkClick ), NULL, this );
	m_pBDescargar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAdquisicionBase::OnDescargarClick ), NULL, this );
	m_pBotonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAdquisicionBase::OnCloseClick ), NULL, this );
}

DialogoAdquisicionBase::~DialogoAdquisicionBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DialogoAdquisicionBase::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ) );
	m_pFieldCombo->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoAdquisicionBase::OnComboField ), NULL, this );
	m_pTextControlField->Disconnect( wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler( DialogoAdquisicionBase::OnCancelClick ), NULL, this );
	m_pTextControlField->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( DialogoAdquisicionBase::OnBusquedaClick ), NULL, this );
	m_pTextControlField->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogoAdquisicionBase::OnBusquedaClick ), NULL, this );
	m_pPACSListaServidores->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoAdquisicionBase::OnPACSChanged ), NULL, this );
	m_pAnyDate->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTodayAM->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTodayPM->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pToday->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pYesterday->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pLastWeek->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pLastMonth->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pLastThreeMonths->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pBetween->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaDesde->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( DialogoAdquisicionBase::OnFechaDesdeDateChanged ), NULL, this );
	m_pTextControlFechaDesde->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pTextControlFechaHasta->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( DialogoAdquisicionBase::OnFechaHastaDateChanged ), NULL, this );
	m_pTextControlFechaHasta->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( DialogoAdquisicionBase::OnKeyDownFormulario ), NULL, this );
	m_pComboQueries->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoAdquisicionBase::OnComboStoredQueriesClick ), NULL, this );
	m_pBLink->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAdquisicionBase::OnLinkClick ), NULL, this );
	m_pBDescargar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAdquisicionBase::OnDescargarClick ), NULL, this );
	m_pBotonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAdquisicionBase::OnCloseClick ), NULL, this );
	
}

DialogoNotFoundBase::DialogoNotFoundBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 285,164 ), wxDefaultSize );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_panel5 = new TitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel5->SetToolTip( _("Series does not exist") );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText10 = new wxStaticText( m_panel5, wxID_ANY, _("These series have been deleted from the local history"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( 440 );
	bSizer13->Add( m_staticText10, 0, wxALL, 5 );
	
	m_pListaSeries = new wxListBox( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer13->Add( m_pListaSeries, 1, wxEXPAND|wxTOP, 0 );
	
	
	m_panel5->SetSizer( bSizer13 );
	m_panel5->Layout();
	bSizer13->Fit( m_panel5 );
	bSizer12->Add( m_panel5, 1, wxEXPAND | wxALL, 5 );
	
	m_pPanelOkCancel = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pButtonOk = new wxButton( m_pPanelOkCancel, wxID_OK, _("Continue"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pButtonOk->SetDefault(); 
	bSizer14->Add( m_pButtonOk, 0, wxRIGHT, 5 );
	
	m_pButtonCancel = new wxButton( m_pPanelOkCancel, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_pButtonCancel, 0, wxLEFT, 5 );
	
	
	m_pPanelOkCancel->SetSizer( bSizer14 );
	m_pPanelOkCancel->Layout();
	bSizer14->Fit( m_pPanelOkCancel );
	bSizer12->Add( m_pPanelOkCancel, 0, wxEXPAND, 5 );
	
	m_pPanelOk = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button10 = new wxButton( m_pPanelOk, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button10->SetDefault(); 
	bSizer16->Add( m_button10, 0, 0, 5 );
	
	
	m_pPanelOk->SetSizer( bSizer16 );
	m_pPanelOk->Layout();
	bSizer16->Fit( m_pPanelOk );
	bSizer12->Add( m_pPanelOk, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer12 );
	this->Layout();
	bSizer12->Fit( this );
	
	this->Centre( wxBOTH );
}

DialogoNotFoundBase::~DialogoNotFoundBase()
{
}

DownloadAgainDialogBase::DownloadAgainDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 415,300 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Download again") );
	
	m_pMainSizer->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	m_pFormPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pFormPanel->SetToolTip( _("Download again") );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabelMessage = new wxStaticText( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelMessage->Wrap( 350 );
	bSizer19->Add( m_pLabelMessage, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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
	
	m_button9 = new wxButton( m_panel6, wxID_ANY, _("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button9->SetDefault(); 
	bSizer14->Add( m_button9, 0, wxALL, 5 );
	
	m_pButtonOk = new wxButton( m_panel6, wxID_OK, _("Download"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	// Connect Events
	m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DownloadAgainDialogBase::OnOpenClick ), NULL, this );
}

DownloadAgainDialogBase::~DownloadAgainDialogBase()
{
	// Disconnect Events
	m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DownloadAgainDialogBase::OnOpenClick ), NULL, this );
	
}

QueryNameDialogBase::QueryNameDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 415,250 ), wxDefaultSize );
	
	wxBoxSizer* m_pMainSizer;
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Set query name") );
	
	m_pMainSizer->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	m_pFormPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pFormPanel->SetToolTip( _("Set query name") );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabelMessage = new wxStaticText( m_pFormPanel, wxID_ANY, _("Set query name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelMessage->Wrap( 350 );
	bSizer19->Add( m_pLabelMessage, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pQueryName = new wxTextCtrl( m_pFormPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_pQueryName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pButtonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( QueryNameDialogBase::OnOkClick ), NULL, this );
}

QueryNameDialogBase::~QueryNameDialogBase()
{
	// Disconnect Events
	m_pButtonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( QueryNameDialogBase::OnOkClick ), NULL, this );
	
}
