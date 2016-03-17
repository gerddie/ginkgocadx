///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "opendialogsbase.h"

///////////////////////////////////////////////////////////////////////////

DialogoDicomDirBase::DialogoDicomDirBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 880,521 ), wxDefaultSize );

        wxBoxSizer* bSizer15;
        bSizer15 = new wxBoxSizer( wxVERTICAL );

        m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pHeader->SetToolTip( _("Acquire from DICOM Dir") );

        bSizer15->Add( m_pHeader, 0, wxEXPAND, 5 );

        m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer7;
        bSizer7 = new wxBoxSizer( wxVERTICAL );

        m_pResultsView = new wxDataViewCtrl(m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize,  wxDV_MULTIPLE|wxDV_ROW_LINES);
        bSizer7->Add( m_pResultsView , 1, wxALL|wxEXPAND, 5 );


        m_pBody->SetSizer( bSizer7 );
        m_pBody->Layout();
        bSizer7->Fit( m_pBody );
        bSizer15->Add( m_pBody, 1, wxEXPAND, 5 );

        m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer16;
        bSizer16 = new wxBoxSizer( wxHORIZONTAL );


        bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );

        m_button4 = new wxButton( m_pFooter, wxID_OK, _("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button4->SetDefault();
        bSizer16->Add( m_button4, 0, wxRIGHT, 5 );

        m_button13 = new wxButton( m_pFooter, wxID_ANY, _("Link"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer16->Add( m_button13, 0, wxALIGN_CENTER_VERTICAL, 5 );

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

        // Connect Events
        m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDicomDirBase::OnCopyClick ), NULL, this );
        m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDicomDirBase::OnLinkClick ), NULL, this );
}

DialogoDicomDirBase::~DialogoDicomDirBase()
{
        // Disconnect Events
        m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDicomDirBase::OnCopyClick ), NULL, this );
        m_button13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDicomDirBase::OnLinkClick ), NULL, this );

}

SelectDriveBase::SelectDriveBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 380,220 ), wxDefaultSize );

        wxBoxSizer* bSizer15;
        bSizer15 = new wxBoxSizer( wxVERTICAL );

        m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer7;
        bSizer7 = new wxBoxSizer( wxVERTICAL );

        m_pPanelMessage = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pPanelMessage->SetToolTip( _("Select drive") );

        wxBoxSizer* bSizer4;
        bSizer4 = new wxBoxSizer( wxVERTICAL );

        m_pLabel = new wxStaticText( m_pPanelMessage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        m_pLabel->Wrap( -1 );
        bSizer4->Add( m_pLabel, 0, wxALL, 5 );

        wxArrayString m_pUnitsChoices;
        m_pUnits = new wxChoice( m_pPanelMessage, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pUnitsChoices, 0 );
        m_pUnits->SetSelection( 0 );
        bSizer4->Add( m_pUnits, 0, wxALL, 5 );


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

        m_button4 = new wxButton( m_pFooter, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button4->SetDefault();
        bSizer16->Add( m_button4, 0, wxRIGHT, 5 );

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

        // Connect Events
        m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectDriveBase::OnEndClick ), NULL, this );
}

SelectDriveBase::~SelectDriveBase()
{
        // Disconnect Events
        m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectDriveBase::OnEndClick ), NULL, this );

}

CopyOrLinkBase::CopyOrLinkBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 380,220 ), wxDefaultSize );

        wxBoxSizer* bSizer15;
        bSizer15 = new wxBoxSizer( wxVERTICAL );

        m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer7;
        bSizer7 = new wxBoxSizer( wxVERTICAL );

        m_pPanelMessage = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pPanelMessage->SetToolTip( _("Copy or link") );

        wxBoxSizer* bSizer4;
        bSizer4 = new wxBoxSizer( wxVERTICAL );

        m_pLabel = new wxStaticText( m_pPanelMessage, wxID_ANY, _("Do you want to copy or link files?"), wxDefaultPosition, wxDefaultSize, 0 );
        m_pLabel->Wrap( -1 );
        bSizer4->Add( m_pLabel, 0, wxALL, 5 );


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

        m_button4 = new wxButton( m_pFooter, wxID_ANY, _("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button4->SetDefault();
        bSizer16->Add( m_button4, 0, wxRIGHT, 5 );

        m_button5 = new wxButton( m_pFooter, wxID_ANY, _("Link"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer16->Add( m_button5, 0, wxLEFT, 5 );


        m_pFooter->SetSizer( bSizer16 );
        m_pFooter->Layout();
        bSizer16->Fit( m_pFooter );
        bSizer15->Add( m_pFooter, 0, wxEXPAND, 5 );


        this->SetSizer( bSizer15 );
        this->Layout();
        bSizer15->Fit( this );

        this->Centre( wxBOTH );

        // Connect Events
        m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CopyOrLinkBase::OnCopy ), NULL, this );
        m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CopyOrLinkBase::OnLink ), NULL, this );
}

CopyOrLinkBase::~CopyOrLinkBase()
{
        // Disconnect Events
        m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CopyOrLinkBase::OnCopy ), NULL, this );
        m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CopyOrLinkBase::OnLink ), NULL, this );

}
