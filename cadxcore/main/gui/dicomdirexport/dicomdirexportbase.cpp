///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "dicomdirexportbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

ExportDicomBase::ExportDicomBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 580,270 ), wxDefaultSize );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pHeader->SetToolTip( _("Export DICOM") );
	
	bSizer13->Add( m_pHeader, 0, wxEXPAND, 5 );
	
	m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_pAnonymize = new SubTitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pAnonymize->SetToolTip( _("Anonymize") );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_pCheckPatientId = new wxCheckBox( m_pAnonymize, wxID_ANY, _("Patient Id"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_pCheckPatientId, 0, wxALL, 5 );
	
	m_pCheckReferringPhysician = new wxCheckBox( m_pAnonymize, wxID_ANY, _("Refering physician"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_pCheckReferringPhysician, 0, wxALL, 5 );
	
	m_pCheckInstitutions = new wxCheckBox( m_pAnonymize, wxID_ANY, _("Institutions"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_pCheckInstitutions, 0, wxALL, 5 );
	
	bSizer3->Add( bSizer4, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_pCheckPatientName = new wxCheckBox( m_pAnonymize, wxID_ANY, _("Patient's name and surname"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_pCheckPatientName, 0, wxALL, 5 );
	
	m_pCheckComments = new wxCheckBox( m_pAnonymize, wxID_ANY, _("Comments"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_pCheckComments, 0, wxALL, 5 );
	
	m_pCheckPrivateTags = new wxCheckBox( m_pAnonymize, wxID_ANY, _("Ginkgo CADx private tags"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_pCheckPrivateTags, 0, wxALL, 5 );
	
	bSizer3->Add( bSizer5, 1, wxEXPAND, 5 );
	
	m_pAnonymize->SetSizer( bSizer3 );
	m_pAnonymize->Layout();
	bSizer3->Fit( m_pAnonymize );
	bSizer8->Add( m_pAnonymize, 0, wxEXPAND, 5 );
	
	m_pBody->SetSizer( bSizer8 );
	m_pBody->Layout();
	bSizer8->Fit( m_pBody );
	bSizer13->Add( m_pBody, 1, wxEXPAND, 5 );
	
	m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button1 = new wxButton( m_pFooter, wxID_ANY, _("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_button1, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_button2 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button2->SetDefault(); 
	bSizer10->Add( m_button2, 0, wxRIGHT, 5 );
	
	m_pFooter->SetSizer( bSizer10 );
	m_pFooter->Layout();
	bSizer10->Fit( m_pFooter );
	bSizer13->Add( m_pFooter, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer13 );
	this->Layout();
	bSizer13->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDicomBase::OnExportClick ), NULL, this );
}

ExportDicomBase::~ExportDicomBase()
{
	// Disconnect Events
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDicomBase::OnExportClick ), NULL, this );
	
}
