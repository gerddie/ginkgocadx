///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "dialogopathssobreescribirbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

OverwriteDialogBase::OverwriteDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 700,209 ), wxDefaultSize );

        wxBoxSizer* bSizer3;
        bSizer3 = new wxBoxSizer( wxVERTICAL );

        m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer6;
        bSizer6 = new wxBoxSizer( wxVERTICAL );

        m_pPanelOverwrite = new TitledPanel( m_pBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pPanelOverwrite->SetToolTip( _("Overwrite") );

        wxBoxSizer* bSizer31;
        bSizer31 = new wxBoxSizer( wxVERTICAL );

        m_pTextMessage = new wxStaticText( m_pPanelOverwrite, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        m_pTextMessage->Wrap( -1 );
        bSizer31->Add( m_pTextMessage, 0, 0, 0 );

        m_staticText1 = new wxStaticText( m_pPanelOverwrite, wxID_ANY, _("Would you like to overwrite them?"), wxDefaultPosition, wxDefaultSize, 0 );
        m_staticText1->Wrap( 440 );
        bSizer31->Add( m_staticText1, 0, wxBOTTOM, 10 );

        m_pPathList = new wxListBox( m_pPanelOverwrite, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
        m_pPathList->Hide();

        bSizer31->Add( m_pPathList, 1, wxEXPAND|wxTOP, 5 );

        m_pPanelOverwrite->SetSizer( bSizer31 );
        m_pPanelOverwrite->Layout();
        bSizer31->Fit( m_pPanelOverwrite );
        bSizer6->Add( m_pPanelOverwrite, 1, wxEXPAND, 5 );

        m_pBody->SetSizer( bSizer6 );
        m_pBody->Layout();
        bSizer6->Fit( m_pBody );
        bSizer3->Add( m_pBody, 1, wxEXPAND, 5 );

        m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer4;
        bSizer4 = new wxBoxSizer( wxHORIZONTAL );


        bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

        m_button1 = new wxButton( m_pFooter, wxID_OK, _("Overwrite"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer4->Add( m_button1, 0, wxRIGHT, 5 );

        m_button2 = new wxButton( m_pFooter, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button2->SetDefault();
        bSizer4->Add( m_button2, 0, wxLEFT, 5 );

        m_pFooter->SetSizer( bSizer4 );
        m_pFooter->Layout();
        bSizer4->Fit( m_pFooter );
        bSizer3->Add( m_pFooter, 0, wxEXPAND, 5 );

        this->SetSizer( bSizer3 );
        this->Layout();
        bSizer3->Fit( this );

        this->Centre( wxBOTH );
}

OverwriteDialogBase::~OverwriteDialogBase()
{
}
