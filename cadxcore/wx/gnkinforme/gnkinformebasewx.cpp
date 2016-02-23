///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "gnkinformebasewx.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

GNKInformeBaseWx::GNKInformeBaseWx( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxScrolledWindow( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 240, 240, 240 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelReport = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 669,-1 ), wxTAB_TRAVERSAL );
	m_pPanelReport->SetMinSize( wxSize( 669,-1 ) );
	m_pPanelReport->SetMaxSize( wxSize( 669,-1 ) );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_panel21 = new wxPanel( m_pPanelReport, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel21->SetBackgroundColour( wxColour( 90, 90, 90 ) );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->AddGrowableCol( 0 );
	gbSizer1->AddGrowableRow( 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pNombrePaciente = new wxStaticText( m_panel21, wxID_ANY, _("NAME SURNAME SECOND_SURNAME"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pNombrePaciente->Wrap( -1 );
	m_pNombrePaciente->SetFont( wxFont( 13, 70, 90, 90, false, wxEmptyString ) );
	m_pNombrePaciente->SetForegroundColour( wxColour( 255, 255, 255 ) );
	
	gbSizer1->Add( m_pNombrePaciente, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	m_pBitmapSexo = new wxStaticBitmap( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_pBitmapSexo, wxGBPosition( 0, 2 ), wxGBSpan( 2, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pIdPaciente = new wxStaticText( m_panel21, wxID_ANY, _("644654645"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pIdPaciente->Wrap( -1 );
	m_pIdPaciente->SetForegroundColour( wxColour( 255, 255, 255 ) );
	
	gbSizer1->Add( m_pIdPaciente, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	bSizer19->Add( gbSizer1, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	m_pPanelCuerpo = new BodyPanel( m_panel21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* pSizerCuerpo;
	pSizerCuerpo = new wxBoxSizer( wxVERTICAL );
	
	m_pPanelDatosDemograficos = new SubTitledPanel( m_pPanelCuerpo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelDatosDemograficos->SetToolTip( _("Demographic data") );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerDatosDemograficos = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_pSizerDatosDemograficos->AddGrowableCol( 0 );
	m_pSizerDatosDemograficos->SetFlexibleDirection( wxBOTH );
	m_pSizerDatosDemograficos->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	bSizer20->Add( m_pSizerDatosDemograficos, 1, wxLEFT, 5 );
	
	m_pPanelDatosDemograficos->SetSizer( bSizer20 );
	m_pPanelDatosDemograficos->Layout();
	bSizer20->Fit( m_pPanelDatosDemograficos );
	pSizerCuerpo->Add( m_pPanelDatosDemograficos, 0, wxEXPAND, 1 );
	
	m_panel22 = new SubTitledPanelSeparatorV( m_pPanelCuerpo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pSizerCuerpo->Add( m_panel22, 0, wxEXPAND, 5 );
	
	m_pSizerCustom = new wxBoxSizer( wxVERTICAL );
	
	pSizerCuerpo->Add( m_pSizerCustom, 0, wxEXPAND, 3 );
	
	m_pPanelComentarios = new SubTitledPanel( m_pPanelCuerpo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelComentarios->SetToolTip( _("Comments") );
	
	wxBoxSizer* pSizerComments;
	pSizerComments = new wxBoxSizer( wxVERTICAL );
	
	m_pComentarios = new wxTextCtrl( m_pPanelComentarios, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_pComentarios->SetMinSize( wxSize( -1,60 ) );
	
	pSizerComments->Add( m_pComentarios, 0, wxEXPAND|wxALL, 10 );
	
	m_pPanelComentarios->SetSizer( pSizerComments );
	m_pPanelComentarios->Layout();
	pSizerComments->Fit( m_pPanelComentarios );
	pSizerCuerpo->Add( m_pPanelComentarios, 0, wxEXPAND, 5 );
	
	m_panel221 = new SubTitledPanelSeparatorV( m_pPanelCuerpo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pSizerCuerpo->Add( m_panel221, 0, wxEXPAND, 5 );
	
	m_pPanelConsolidar = new SubTitledPanel( m_pPanelCuerpo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPanelConsolidar->SetToolTip( _("Commit inform") );
	
	wxBoxSizer* pSizerComments1;
	pSizerComments1 = new wxBoxSizer( wxVERTICAL );
	
	m_pListaConsolidaciones = new wxListCtrl( m_pPanelConsolidar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT );
	pSizerComments1->Add( m_pListaConsolidaciones, 0, wxEXPAND|wxALL, 10 );
	
	m_pPanelBoton = new wxPanel( m_pPanelConsolidar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer43->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pLabelStatus = new wxStaticText( m_pPanelBoton, wxID_ANY, _("Commited"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelStatus->Wrap( -1 );
	m_pLabelStatus->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer43->Add( m_pLabelStatus, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pBConsolidar = new wxButton( m_pPanelBoton, wxID_ANY, _("Commit inform"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer43->Add( m_pBConsolidar, 0, wxALL, 5 );
	
	m_pPanelBoton->SetSizer( bSizer43 );
	m_pPanelBoton->Layout();
	bSizer43->Fit( m_pPanelBoton );
	pSizerComments1->Add( m_pPanelBoton, 0, wxEXPAND | wxALL, 0 );
	
	m_pPanelConsolidar->SetSizer( pSizerComments1 );
	m_pPanelConsolidar->Layout();
	pSizerComments1->Fit( m_pPanelConsolidar );
	pSizerCuerpo->Add( m_pPanelConsolidar, 0, wxEXPAND, 5 );
	
	m_pPanelCuerpo->SetSizer( pSizerCuerpo );
	m_pPanelCuerpo->Layout();
	pSizerCuerpo->Fit( m_pPanelCuerpo );
	bSizer19->Add( m_pPanelCuerpo, 1, wxEXPAND|wxALL, 2 );
	
	m_panel21->SetSizer( bSizer19 );
	m_panel21->Layout();
	bSizer19->Fit( m_panel21 );
	bSizer2->Add( m_panel21, 0, wxEXPAND, 5 );
	
	m_pPanelReport->SetSizer( bSizer2 );
	m_pPanelReport->Layout();
	bSizer1->Add( m_pPanelReport, 1, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	// Connect Events
	m_pComentarios->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GNKInformeBaseWx::OnComentariosText ), NULL, this );
	m_pBConsolidar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GNKInformeBaseWx::OnConsolidarClick ), NULL, this );
}

GNKInformeBaseWx::~GNKInformeBaseWx()
{
	// Disconnect Events
	m_pComentarios->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GNKInformeBaseWx::OnComentariosText ), NULL, this );
	m_pBConsolidar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GNKInformeBaseWx::OnConsolidarClick ), NULL, this );
	
}
