///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "ventanacontrollogsbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

VentanaControlLogsBase::VentanaControlLogsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 500,200 ), wxDefaultSize );

        m_menubar1 = new wxMenuBar( 0 );
        m_menuRegistros = new wxMenu();
        m_menuItemGuardar = new wxMenuItem( m_menuRegistros, wxID_ANY, wxString( _("Save log...") ) + wxT('\t') + wxT("ALT+g"), wxEmptyString, wxITEM_NORMAL );
        m_menuRegistros->Append( m_menuItemGuardar );

        m_menuRegistros->AppendSeparator();

        m_menuItemLimpiar = new wxMenuItem( m_menuRegistros, wxID_ANY, wxString( _("Clear log") ) + wxT('\t') + wxT("ALT+l"), wxEmptyString, wxITEM_NORMAL );
        m_menuRegistros->Append( m_menuItemLimpiar );

        m_menuRegistros->AppendSeparator();

        m_menuItemSalir = new wxMenuItem( m_menuRegistros, wxID_ANY, wxString( _("Exit") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
        m_menuRegistros->Append( m_menuItemSalir );

        m_menubar1->Append( m_menuRegistros, _("Log") );

        this->SetMenuBar( m_menubar1 );

        wxBoxSizer* bSizer5;
        bSizer5 = new wxBoxSizer( wxVERTICAL );

        m_panel3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer7;
        bSizer7 = new wxBoxSizer( wxVERTICAL );

        m_pRegistros = new wxRichTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
        bSizer7->Add( m_pRegistros, 1, wxEXPAND | wxALL, 5 );


        m_panel3->SetSizer( bSizer7 );
        m_panel3->Layout();
        bSizer7->Fit( m_panel3 );
        bSizer5->Add( m_panel3, 1, wxEXPAND, 5 );

        m_panelNivel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* bSizer3;
        bSizer3 = new wxBoxSizer( wxHORIZONTAL );

        m_pNivel = new wxStaticText( m_panelNivel, wxID_ANY, _("Level"), wxDefaultPosition, wxDefaultSize, 0 );
        m_pNivel->Wrap( -1 );
        m_pNivel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );

        bSizer3->Add( m_pNivel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

        wxArrayString m_pComboNivelChoices;
        m_pComboNivel = new wxChoice( m_panelNivel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pComboNivelChoices, 0 );
        m_pComboNivel->SetSelection( 0 );
        bSizer3->Add( m_pComboNivel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


        bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

        m_pBLimpiar = new wxButton( m_panelNivel, wxID_ANY, _("Clear"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
        bSizer3->Add( m_pBLimpiar, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


        m_panelNivel->SetSizer( bSizer3 );
        m_panelNivel->Layout();
        bSizer3->Fit( m_panelNivel );
        bSizer5->Add( m_panelNivel, 0, wxEXPAND | wxALL, 0 );


        this->SetSizer( bSizer5 );
        this->Layout();

        this->Centre( wxBOTH );

        // Connect Events
        this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( VentanaControlLogsBase::OnActivate ) );
        this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( VentanaControlLogsBase::OnClose ) );
        this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ) );
        this->Connect( m_menuItemLimpiar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlLogsBase::OnLimpiarRegistros ) );
        m_panel3->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_panelNivel->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_pNivel->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_pComboNivel->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VentanaControlLogsBase::OnNivelChoice ), NULL, this );
        m_pComboNivel->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_pBLimpiar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VentanaControlLogsBase::OnLimpiarClick ), NULL, this );
        m_pBLimpiar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
}

VentanaControlLogsBase::~VentanaControlLogsBase()
{
        // Disconnect Events
        this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( VentanaControlLogsBase::OnActivate ) );
        this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( VentanaControlLogsBase::OnClose ) );
        this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ) );
        this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlLogsBase::OnLimpiarRegistros ) );
        m_panel3->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_panelNivel->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_pNivel->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_pComboNivel->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VentanaControlLogsBase::OnNivelChoice ), NULL, this );
        m_pComboNivel->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );
        m_pBLimpiar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VentanaControlLogsBase::OnLimpiarClick ), NULL, this );
        m_pBLimpiar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlLogsBase::OnKeyDown ), NULL, this );

}

ShowPlainTextDialog::ShowPlainTextDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
        this->SetSizeHints( wxSize( 500,242 ), wxDefaultSize );

        wxBoxSizer* bSizer5;
        bSizer5 = new wxBoxSizer( wxVERTICAL );

        m_pPanelLog = new SubTitledPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_pPanelLog->SetToolTip( _("LogText") );

        wxBoxSizer* bSizer7;
        bSizer7 = new wxBoxSizer( wxVERTICAL );

        m_pLogText = new wxTextCtrl( m_pPanelLog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
        m_pLogText->SetMaxLength( 0 );
        bSizer7->Add( m_pLogText, 1, wxEXPAND, 5 );


        m_pPanelLog->SetSizer( bSizer7 );
        m_pPanelLog->Layout();
        bSizer7->Fit( m_pPanelLog );
        bSizer5->Add( m_pPanelLog, 1, wxEXPAND | wxALL, 5 );

        m_button2 = new wxButton( this, wxID_CANCEL, _("Accept"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
        m_button2->SetDefault();
        bSizer5->Add( m_button2, 0, wxALL|wxALIGN_RIGHT, 5 );


        this->SetSizer( bSizer5 );
        this->Layout();

        this->Centre( wxBOTH );
}

ShowPlainTextDialog::~ShowPlainTextDialog()
{
}
