///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "pasosimportacionbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

SelectImagesImportationBase::SelectImagesImportationBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxScrolledWindow( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_panelStep0 = new TitledPanelSinBorder( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelStep0->SetToolTip( _("Selected images") );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer11->Add( m_pSizerButtons, 0, wxEXPAND, 5 );
	
	m_pImagenes = new wxThumbnailCtrl(m_panelStep0, -1, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxTH_MULTIPLE_SELECT|wxTH_TEXT_LABEL);
	
	m_pImagenes->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	bSizer11->Add( m_pImagenes, 1, wxEXPAND|wxALL, 1 );
	
	m_pPreviewPanel = new wxPanel( m_panelStep0, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPreviewPanel->Hide();
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );
	
	m_pPreview = new wxPrevisualizacionDicom(m_pPreviewPanel, -1);
	
	bSizer71->Add( m_pPreview, 1, wxEXPAND, 5 );
	
	
	m_pPreviewPanel->SetSizer( bSizer71 );
	m_pPreviewPanel->Layout();
	bSizer71->Fit( m_pPreviewPanel );
	bSizer11->Add( m_pPreviewPanel, 1, wxEXPAND | wxALL, 1 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pButtonPreview = new wxButton( m_panelStep0, wxID_ANY, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_pButtonPreview, 0, wxALL, 2 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pButtonRemoveAll = new wxButton( m_panelStep0, wxID_ANY, _("Remove all"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_pButtonRemoveAll, 0, wxALL|wxALIGN_RIGHT, 2 );
	
	
	bSizer11->Add( bSizer8, 0, wxEXPAND, 5 );
	
	
	m_panelStep0->SetSizer( bSizer11 );
	m_panelStep0->Layout();
	bSizer11->Fit( m_panelStep0 );
	bSizer7->Add( m_panelStep0, 1, wxEXPAND, 10 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	// Connect Events
	m_pButtonPreview->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectImagesImportationBase::OnPreviewClick ), NULL, this );
	m_pButtonRemoveAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectImagesImportationBase::OnRemoveAll ), NULL, this );
}

SelectImagesImportationBase::~SelectImagesImportationBase()
{
	// Disconnect Events
	m_pButtonPreview->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectImagesImportationBase::OnPreviewClick ), NULL, this );
	m_pButtonRemoveAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectImagesImportationBase::OnRemoveAll ), NULL, this );
	
}

PasoPerfilImportacionBase::PasoPerfilImportacionBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 230, 230, 230 ) );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
}

PasoPerfilImportacionBase::~PasoPerfilImportacionBase()
{
}

