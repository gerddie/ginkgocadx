///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "startupformbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

StartUpFormBase::StartUpFormBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
        this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
        this->SetMinSize( wxSize( 548,288 ) );

        wxBoxSizer*bSizer5 = new wxBoxSizer( wxVERTICAL );
        m_pSizerToolBar = new wxBoxSizer( wxHORIZONTAL );

        bSizer5->Add( m_pSizerToolBar, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

        wxBoxSizer* bSizer13 = new wxBoxSizer( wxHORIZONTAL );
        m_pSizerCenter = new wxBoxSizer( wxHORIZONTAL );

        wxBoxSizer* bSizer51 = new wxBoxSizer( wxVERTICAL );

        wxBoxSizer* bSizer6 = new wxBoxSizer( wxHORIZONTAL );

        m_staticText2 = new wxStaticText( this, wxID_ANY, _("No suitable for diagnostic usage."), wxDefaultPosition, wxDefaultSize, 0 );
        m_staticText2->Wrap( 360 );
        m_staticText2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
        m_staticText2->SetForegroundColour( wxColour( 255, 0, 0 ) );

        bSizer6->Add( m_staticText2, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );

        bSizer51->Add( bSizer6, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

        m_pSizerCenter->Add( bSizer51, 1, wxEXPAND, 5 );


        bSizer13->Add( m_pSizerCenter, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );


        bSizer5->Add( bSizer13, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 20 );


        bSizer5->Add( 0, 15, 0, wxEXPAND, 5 );


        this->SetSizer( bSizer5 );
        this->Layout();

        // Connect Events
        this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( StartUpFormBase::OnEraseBackground ) );
}

StartUpFormBase::~StartUpFormBase()
{
        // Disconnect Events
        this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( StartUpFormBase::OnEraseBackground ) );

}
