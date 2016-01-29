///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxpropiedadesbase.h"

///////////////////////////////////////////////////////////////////////////
using namespace GNC::GUI;

wxPropiedadesBase::wxPropiedadesBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 385,441 ), wxDefaultSize );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	m_pNotebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pGeneral = new wxScrolledWindow( m_pNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_pGeneral->SetScrollRate( 5, 5 );
	m_pSizerGeneral = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pIcono = new wxStaticBitmap( m_pGeneral, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
	bSizer7->Add( m_pIcono, 0, wxALL, 5 );
	
	m_pNombreWidget = new wxStaticText( m_pGeneral, wxID_ANY, _("Widget's Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pNombreWidget->Wrap( -1 );
	bSizer7->Add( m_pNombreWidget, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_pSizerGeneral->Add( bSizer7, 0, wxEXPAND, 5 );
	
	m_pGeneral->SetSizer( m_pSizerGeneral );
	m_pGeneral->Layout();
	m_pSizerGeneral->Fit( m_pGeneral );
	m_pNotebook->AddPage( m_pGeneral, _("General"), false );
	
	bSizer28->Add( m_pNotebook, 1, wxEXPAND | wxALL, 5 );
	
	m_pPanelBotones = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_pArchSizerPanelPropiedades;
	m_pArchSizerPanelPropiedades = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_pArchSizerPanelPropiedades->Add( 0, 0, 1, wxEXPAND, 2 );
	
	
	m_pArchSizerPanelPropiedades->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_pBCerrar = new wxButton( m_pPanelBotones, wxID_CANCEL, _("&Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBCerrar->SetDefault(); 
	m_pArchSizerPanelPropiedades->Add( m_pBCerrar, 0, wxALL, 5 );
	
	m_pPanelBotones->SetSizer( m_pArchSizerPanelPropiedades );
	m_pPanelBotones->Layout();
	m_pArchSizerPanelPropiedades->Fit( m_pPanelBotones );
	bSizer28->Add( m_pPanelBotones, 0, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer28 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxPropiedadesBase::OnClose ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( wxPropiedadesBase::OnSize ) );
}

wxPropiedadesBase::~wxPropiedadesBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxPropiedadesBase::OnClose ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( wxPropiedadesBase::OnSize ) );
	
}