PanelDatosGeneralesBase::PanelDatosGeneralesBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxScrolledWindow( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_panelDemographicData = new TitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelDemographicData->SetToolTip( _("Demographic data") );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer152;
	bSizer152 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( bSizer152, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 0, 2, 0, 0 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText311 = new wxStaticText( m_panelDemographicData, wxID_ANY, _("Identificator *"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText311->Wrap( -1 );
	fgSizer2->Add( m_staticText311, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pPacienteId = new wxTextCtrl( m_panelDemographicData, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pPacienteId->SetMaxLength( 64 ); 
	bSizer3->Add( m_pPacienteId, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText17 = new wxStaticText( m_panelDemographicData, wxID_ANY, _("(ID or medical history number)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer3->Add( m_staticText17, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	fgSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_staticText23 = new wxStaticText( m_panelDemographicData, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer2->Add( m_staticText23, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPacienteNombre = new wxTextCtrl( m_panelDemographicData, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 125,-1 ), 0 );
	m_pPacienteNombre->SetMaxLength( 64 ); 
	fgSizer2->Add( m_pPacienteNombre, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText41 = new wxStaticText( m_panelDemographicData, wxID_ANY, _("Birth date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer2->Add( m_staticText41, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPacienteFechaNacimiento = new wxDatePickerCtrl( m_panelDemographicData, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_SHOWCENTURY );
	fgSizer2->Add( m_pPacienteFechaNacimiento, 0, wxALL, 5 );
	
	
	gSizer1->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dummy1 = new wxStaticText( m_panelDemographicData, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dummy1->Wrap( -1 );
	fgSizer3->Add( dummy1, 0, wxALL, 5 );
	
	dummy2 = new wxStaticText( m_panelDemographicData, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dummy2->Wrap( -1 );
	fgSizer3->Add( dummy2, 0, wxALL, 5 );
	
	m_staticText261 = new wxStaticText( m_panelDemographicData, wxID_ANY, _("Surname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText261->Wrap( -1 );
	fgSizer3->Add( m_staticText261, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pPatientSurname = new wxTextCtrl( m_panelDemographicData, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 125,-1 ), 0 );
	m_pPatientSurname->SetMaxLength( 64 ); 
	fgSizer3->Add( m_pPatientSurname, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText13 = new wxStaticText( m_panelDemographicData, wxID_ANY, _("Sex"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer3->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_pPacienteSexoChoices[] = { _("Unknown"), _("Male"), _("Female"), _("Others") };
	int m_pPacienteSexoNChoices = sizeof( m_pPacienteSexoChoices ) / sizeof( wxString );
	m_pPacienteSexo = new wxChoice( m_panelDemographicData, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pPacienteSexoNChoices, m_pPacienteSexoChoices, 0 );
	m_pPacienteSexo->SetSelection( 0 );
	fgSizer3->Add( m_pPacienteSexo, 0, wxALL, 5 );
	
	
	gSizer1->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	
	bSizer8->Add( gSizer1, 1, wxEXPAND, 5 );
	
	
	m_panelDemographicData->SetSizer( bSizer8 );
	m_panelDemographicData->Layout();
	bSizer8->Fit( m_panelDemographicData );
	bSizer15->Add( m_panelDemographicData, 0, wxEXPAND, 5 );
	
	m_pSep = new TitledPanelSeparatorV( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer15->Add( m_pSep, 0, wxEXPAND, 5 );
	
	m_panelStudyDetails = new TitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelStudyDetails->SetToolTip( _("Study Details") );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText22 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Patient"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	m_staticText22->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText22->SetMinSize( wxSize( 75,-1 ) );
	
	gbSizer2->Add( m_staticText22, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText11 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Weight"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	gbSizer2->Add( m_staticText11, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pPacientePeso = new wxTextCtrl( m_panelStudyDetails, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 35,-1 ), 0 );
	m_pPacientePeso->SetMaxLength( 0 ); 
	bSizer4->Add( m_pPacientePeso, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText18 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Kg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer4->Add( m_staticText18, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	gbSizer2->Add( bSizer4, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_staticText15 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	gbSizer2->Add( m_staticText15, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pPacienteAltura = new wxTextCtrl( m_panelStudyDetails, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 35,-1 ), 0 );
	m_pPacienteAltura->SetMaxLength( 0 ); 
	bSizer5->Add( m_pPacienteAltura, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText19 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("cm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer5->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	gbSizer2->Add( bSizer5, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_staticText221 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Study"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText221->Wrap( -1 );
	m_staticText221->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gbSizer2->Add( m_staticText221, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_pLabelAccNumber = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Accession Number"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelAccNumber->Wrap( -1 );
	gbSizer2->Add( m_pLabelAccNumber, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pAccessionNumber = new wxTextCtrl( m_panelStudyDetails, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pAccessionNumber->SetMaxLength( 64 ); 
	gbSizer2->Add( m_pAccessionNumber, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gbSizer2->Add( m_staticText2, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pEstudioFecha = new wxDatePickerCtrl( m_panelStudyDetails, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_SHOWCENTURY );
	gbSizer2->Add( m_pEstudioFecha, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText16 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	gbSizer2->Add( m_staticText16, wxGBPosition( 2, 3 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pEstudioTime = new wxMaskedTextCtrl(m_panelStudyDetails, -1, wxT("hh:nn:ss"), wxT(""), wxDefaultPosition, wxSize(80,-1), 0);
	gbSizer2->Add( m_pEstudioTime, wxGBPosition( 2, 4 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText3 = new wxStaticText( m_panelStudyDetails, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	gbSizer2->Add( m_staticText3, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pEstudioDescripcion = new wxTextCtrl( m_panelStudyDetails, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	gbSizer2->Add( m_pEstudioDescripcion, wxGBPosition( 3, 2 ), wxGBSpan( 2, 4 ), wxALL|wxEXPAND, 5 );
	
	
	gbSizer2->AddGrowableCol( 4 );
	
	bSizer13->Add( gbSizer2, 0, wxEXPAND|wxALL, 5 );
	
	m_pSeriesPanel = new wxPanel( m_panelStudyDetails, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* m_pSeriesSizer;
	m_pSeriesSizer = new wxGridBagSizer( 0, 0 );
	m_pSeriesSizer->SetFlexibleDirection( wxBOTH );
	m_pSeriesSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText231 = new wxStaticText( m_pSeriesPanel, wxID_ANY, _("Series"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText231->Wrap( -1 );
	m_staticText231->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText231->SetMinSize( wxSize( 75,-1 ) );
	
	m_pSeriesSizer->Add( m_staticText231, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText212 = new wxStaticText( m_pSeriesPanel, wxID_ANY, _("Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText212->Wrap( -1 );
	m_pSeriesSizer->Add( m_staticText212, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pSerieFecha = new wxDatePickerCtrl( m_pSeriesPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_SHOWCENTURY );
	m_pSeriesSizer->Add( m_pSerieFecha, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText1611 = new wxStaticText( m_pSeriesPanel, wxID_ANY, _("Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1611->Wrap( -1 );
	m_pSeriesSizer->Add( m_staticText1611, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_pSerieTime = new wxMaskedTextCtrl(m_pSeriesPanel, -1, wxT("hh:nn:ss"), wxT(""), wxDefaultPosition, wxSize(80,-1), 0);
	m_pSeriesSizer->Add( m_pSerieTime, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText312 = new wxStaticText( m_pSeriesPanel, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText312->Wrap( -1 );
	m_pSeriesSizer->Add( m_staticText312, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pSerieDescripcion = new wxTextCtrl( m_pSeriesPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	m_pSeriesSizer->Add( m_pSerieDescripcion, wxGBPosition( 1, 2 ), wxGBSpan( 2, 4 ), wxEXPAND|wxALL, 5 );
	
	
	m_pSeriesSizer->AddGrowableCol( 4 );
	
	bSizer151->Add( m_pSeriesSizer, 1, wxEXPAND|wxALL, 5 );
	
	
	m_pSeriesPanel->SetSizer( bSizer151 );
	m_pSeriesPanel->Layout();
	bSizer151->Fit( m_pSeriesPanel );
	bSizer13->Add( m_pSeriesPanel, 1, wxEXPAND, 5 );
	
	
	m_panelStudyDetails->SetSizer( bSizer13 );
	m_panelStudyDetails->Layout();
	bSizer13->Fit( m_panelStudyDetails );
	bSizer15->Add( m_panelStudyDetails, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer15 );
	this->Layout();
	bSizer15->Fit( this );
	
	// Connect Events
	m_pPacienteFechaNacimiento->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( PanelDatosGeneralesBase::OnDateChanged ), NULL, this );
	m_pEstudioFecha->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( PanelDatosGeneralesBase::OnDateChanged ), NULL, this );
	m_pEstudioTime->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( PanelDatosGeneralesBase::OnKillFocusEstudioTimeControl ), NULL, this );
	m_pSerieTime->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( PanelDatosGeneralesBase::OnKillFocusSerieTimeControl ), NULL, this );
}

PanelDatosGeneralesBase::~PanelDatosGeneralesBase()
{
	// Disconnect Events
	m_pPacienteFechaNacimiento->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( PanelDatosGeneralesBase::OnDateChanged ), NULL, this );
	m_pEstudioFecha->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( PanelDatosGeneralesBase::OnDateChanged ), NULL, this );
	m_pEstudioTime->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( PanelDatosGeneralesBase::OnKillFocusEstudioTimeControl ), NULL, this );
	m_pSerieTime->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( PanelDatosGeneralesBase::OnKillFocusSerieTimeControl ), NULL, this );
	
}
