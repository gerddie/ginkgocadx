///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ventanacontrolhl7base.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

VentanaControlHL7Base::VentanaControlHL7Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,250 ), wxDefaultSize );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menuMensajes = new wxMenu();
	m_menuItemPurgarMensajes = new wxMenuItem( m_menuMensajes, wxID_ANY, wxString( _("Remove sent messages") ) + wxT('\t') + wxT("ALT+p"), wxEmptyString, wxITEM_NORMAL );
	m_menuMensajes->Append( m_menuItemPurgarMensajes );
	
	m_menuMensajes->AppendSeparator();
	
	m_menuItemSalir = new wxMenuItem( m_menuMensajes, wxID_ANY, wxString( _("Exit") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
	m_menuMensajes->Append( m_menuItemSalir );
	
	m_menubar1->Append( m_menuMensajes, _("Messages") ); 
	
	m_menu3 = new wxMenu();
	m_menuItemActualizar = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("Refresh") ) + wxT('\t') + wxT("F5"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItemActualizar );
	
	m_menubar1->Append( m_menu3, _("View") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_pListModel = new wxDataViewListStore();
	m_pListViewCtrl= new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	
	m_pListViewCtrl->AssociateModel(m_pListModel.get());
	bSizer5->Add( m_pListViewCtrl, 1, wxEXPAND, 5 );
	
	m_panelEstado = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pLabelEnvio = new wxStaticText( m_panelEstado, wxID_ANY, _("Send task status:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabelEnvio->Wrap( -1 );
	bSizer3->Add( m_pLabelEnvio, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pEstado = new wxStaticText( m_panelEstado, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pEstado->Wrap( -1 );
	m_pEstado->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_pEstado->SetForegroundColour( wxColour( 0, 174, 0 ) );
	
	bSizer3->Add( m_pEstado, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pBArrancarParar = new wxButton( m_panelEstado, wxID_ANY, _("Start"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_pBArrancarParar->SetDefault(); 
	bSizer3->Add( m_pBArrancarParar, 0, wxALL, 5 );
	
	
	m_panelEstado->SetSizer( bSizer3 );
	m_panelEstado->Layout();
	bSizer3->Fit( m_panelEstado );
	bSizer5->Add( m_panelEstado, 0, wxEXPAND | wxALL, 0 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ) );
	m_menubar1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
	m_panelEstado->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeydown ), NULL, this );
	m_pLabelEnvio->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
	m_pEstado->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
	m_pBArrancarParar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VentanaControlHL7Base::OnArrancarPararClick ), NULL, this );
	m_pBArrancarParar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
}

VentanaControlHL7Base::~VentanaControlHL7Base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ) );
	m_menubar1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
	m_panelEstado->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeydown ), NULL, this );
	m_pLabelEnvio->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
	m_pEstado->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
	m_pBArrancarParar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VentanaControlHL7Base::OnArrancarPararClick ), NULL, this );
	m_pBArrancarParar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( VentanaControlHL7Base::OnKeyDown ), NULL, this );
	
}

DialogoMostrarMensajeBase::DialogoMostrarMensajeBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,291 ), wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Message") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Date:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pFecha = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_pFecha->SetMaxLength( 10000 ); 
	fgSizer1->Add( m_pFecha, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, _("Destination:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_pDestino = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_pDestino->SetMaxLength( 0 ); 
	fgSizer1->Add( m_pDestino, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("Status:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pEstado = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_pEstado->SetMaxLength( 0 ); 
	bSizer6->Add( m_pEstado, 1, wxALL|wxEXPAND, 5 );
	
	wxString m_pChoiceEstadoChoices[] = { _("Sent"), _("Pending"), _("Error"), _("Warning") };
	int m_pChoiceEstadoNChoices = sizeof( m_pChoiceEstadoChoices ) / sizeof( wxString );
	m_pChoiceEstado = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_pChoiceEstadoNChoices, m_pChoiceEstadoChoices, 0 );
	m_pChoiceEstado->SetSelection( 0 );
	m_pChoiceEstado->Hide();
	
	bSizer6->Add( m_pChoiceEstado, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer1->Add( bSizer6, 1, wxEXPAND, 5 );
	
	
	sbSizer1->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Message content:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	sbSizer1->Add( m_staticText7, 0, wxALL, 5 );
	
	m_pContenido = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY );
	m_pContenido->SetMaxLength( 0 ); 
	sbSizer1->Add( m_pContenido, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer5->Add( sbSizer1, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( bSizer5, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button1->SetDefault(); 
	bSizer3->Add( m_button1, 0, wxALL, 5 );
	
	m_CancelButton = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_CancelButton, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer3, 0, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

DialogoMostrarMensajeBase::~DialogoMostrarMensajeBase()
{
}
