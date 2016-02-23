///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "dicomdirmigrationbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

DicomDirMigrationDialogBase::DicomDirMigrationDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,360 ), wxDefaultSize );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	m_pMainPanel = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pMainPanel->SetToolTip( _("DICOM Dir migration") );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_pSizerOld = new wxBoxSizer( wxHORIZONTAL );
	
	m_pLabelVersion = new wxStaticText( m_pMainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelVersion->Wrap( -1 );
	m_pSizerOld->Add( m_pLabelVersion, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer11->Add( m_pSizerOld, 0, wxEXPAND, 5 );
	
	m_pSep1 = new SubTitledPanelSeparatorV( m_pMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer11->Add( m_pSep1, 0, wxEXPAND, 5 );
	
	m_panelMigration = new SubTitledPanel( m_pMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelMigration->SetToolTip( _("Migrate existing (recommended)") );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText31 = new wxStaticText( m_panelMigration, wxID_ANY, _("Migrate existing DICOM Dir, existing files will be added to new DICOM Dir. Do not close Ginkgo CADx until migration is finished."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( 610 );
	bSizer6->Add( m_staticText31, 0, wxALL, 5 );
	
	m_pRemoveFiles = new wxCheckBox( m_panelMigration, wxID_ANY, _("Remove old DICOM Dir after migration (will be renamed as *_old)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pRemoveFiles->SetValue(true); 
	bSizer6->Add( m_pRemoveFiles, 0, wxALL, 5 );
	
	fgSizer2->Add( bSizer6, 1, wxEXPAND, 5 );
	
	m_button5 = new wxButton( m_panelMigration, wxID_ANY, _("Continue..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_button5, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_panelMigration->SetSizer( fgSizer2 );
	m_panelMigration->Layout();
	fgSizer2->Fit( m_panelMigration );
	bSizer11->Add( m_panelMigration, 0, wxEXPAND, 5 );
	
	m_panelMigrationSep = new SubTitledPanelSeparatorV( m_pMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer11->Add( m_panelMigrationSep, 0, wxEXPAND, 5 );
	
	m_panel7 = new SubTitledPanel( m_pMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel7->SetToolTip( _("Select other") );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3 = new wxStaticText( m_panel7, wxID_ANY, _("Select other path, if path doesn't contains a Ginkgo CADx DICOM Dir, a new one will be created, else existing DICOM Dir will be used"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( 610 );
	fgSizer1->Add( m_staticText3, 0, wxALL, 5 );
	
	m_button4 = new wxButton( m_panel7, wxID_ANY, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_button4->SetDefault(); 
	fgSizer1->Add( m_button4, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_panel7->SetSizer( fgSizer1 );
	m_panel7->Layout();
	fgSizer1->Fit( m_panel7 );
	bSizer11->Add( m_panel7, 0, wxEXPAND, 5 );
	
	m_pMainPanel->SetSizer( bSizer11 );
	m_pMainPanel->Layout();
	bSizer11->Fit( m_pMainPanel );
	bSizer61->Add( m_pMainPanel, 1, wxEXPAND, 5 );
	
	m_pBody->SetSizer( bSizer61 );
	m_pBody->Layout();
	bSizer61->Fit( m_pBody );
	bSizer8->Add( m_pBody, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer8 );
	this->Layout();
	bSizer8->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DicomDirMigrationDialogBase::OnMigrate ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DicomDirMigrationDialogBase::OnSelectOther ), NULL, this );
}

DicomDirMigrationDialogBase::~DicomDirMigrationDialogBase()
{
	// Disconnect Events
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DicomDirMigrationDialogBase::OnMigrate ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DicomDirMigrationDialogBase::OnSelectOther ), NULL, this );
	
}
