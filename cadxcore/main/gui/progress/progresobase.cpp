///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "progresobase.h"

///////////////////////////////////////////////////////////////////////////

PanelTareasBase::PanelTareasBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* m_pSizer1;
	m_pSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_pTasks = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_pTasks->SetScrollRate( 5, 5 );
	m_pSizerTareas = new wxBoxSizer( wxVERTICAL );
	
	m_pSinTareas = new wxStaticText( m_pTasks, wxID_ANY, _("There is no running tasks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSinTareas->Wrap( -1 );
	m_pSizerTareas->Add( m_pSinTareas, 0, wxALL, 5 );
	
	m_pTasks->SetSizer( m_pSizerTareas );
	m_pTasks->Layout();
	m_pSizerTareas->Fit( m_pTasks );
	m_pSizer1->Add( m_pTasks, 1, wxEXPAND, 5 );
	
	this->SetSizer( m_pSizer1 );
	this->Layout();
	m_pSizer1->Fit( this );
}

PanelTareasBase::~PanelTareasBase()
{
}

PanelTareaBase::PanelTareaBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_pProgress = new GinkgoGauge( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer5->Add( m_pProgress, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );
}

PanelTareaBase::~PanelTareaBase()
{
}
