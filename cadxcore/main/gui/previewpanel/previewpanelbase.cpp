///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/ginkgostyle/ginkgostyle.h"

#include "previewpanelbase.h"

///////////////////////////////////////////////////////////////////////////

FirstLevelPanelBase::FirstLevelPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	this->SetBackgroundColour( wxColour( 234, 234, 234 ) );
	this->SetMinSize( wxSize( 100,-1 ) );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer18->SetMinSize( wxSize( -1,20 ) ); 
	m_pLabel = new wxStaticText( this, wxID_ANY, _("javi"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	m_pLabel->SetFont( wxFont( 8, 70, 90, 92, false, wxEmptyString ) );
	m_pLabel->SetForegroundColour( wxColour( 0, 0, 0 ) );
	
	bSizer18->Add( m_pLabel, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizer10->Add( bSizer18, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer10 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( FirstLevelPanelBase::OnChar ) );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( FirstLevelPanelBase::OnEraseBackground ) );
	this->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( FirstLevelPanelBase::OnPanelDClick ) );
	this->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnPanelClick ) );
	this->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( FirstLevelPanelBase::OnMouseWheel ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( FirstLevelPanelBase::OnPaint ) );
	this->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnMenuMouse ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( FirstLevelPanelBase::OnSize ) );
	m_pLabel->Connect( wxEVT_CHAR, wxKeyEventHandler( FirstLevelPanelBase::OnChar ), NULL, this );
	m_pLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( FirstLevelPanelBase::OnEraseBackground ), NULL, this );
	m_pLabel->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnCabeceraClick ), NULL, this );
	m_pLabel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( FirstLevelPanelBase::OnMouseWheel ), NULL, this );
	m_pLabel->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnMenuMouse ), NULL, this );
}

FirstLevelPanelBase::~FirstLevelPanelBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( FirstLevelPanelBase::OnChar ) );
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( FirstLevelPanelBase::OnEraseBackground ) );
	this->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( FirstLevelPanelBase::OnPanelDClick ) );
	this->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnPanelClick ) );
	this->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( FirstLevelPanelBase::OnMouseWheel ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( FirstLevelPanelBase::OnPaint ) );
	this->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnMenuMouse ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( FirstLevelPanelBase::OnSize ) );
	m_pLabel->Disconnect( wxEVT_CHAR, wxKeyEventHandler( FirstLevelPanelBase::OnChar ), NULL, this );
	m_pLabel->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( FirstLevelPanelBase::OnEraseBackground ), NULL, this );
	m_pLabel->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnCabeceraClick ), NULL, this );
	m_pLabel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( FirstLevelPanelBase::OnMouseWheel ), NULL, this );
	m_pLabel->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( FirstLevelPanelBase::OnMenuMouse ), NULL, this );
	
}

LeafPanelBase::LeafPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	this->SetBackgroundColour( wxColour( 204, 204, 204 ) );
	this->SetMinSize( wxSize( 100,120 ) );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_pThumbnail = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_pThumbnail->SetMinSize( wxSize( 76,76 ) );
	m_pThumbnail->SetMaxSize( wxSize( 76,76 ) );
	
	bSizer12->Add( m_pThumbnail, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_pLabel = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel->Wrap( -1 );
	m_pLabel->SetFont( wxFont( 8, 70, 90, 90, false, wxEmptyString ) );
	m_pLabel->SetForegroundColour( wxColour( 32, 32, 32 ) );
	
	bSizer9->Add( m_pLabel, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, 3 );
	
	m_pLabel1 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_pLabel1->Wrap( -1 );
	m_pLabel1->SetFont( wxFont( 8, 70, 90, 90, false, wxEmptyString ) );
	m_pLabel1->SetForegroundColour( wxColour( 32, 32, 32 ) );
	
	bSizer9->Add( m_pLabel1, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	bSizer12->Add( bSizer9, 1, wxEXPAND, 6 );
	
	
	bSizer11->Add( bSizer12, 1, wxEXPAND|wxALL, 1 );
	
	
	this->SetSizer( bSizer11 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ) );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ) );
	this->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ) );
	this->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ) );
	this->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( LeafPanelBase::OnPaint ) );
	this->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ) );
	m_pThumbnail->Connect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ), NULL, this );
	m_pThumbnail->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ), NULL, this );
	m_pThumbnail->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ), NULL, this );
	m_pThumbnail->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ), NULL, this );
	m_pThumbnail->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ), NULL, this );
	m_pThumbnail->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ), NULL, this );
	m_pLabel->Connect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ), NULL, this );
	m_pLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ), NULL, this );
	m_pLabel->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ), NULL, this );
	m_pLabel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ), NULL, this );
	m_pLabel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ), NULL, this );
	m_pLabel->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ), NULL, this );
	m_pLabel1->Connect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ), NULL, this );
	m_pLabel1->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ), NULL, this );
	m_pLabel1->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ), NULL, this );
	m_pLabel1->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ), NULL, this );
	m_pLabel1->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ), NULL, this );
	m_pLabel1->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ), NULL, this );
}

