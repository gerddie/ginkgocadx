///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "acceptlicensedialogbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

AcceptLicenseDialogBase::AcceptLicenseDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxDefaultSize, wxDefaultSize );

        wxBoxSizer* bSizerPrincipal;
        bSizerPrincipal = new wxBoxSizer( wxVERTICAL );

        m_pHeader = new HeaderPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pHeader->SetToolTip( _("License agreement") );

        bSizerPrincipal->Add( m_pHeader, 0, wxEXPAND, 5 );

        m_pBody = new BodyPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer4;
        bSizer4 = new wxBoxSizer( wxVERTICAL );

        m_pTextoLicencia = new wxTextCtrl( m_pBody, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
        bSizer4->Add( m_pTextoLicencia, 1, wxEXPAND, 5 );

        m_pBody->SetSizer( bSizer4 );
        m_pBody->Layout();
        bSizer4->Fit( m_pBody );
        bSizerPrincipal->Add( m_pBody, 1, wxEXPAND, 5 );

        m_pFooter = new FooterPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer5;
        bSizer5 = new wxBoxSizer( wxVERTICAL );

        m_staticText1 = new wxStaticText( m_pFooter, wxID_ANY, _("If you accept the terms of the agreement, click \"I Accept the license\" to continue. You must accept the license to start using Ginkgo CADx."), wxDefaultPosition, wxDefaultSize, 0 );
        m_staticText1->Wrap( -1 );
        bSizer5->Add( m_staticText1, 0, wxALL, 5 );

        wxBoxSizer* bSizer7;
        bSizer7 = new wxBoxSizer( wxHORIZONTAL );

        wxBoxSizer* bSizer51;
        bSizer51 = new wxBoxSizer( wxVERTICAL );

        m_pCheckForUpdates = new wxCheckBox( m_pFooter, wxID_ANY, _("Check for updates automatically"), wxDefaultPosition, wxDefaultSize, 0 );
        m_pCheckForUpdates->SetValue(true);
        bSizer51->Add( m_pCheckForUpdates, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

        m_pCheckAnonUsageStats = new wxCheckBox( m_pFooter, wxID_ANY, _("Help us sending anonymous usage data (no patient data will be sent)"), wxDefaultPosition, wxDefaultSize, 0 );
        m_pCheckAnonUsageStats->SetValue(true);
        bSizer51->Add( m_pCheckAnonUsageStats, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

        bSizer7->Add( bSizer51, 1, wxEXPAND, 5 );


        bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );

        m_buttonAceptar = new wxButton( m_pFooter, wxID_OK, _("I Accept the license"), wxDefaultPosition, wxDefaultSize, 0 );
        m_buttonAceptar->SetDefault();
        bSizer7->Add( m_buttonAceptar, 0, wxRIGHT|wxLEFT|wxALIGN_BOTTOM, 5 );

        m_buttonCancelar = new wxButton( m_pFooter, wxID_CANCEL, _("I do NOT accept the license"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer7->Add( m_buttonCancelar, 0, wxLEFT|wxALIGN_BOTTOM, 5 );

        bSizer5->Add( bSizer7, 1, wxEXPAND, 5 );

        m_pFooter->SetSizer( bSizer5 );
        m_pFooter->Layout();
        bSizer5->Fit( m_pFooter );
        bSizerPrincipal->Add( m_pFooter, 0, wxALL|wxEXPAND, 0 );

        this->SetSizer( bSizerPrincipal );
        this->Layout();

        this->Centre( wxBOTH );

        // Connect Events
        this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( AcceptLicenseDialogBase::OnAnyKeyDown ) );
        this->Connect( wxEVT_SIZE, wxSizeEventHandler( AcceptLicenseDialogBase::OnSize ) );
        m_buttonAceptar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcceptLicenseDialogBase::OnAceptarClick ), NULL, this );
}

AcceptLicenseDialogBase::~AcceptLicenseDialogBase()
{
        // Disconnect Events
        this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( AcceptLicenseDialogBase::OnAnyKeyDown ) );
        this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( AcceptLicenseDialogBase::OnSize ) );
        m_buttonAceptar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcceptLicenseDialogBase::OnAceptarClick ), NULL, this );

}