LeafPanelBase::~LeafPanelBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ) );
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ) );
	this->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ) );
	this->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ) );
	this->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( LeafPanelBase::OnPaint ) );
	this->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ) );
	m_pThumbnail->Disconnect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ), NULL, this );
	m_pThumbnail->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ), NULL, this );
	m_pThumbnail->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ), NULL, this );
	m_pThumbnail->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ), NULL, this );
	m_pThumbnail->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ), NULL, this );
	m_pThumbnail->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ), NULL, this );
	m_pLabel->Disconnect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ), NULL, this );
	m_pLabel->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ), NULL, this );
	m_pLabel->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ), NULL, this );
	m_pLabel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ), NULL, this );
	m_pLabel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ), NULL, this );
	m_pLabel->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ), NULL, this );
	m_pLabel1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( LeafPanelBase::OnChar ), NULL, this );
	m_pLabel1->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( LeafPanelBase::OnEraseBackground ), NULL, this );
	m_pLabel1->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( LeafPanelBase::OnLeafDClick ), NULL, this );
	m_pLabel1->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LeafPanelBase::OnLeafClick ), NULL, this );
	m_pLabel1->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( LeafPanelBase::OnMouseWheel ), NULL, this );
	m_pLabel1->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( LeafPanelBase::OnMenuMouse ), NULL, this );
	
}

PreviewPanelBase::PreviewPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	this->SetBackgroundColour( wxColour( 128, 128, 128 ) );
	this->Hide();
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_pPreviewTitle = new HeaderPanelWithButton( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pPreviewTitle->SetToolTip( _("Previews") );
	
	bSizer16->Add( m_pPreviewTitle, 0, wxEXPAND, 5 );
	
	m_pPanelPreviews = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL|wxWANTS_CHARS );
	m_pPanelPreviews->SetScrollRate( 0, 10 );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	
	m_pPanelPreviews->SetSizer( bSizer17 );
	m_pPanelPreviews->Layout();
	bSizer17->Fit( m_pPanelPreviews );
	bSizer16->Add( m_pPanelPreviews, 1, wxEXPAND, 5 );
	
	m_pPanelVertical = new VerticalHeader( this, wxID_ANY, wxDefaultPosition, wxSize( 26,-1 ), wxTAB_TRAVERSAL );
	m_pPanelVertical->SetFont( wxFont( 10, 70, 90, 92, false, wxEmptyString ) );
	m_pPanelVertical->SetForegroundColour( wxColour( 255, 255, 255 ) );
	m_pPanelVertical->SetBackgroundColour( wxColour( 54, 54, 54 ) );
	m_pPanelVertical->Hide();
	m_pPanelVertical->SetToolTip( _("Previews") );
	m_pPanelVertical->SetMinSize( wxSize( 26,-1 ) );
	m_pPanelVertical->SetMaxSize( wxSize( 26,-1 ) );
	
	bSizer16->Add( m_pPanelVertical, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( bSizer16 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ) );
	this->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( PreviewPanelBase::OnMouseWheel ) );
	this->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( PreviewPanelBase::OnMenuMouse ) );
	m_pPreviewTitle->Connect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPreviewTitle->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PreviewPanelBase::OnEraseBackground ), NULL, this );
	m_pPanelPreviews->Connect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelPreviews->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelPreviews->Connect( wxEVT_KEY_UP, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelPreviews->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( PreviewPanelBase::OnMedicalHistoryClick ), NULL, this );
	m_pPanelPreviews->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( PreviewPanelBase::OnMouseWheel ), NULL, this );
	m_pPanelPreviews->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( PreviewPanelBase::OnMenuMouse ), NULL, this );
	m_pPanelVertical->Connect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelVertical->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PreviewPanelBase::OnEraseBackground ), NULL, this );
	m_pPanelVertical->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( PreviewPanelBase::OnShowPreviewsClick ), NULL, this );
}

PreviewPanelBase::~PreviewPanelBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ) );
	this->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( PreviewPanelBase::OnMouseWheel ) );
	this->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( PreviewPanelBase::OnMenuMouse ) );
	m_pPreviewTitle->Disconnect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPreviewTitle->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PreviewPanelBase::OnEraseBackground ), NULL, this );
	m_pPanelPreviews->Disconnect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelPreviews->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelPreviews->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelPreviews->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( PreviewPanelBase::OnMedicalHistoryClick ), NULL, this );
	m_pPanelPreviews->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( PreviewPanelBase::OnMouseWheel ), NULL, this );
	m_pPanelPreviews->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( PreviewPanelBase::OnMenuMouse ), NULL, this );
	m_pPanelVertical->Disconnect( wxEVT_CHAR, wxKeyEventHandler( PreviewPanelBase::OnChar ), NULL, this );
	m_pPanelVertical->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( PreviewPanelBase::OnEraseBackground ), NULL, this );
	m_pPanelVertical->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( PreviewPanelBase::OnShowPreviewsClick ), NULL, this );
	
}
