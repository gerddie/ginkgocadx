/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ginkgostyle.h"
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include <wx/window.h>
#include <wx/wxginkgotoolbar.h>
#include <resources/ginkgoresourcesmanager.h>
#include <algorithm>

/////////////////////////////////////////////VerticalHeader/////////////////////////////////////
VerticalHeader::VerticalHeader(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& titulo )
	:wxPanel(parent,winid,pos,size,style,titulo)
{
	wxPanel::SetBackgroundColour(wxBG_STYLE_CUSTOM);
	m_colorBorde = wxColour(115, 115, 115);
	m_colorSombraBorde = wxColour(230, 230, 230);
	m_colorTitulo = wxColour(32,32,32);
	m_colorTituloInactivo = wxColour(200,200,200);
	m_sizeBorder = 1;
	m_fuente = wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );
	m_fondoTituloTopColor = wxColour(230, 230, 230);
	m_fondoTituloBottomColor = wxColour(180, 180, 180);
	m_AlturaCabecera = 50;

	m_titulo = wxGetTranslation(titulo);

	//tamanio minimo

	int x = -1,y = -1;
	if (m_titulo != wxEmptyString) {
		wxClientDC dc(this);;
		dc.SetFont(m_fuente);
		dc.GetTextExtent(m_titulo, &x, &y, NULL, NULL, NULL);
	}
	wxSizer* pNewSizer = new wxBoxSizer(wxVERTICAL);
	pNewSizer->Insert(0, m_AlturaCabecera, x, 0, 0, 0 );
	wxPanel::SetSizer(pNewSizer);
	Layout();
	pNewSizer->Fit(this);

	this->Connect( wxEVT_SIZE, wxSizeEventHandler( VerticalHeader::OnSize ), NULL, this );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( VerticalHeader::OnPaint ), NULL, this );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( VerticalHeader::OnEraseBackground ), NULL, this);
}

VerticalHeader::~VerticalHeader()
{
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( VerticalHeader::OnSize ), NULL, this );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( VerticalHeader::OnPaint ), NULL, this );
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( VerticalHeader::OnEraseBackground ), NULL, this);
}


void VerticalHeader::SetToolTip(const wxString& tip)
{
	SetTitle(tip);
}

void VerticalHeader::SetTitle(const wxString& titulo)
{
	m_titulo = titulo;
	wxClientDC dc(this);
	dc.SetFont(m_fuente);
	int x,y;
	dc.GetTextExtent(titulo, &x, &y, NULL, NULL, NULL);

	//tamanio minimo
	GetSizer()->Remove(0);
	GetSizer()->Insert(0, m_AlturaCabecera, x + 20, 0, 0, 0 );
	Layout();

	Refresh();
}

void VerticalHeader::OnEraseBackground( wxEraseEvent& /*event*/ )
{
}

void VerticalHeader::OnPaint(wxPaintEvent &/*event*/)
{
	wxPaintDC dc(this);
	//no muestra la rallita de abajo
	//wxBufferedPaintDC dc(this);

	const int width = dc.GetSize().x;
	const int height = dc.GetSize().y;

	dc.SetFont( m_fuente );

	//se calcula los m�rgenes para centrar el texto

	wxCoord textW, textH;
	dc.GetTextExtent(m_titulo, &textW, &textH);

/*
	wxRect rTitle(0, 0, cli_size.x, cli_size.y);
	dc.GradientFillLinear(rTitle, wxColour(180, 180, 180), wxColour(230, 230, 230), wxWEST);

	dc.SetFont( wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	wxCoord x = 0;
	wxCoord y = 0;
	//dc.SetTextForeground( wxColour(245,245,245) );

	dc.GetTextExtent(str, &x, &y, NULL, NULL, NULL);
	dc.DrawRotatedText(str, (cli_size.x - y) / 2.0, (cli_size.y + x) / 2.0, 90.0);
	*/
	// Title
	wxRect rTitle(0, 0, width, height);

	// Title background
	dc.GradientFillLinear(rTitle, m_fondoTituloBottomColor, m_fondoTituloTopColor, wxWEST);

	// Title border
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(wxPen(m_colorBorde, m_sizeBorder, wxSOLID));
	dc.DrawRectangle(rTitle);

	dc.SetTextForeground(m_colorTitulo);

	dc.SetFont(m_fuente);
	wxCoord x = 0;
	wxCoord y = 0;
	dc.GetTextExtent(m_titulo, &x, &y, NULL, NULL, NULL);
	dc.DrawRotatedText(m_titulo, (width - y) / 2.0, (height + x) / 2.0, 90.0);
}

void VerticalHeader::OnSize(wxSizeEvent & event)
{
	Layout();
	this->Refresh(false);
	event.Skip(true);
}


/////////////////////////////////////////////HeaderPanelWithButton/////////////////////////////////////
#define ID_TITULO 0

class TitleBar : public wxGinkgoToolBar {
public:
	TitleBar(wxWindow* pParent, const wxColour& color, const wxFont& fuente, const wxString& titulo = wxEmptyString) : wxGinkgoToolBar(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
	{
		SetToolBitmapSize(wxSize(16,16));

		wxGinkgoToolBarArt* pToolArt= new wxGinkgoToolBarArt(color);
		pToolArt->SetFont(fuente);

		SetArtProvider(pToolArt);

		if (titulo != wxEmptyString) {
			wxClientDC dc(this);
			int width, height;
			#if !defined(__WXOSX__)
			dc.SetFont(fuente);
			#else
			dc.SetFont(GetFont());
			#endif
			dc.GetTextExtent(titulo, &width, &height);
			AddLabel(ID_TITULO,titulo, width);
		}

		Realize();
	}

	~TitleBar()
	{
	}
};


HeaderPanelWithButton::HeaderPanelWithButton(wxWindow *parent, wxWindowID winid,const wxPoint& pos, const wxSize& size,long style, const wxString& titulo )
	:wxPanel(parent,winid,pos,size,style,titulo)
{
	m_barColor = wxColour(200,200,200);
	m_fontBar = wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );

	m_pTittleBar = m_pButtonBar = NULL;

	wxSizer* pNewSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel::SetSizer(pNewSizer);
	Layout();
	pNewSizer->Fit(this);

	if (titulo != wxEmptyString) {
		SetTitle(titulo);
	}

	m_pButtonBar = new TitleBar(this, m_barColor, m_fontBar);
	GetSizer()->Add(m_pButtonBar,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);
}

HeaderPanelWithButton::~HeaderPanelWithButton()
{
}

void HeaderPanelWithButton::SetToolTip(const wxString& tip)
{
	SetTitle(tip);
}

void HeaderPanelWithButton::SetTitle(const wxString& titulo)
{
	if (m_pTittleBar != NULL) {
		GetSizer()->Detach(m_pTittleBar);
		m_pTittleBar->Destroy();
	}
	m_pTittleBar = new TitleBar(this, m_barColor, m_fontBar, titulo);
	GetSizer()->Insert(0,m_pTittleBar,1, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);
	Layout();
}

void HeaderPanelWithButton::Realize()
{
	m_pButtonBar->Realize();
	m_pTittleBar->Realize();
	Layout();
}

wxGinkgoToolBar* HeaderPanelWithButton::GetButtonBar()
{
	return m_pButtonBar;
}

/////////////////////////////////////////////SubHeaderPanelWithButton/////////////////////////////////////
SubHeaderPanelWithButton::SubHeaderPanelWithButton(wxWindow *parent, wxWindowID winid,const wxPoint& pos, const wxSize& size,long style, const wxString& titulo )
	:HeaderPanelWithButton(parent,winid,pos,size,style,titulo)
{
	m_barColor = wxColour(200,200,200);
	m_fontBar = wxFont( 8, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );

	m_pTittleBar = m_pButtonBar = NULL;

	wxSizer* pNewSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel::SetSizer(pNewSizer);
	Layout();
	pNewSizer->Fit(this);

	if (titulo != wxEmptyString) {
		SetTitle(titulo);
	}

	m_pButtonBar = new TitleBar(this, m_barColor, m_fontBar);
	GetSizer()->Add(m_pButtonBar,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);
}

SubHeaderPanelWithButton::~SubHeaderPanelWithButton()
{
}


/////////////////////////////////////////////HeaderPanel/////////////////////////////////////

HeaderPanel::HeaderPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& titulo )
	:wxPanel(parent,winid,pos,size,style,titulo)
{
	wxPanel::SetBackgroundColour(wxBG_STYLE_CUSTOM);
	m_colorBorde = wxColour(115, 115, 115);
	m_colorSombraBorde = wxColour(230, 230, 230);
	m_colorTitulo = wxColour(245,245,245);
	m_colorTituloInactivo = wxColour(200,200,200);
	m_sizeBorder = 1;
	m_fuente = wxFont( 14, 70, 90, wxFONTWEIGHT_NORMAL, false, wxEmptyString );
	m_fuenteSubTitle = wxFont( 10, 70, 90, wxFONTWEIGHT_NORMAL, false, wxEmptyString );
	m_fondoTituloTopColor = wxColour(0, 0, 0);
	m_fondoTituloBottomColor = wxColour(0, 0, 0);
	m_AlturaCabecera = 40;

	m_titulo = wxGetTranslation(titulo);

	m_bitmap = GinkgoResourcesManager::Wizards::GetIcoBarraSuperior();

	//tamanio minimo
	wxSizer* pNewSizer = new wxBoxSizer(wxVERTICAL);
	pNewSizer->Insert(0, 0, m_AlturaCabecera, 0, 0, 0 );
	wxPanel::SetSizer(pNewSizer);
	Layout();
	pNewSizer->Fit(this);

	this->Connect( wxEVT_SIZE, wxSizeEventHandler( HeaderPanel::OnSize ), NULL, this );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( HeaderPanel::OnPaint ), NULL, this );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( HeaderPanel::OnEraseBackground ), NULL, this);
}

HeaderPanel::~HeaderPanel()
{
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( HeaderPanel::OnSize ), NULL, this );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( HeaderPanel::OnPaint ), NULL, this );
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( HeaderPanel::OnEraseBackground ), NULL, this);
}

void HeaderPanel::SetSubtitle(const wxString& subtitle)
{
	m_subtitle = subtitle;
	m_AlturaCabecera = 70;

	wxSizer* pNewSizer = new wxBoxSizer(wxVERTICAL);
	pNewSizer->Insert(0, 0, m_AlturaCabecera, 0, 0, 0 );
	wxPanel::SetSizer(pNewSizer);
	Layout();
	pNewSizer->Fit(this);

	Refresh(true);
}


void HeaderPanel::SetToolTip(const wxString& tip)
{
	SetTitle(tip);
}

void HeaderPanel::SetTitle(const wxString& titulo)
{
	m_titulo = titulo;
	Refresh();
}

bool HeaderPanel::Enable(bool enabled)
{
	return wxPanel::Enable(enabled);
}

void HeaderPanel::OnEraseBackground( wxEraseEvent& /*event*/ )
{
}

void HeaderPanel::OnPaint(wxPaintEvent &/*event*/)
{
	wxPaintDC dc(this);
	//no muestra la rallita de abajo
	//wxBufferedPaintDC dc(this);

	const int width = dc.GetSize().x;
	const int height = dc.GetSize().y;

	dc.SetFont( m_fuente );

	//se calcula los m�rgenes para centrar el texto
	int margenTitulo = 0, margenSubtitulo = 0;

	wxCoord textW, textH;
	dc.GetTextExtent(m_titulo, &textW, &textH);

	wxCoord textWSub = 0, textHSub = 0;

	if (m_subtitle != wxEmptyString) {
		dc.SetFont( m_fuenteSubTitle );
		dc.GetTextExtent(m_subtitle, & textWSub, & textHSub);
		dc.SetFont( m_fuente );
		margenTitulo = 8 * ((height - textH - textHSub) / 12);
		margenSubtitulo = 1 * ((height - textH - textHSub) / 12);
	} else {
		margenTitulo = (height - textH) / 2;
	}

	// Title
	wxRect rTitle(0, 0, width, height);

	// Title background
	dc.GradientFillLinear(rTitle, m_fondoTituloBottomColor, m_fondoTituloTopColor, wxNORTH);

	dc.DrawBitmap(m_bitmap,width - m_bitmap.GetWidth(),0);

	// Title border
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(wxPen(m_colorBorde, m_sizeBorder, wxSOLID));
	dc.DrawRectangle(rTitle);

	// Title text
	if (!IsEnabled()) {
		dc.SetTextForeground(wxColour(172,168,153));
	} else {
		dc.SetTextForeground(m_colorTitulo);
	}

	dc.DrawText(m_titulo, 15, margenTitulo);

	if (m_subtitle != wxEmptyString)
	{
		dc.SetFont( m_fuenteSubTitle );
		dc.DrawText(m_subtitle, 15, margenTitulo + margenSubtitulo + textH);
	}
}

void HeaderPanel::OnSize(wxSizeEvent & event)
{
	Layout();
	this->Refresh(false);
	event.Skip(true);
}


/////////////////////////////////////////////PANEL SCROLLABLE/////////////////////////////////////

PanelScrollable::PanelScrollable(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style)
	:wxScrolledWindow(parent,winid,pos,size,style)
{
	SetScrollRate( 5, 5 );
}
PanelScrollable::~PanelScrollable() {}

/////////////////////////////////////////////BodyPanel/////////////////////////////////////
BodyPanel::BodyPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style)
	:PanelScrollable(parent,winid,pos,size,style)
{
	m_colorFondo = wxColour(211,211,211);
	wxPanel::SetBackgroundColour(m_colorFondo);
	m_sizeBorderInterior = 10;

}
BodyPanel::~BodyPanel() {}

void BodyPanel::SetSizer(wxSizer* sizer, bool deleteOld)
{
	wxSizer* pNewSizer = new wxBoxSizer(wxVERTICAL);
	pNewSizer->Add(sizer,1,wxEXPAND|wxALL, m_sizeBorderInterior);
	PanelScrollable::SetSizer(pNewSizer,deleteOld);
	Layout();
	pNewSizer->Fit(this);
}
/////////////////////////////////////////////BodyPanel/////////////////////////////////////
BodyPanelSinScroll::BodyPanelSinScroll(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style)
	:wxPanel(parent,winid,pos,size,style)
{
	m_colorFondo = wxColour(211,211,211);
	wxPanel::SetBackgroundColour(m_colorFondo);
	m_sizeBorderInterior = 5;

}
BodyPanelSinScroll::~BodyPanelSinScroll() {}

void BodyPanelSinScroll::SetSizer(wxSizer* sizer, bool deleteOld)
{
	wxSizer* pNewSizer = new wxBoxSizer(wxVERTICAL);
	pNewSizer->Add(sizer,1,wxEXPAND|wxALL, m_sizeBorderInterior);
	wxPanel::SetSizer(pNewSizer,deleteOld);
	Layout();
	pNewSizer->Fit(this);
}
/////////////////////////////////////////////Footer/////////////////////////////////////
FooterPanel::FooterPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style)
:wxPanel(parent,winid,pos,size,style)
{
	m_sizeBorderInterior = 8;
	m_sizeBorder = 1;
	m_colorBorde = wxColour(115, 115, 115);

	this->Connect( wxEVT_PAINT, wxPaintEventHandler( FooterPanel::OnPaint ), NULL, this );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( FooterPanel::OnEraseBackground ), NULL, this);
}
FooterPanel::~FooterPanel() {}


void FooterPanel::OnEraseBackground( wxEraseEvent& /*event*/ )
{
}

void FooterPanel::OnPaint(wxPaintEvent &/*event*/)
{
	wxPaintDC dc(this);
	//no muestra la rallita de abajo
	//wxBufferedPaintDC dc(this);

	const int width = dc.GetSize().x;
	const int height = dc.GetSize().y;

	//panelbackground
	wxRect rPanelBackground(0, 0, width, height);
	dc.SetBrush(wxBrush(GetBackgroundColour()));
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(rPanelBackground);

	// Border superior
	wxRect rPanel(0, 0, width, height);
	dc.SetPen(wxPen(m_colorBorde, m_sizeBorder, wxSOLID));
	dc.DrawLine(0,0,width,0);
}

void FooterPanel::SetSizer(wxSizer* sizer, bool deleteOld)
{
	wxSizer* pNewSizer = new wxBoxSizer(wxVERTICAL);
	pNewSizer->Add(sizer,1,wxEXPAND|wxALL, m_sizeBorderInterior);
	wxPanel::SetSizer(pNewSizer,deleteOld);
	Layout();
	pNewSizer->Fit(this);
}

/////////////////////////////////////////////TitledPanel/////////////////////////////////////
UnTitledPanel::UnTitledPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style):wxPanel(parent,winid,pos,size,style)
{
	m_colorFondo = wxColour(242,242,242);
	wxPanel::SetBackgroundColour(m_colorFondo);
	m_colorBorde = wxColour(115, 115, 115);
	m_colorSombraBorde = wxColour(230, 230, 230);
	
	m_sizeBorder = 1;
	
#if defined(__WXGTK__)
	m_borderInterior = 4;
#else
	m_borderInterior = 10;
#endif

	this->Connect( wxEVT_SIZE, wxSizeEventHandler( UnTitledPanel::OnSize ), NULL, this );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( UnTitledPanel::OnPaint ), NULL, this );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( UnTitledPanel::OnEraseBackground ), NULL, this);

}
UnTitledPanel::~UnTitledPanel()
{
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( UnTitledPanel::OnSize ), NULL, this );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( UnTitledPanel::OnPaint ), NULL, this );
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( UnTitledPanel::OnEraseBackground ), NULL, this);
}

void UnTitledPanel::OnEraseBackground( wxEraseEvent& /*event*/ )
{
}

void UnTitledPanel::OnPaint(wxPaintEvent &/*event*/)
{
	wxPaintDC dc(this);
	//no muestra la rallita de abajo
	//wxBufferedPaintDC dc(this);

	const int width = dc.GetSize().x;
	const int height = dc.GetSize().y;

	//panelbackground
	wxRect rPanelBackground(0, 0, width, height);
	dc.SetBrush(wxBrush(m_colorFondo));
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(rPanelBackground);

	// Panel Border
	wxRect rPanel(0, 0, width, height);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(wxPen(m_colorBorde, m_sizeBorder, wxSOLID));
	dc.DrawRectangle(rPanel);

}

void UnTitledPanel::OnSize(wxSizeEvent & event)
{
	Layout();
	this->Refresh(false);
	event.Skip(true);
}

void UnTitledPanel::SetColorBorde(const wxColour& colorBorde)
{
	m_colorBorde = colorBorde;
	Refresh();
}

wxColour UnTitledPanel::GetColorBorde()
{
	return m_colorBorde;
}

int UnTitledPanel::GetSizeBorde()
{
	return m_sizeBorder;
}

void UnTitledPanel::SetSizeBorde(int size)
{
	m_sizeBorder = size;
	Refresh();
}

int UnTitledPanel::GetBorderInterior()
{
	return m_borderInterior;
}

void UnTitledPanel::SetBorderInterior(int size)
{
	m_borderInterior = size;
	if ( GetSizer() != NULL )
	{
		GetSizer()->GetItem((size_t)1)->SetBorder(m_borderInterior);
	}
	Refresh();
}


/////////////////////////////////////////////TitledPanel/////////////////////////////////////
TitledPanel::TitledPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
				const wxString& titulo  ):wxPanel(parent,winid,pos,size,style)
{
	m_recogido = false;
	m_colorFondo = wxColour(242,242,242);
	wxPanel::SetBackgroundColour(m_colorFondo);
	m_colorBorde = wxColour(115, 115, 115);
	m_colorSombraBorde = wxColour(230, 230, 230);
	m_colorTitulo = wxColour(245,245,245);
	m_colorTituloInactivo = wxColour(200,200,200);
	m_sizeBorder = 1;
	m_fuente = wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );
	m_fondoTituloTopColor = wxColour(90, 90, 90);
	m_fondoTituloBottomColor = wxColour(54, 54, 54);
	m_margenTitulo = 5;
#if defined(__WXGTK__)
	m_borderInterior = 4;
#else
	m_borderInterior = 10;
#endif

	m_titulo = wxGetTranslation(titulo);

	this->Connect( wxEVT_SIZE, wxSizeEventHandler( TitledPanel::OnSize ), NULL, this );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( TitledPanel::OnPaint ), NULL, this );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( TitledPanel::OnEraseBackground ), NULL, this);

}
TitledPanel::~TitledPanel()
{
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( TitledPanel::OnSize ), NULL, this );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( TitledPanel::OnPaint ), NULL, this );
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( TitledPanel::OnEraseBackground ), NULL, this);
}

void TitledPanel::SetToolTip(const wxString& tip)
{
	SetTitle(tip);
}

void TitledPanel::SetTitle(const wxString& titulo)
{
	m_titulo = titulo;
	Refresh();
}

bool TitledPanel::Enable(bool enabled)
{
	return wxPanel::Enable(enabled);
}

void TitledPanel::OnEraseBackground( wxEraseEvent& /*event*/ )
{
}

void TitledPanel::OnPaint(wxPaintEvent &/*event*/)
{
	wxPaintDC dc(this);
	//no muestra la rallita de abajo
	//wxBufferedPaintDC dc(this);

	const int width = dc.GetSize().x;
	const int height = dc.GetSize().y;

	dc.SetFont( m_fuente );

	wxCoord textW, textH;
	dc.GetTextExtent(m_titulo, & textW, & textH);

	// Title
	int dobleMargen = m_margenTitulo*2;
	wxRect rTitle(0, 0, width, textH + dobleMargen);

	// Title background

	dc.GradientFillLinear(rTitle, m_fondoTituloBottomColor, m_fondoTituloTopColor, wxNORTH);

	// Title border
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(wxPen(m_colorBorde, m_sizeBorder, wxSOLID));
	dc.DrawRectangle(rTitle);

	// Title text
	if (!IsEnabled()) {
		dc.SetTextForeground(wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ));
	} else {
		dc.SetTextForeground(m_colorTitulo);
	}

	dc.DrawText(m_titulo,m_margenTitulo+3,m_margenTitulo);

	// inner shadow container
	wxRect rTitleShadow(1, textH + dobleMargen, width - 2, height - textH - dobleMargen - 1);
	dc.SetPen(wxPen(m_colorSombraBorde, m_sizeBorder, wxSOLID));
	dc.DrawRectangle(rTitleShadow);

	//panelbackground
	wxRect rPanelBackground(0, textH + dobleMargen, width, height);
	dc.SetBrush(wxBrush(m_colorFondo));
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(rPanelBackground);

	// Panel Border
	wxRect rPanel(0, 0, width, height);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(wxPen(m_colorBorde, m_sizeBorder, wxSOLID));
	dc.DrawRectangle(rPanel);

}

void TitledPanel::OnSize(wxSizeEvent & event)
{
	Layout();
	this->Refresh(false);
	event.Skip(true);
}

void TitledPanel::SetColorBorde(const wxColour& colorBorde)
{
	m_colorBorde = colorBorde;
	Refresh();
}

wxColour TitledPanel::GetColorBorde()
{
	return m_colorBorde;
}

void TitledPanel::SetColorTitulo(const wxColour& colorTitulo)
{
	m_colorTitulo = colorTitulo;
	Refresh();
}

wxColour TitledPanel::GetColorTitulo()
{
	return m_colorTitulo;
}

int TitledPanel::GetSizeBorde()
{
	return m_sizeBorder;
}

void TitledPanel::SetSizeBorde(int size)
{
	m_sizeBorder = size;
	Refresh();
}

int TitledPanel::GetBorderInterior()
{
	return m_borderInterior;
}

void TitledPanel::SetBorderInterior(int size)
{
	m_borderInterior = size;
	if ( GetSizer() != NULL )
	{
		GetSizer()->GetItem((size_t)1)->SetBorder(m_borderInterior);
	}
	Refresh();
}

//TODO: esta mu deprecated, pero tiene posibilidades de seguir en un futuro
bool TitledPanel::SetBackgroundColour(const wxColour &colour)
{
	//TODO HAY QUE MEJORAR ESTE METODO PARA PONERLO TODO EN FUNCION DEL RESTO
	bool correcto = wxPanel::SetBackgroundColour(colour);

	// el color del titulo sera mas oscuro y el del borde mas todavia
	wxImage::HSVValue valorHSV = wxImage::RGBtoHSV(wxImage::RGBValue(colour.Red(), colour.Green(), colour.Blue()));

	valorHSV.saturation +=  (float)30 * 0.0125f;
	valorHSV.saturation = std::min<double>(1.0f,valorHSV.saturation);
	valorHSV.value -= (float)30 * 0.00625f;
	valorHSV.value = std::max<double>(0.0f,valorHSV.value);

	wxImage::RGBValue rgbAux = wxImage::HSVtoRGB(valorHSV);
	/*m_colorFondoTitulo = wxColour(rgbAux.red,rgbAux.green,rgbAux.blue);*/


	valorHSV.saturation +=  (float)50 * 0.0125f;
	valorHSV.saturation = std::min<double>(1.0f,valorHSV.saturation);
	valorHSV.value -= (float)50 * 0.00625f;
	valorHSV.value = std::max<double>(0.0f,valorHSV.value);

	rgbAux = wxImage::HSVtoRGB(valorHSV);
	m_colorBorde = wxColour(rgbAux.red,rgbAux.green,rgbAux.blue);

	//????como calcular la letra
	Refresh();

	return correcto;
}

void TitledPanel::Recoger(bool recoger)
{
	m_recogido = recoger;

	wxWindowList listaVentanas = GetChildren();
	for (wxWindowList::iterator it = listaVentanas.begin(); it != listaVentanas.end(); ++it) {
		(*it)->Show(!recoger);
	}

	Layout();
	GetParent()->Layout();
	GetParent()->Refresh(true);
}

bool TitledPanel::EstaRecogido()
{
	return m_recogido;
}

int TitledPanel::GetGap()
{
	int gap = m_margenTitulo*2 + m_fuente.GetPointSize() + m_fuente.GetPointSize()*0.5f; //margenes + fuente + margen porque si pones "pl" y mide mas d 10 porque la p mide 10 pero esta mas abajo y la ele lo mismo pero para arriba
	return gap;
}

void TitledPanel::SetSizer(wxSizer* sizer, bool deleteOld)
{
	int gap = m_margenTitulo*2 + m_fuente.GetPointSize() + m_fuente.GetPointSize()*0.5f; //margenes + fuente + margen porque si pones "pl" y mide mas d 10 porque la p mide 10 pero esta mas abajo y la ele lo mismo pero para arriba
	wxSizer* pNewSizer = new wxBoxSizer(wxVERTICAL);

	pNewSizer->Insert(0, 0, gap, 0, 0, 0, 0 );
	pNewSizer->Add(sizer,1,wxEXPAND|wxALL, m_borderInterior);
	wxPanel::SetSizer(pNewSizer,deleteOld);
	Layout();
	pNewSizer->Fit(this);
}

/////////////////////////////////////////////TitledPanelSinBorder/////////////////////////////////////

TitledPanelSinBorder::TitledPanelSinBorder(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
				const wxString& titulo  ):TitledPanel(parent,winid,pos,size,style,titulo)
{
	m_borderInterior = 1;
}
TitledPanelSinBorder::~TitledPanelSinBorder()
{
}

/////////////////////////////////////////////TitledPanelSeparatorV/////////////////////////////////////
TitledPanelSeparatorV::TitledPanelSeparatorV(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long  )
	:wxPanel(parent,winid,pos,size,wxNO_BORDER)
{
	m_separacion = 15;
	SetMinSize( wxSize(0,m_separacion) );
	SetMaxSize( wxSize(0,m_separacion) );
	SetSize( wxSize(0,m_separacion) );
}

TitledPanelSeparatorV::~TitledPanelSeparatorV()
{
}
/////////////////////////////////////////////TitledPanelSeparatorH/////////////////////////////////////
TitledPanelSeparatorH::TitledPanelSeparatorH(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long  )
	:wxPanel(parent,winid,pos,size,wxNO_BORDER)
{
	m_separacion = 15;
	SetMinSize( wxSize(m_separacion,0) );
	SetMaxSize( wxSize(m_separacion,0) );
	SetSize( wxSize(m_separacion,0) );
}

TitledPanelSeparatorH::~TitledPanelSeparatorH()
{
}

/////////////////////////////////////////////SUBTITLED/////////////////////////////////////

SubTitledPanel::SubTitledPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& titulo )
			:TitledPanel(parent,winid,pos,size,style,titulo)
{
	m_recogido = false;
	m_colorFondo = wxColour(255,255,255);
	wxPanel::SetBackgroundColour(m_colorFondo);

	m_colorBorde = wxColour(175, 175, 175);
	m_colorSombraBorde = wxColour(230, 230, 230);
	m_colorTitulo = wxColour(245,245,245);
	m_sizeBorder = 1;
	m_fuente = wxFont( 8, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );

	m_fondoTituloTopColor = wxColour(130, 130, 130);
	m_fondoTituloBottomColor = wxColour(110, 110, 110);

	m_margenTitulo = 4;
#if defined(__WXGTK__)
	m_borderInterior = 4;
#else
	m_borderInterior = 10;
#endif
}

SubTitledPanel::~SubTitledPanel()
{
}

/////////////////////////////////////////////SubTitledPanelSinBorder/////////////////////////////////////

SubTitledPanelSinBorder::SubTitledPanelSinBorder(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
				const wxString& titulo  ):SubTitledPanel(parent,winid,pos,size,style,titulo)
{
	m_borderInterior = 1;
}
SubTitledPanelSinBorder::~SubTitledPanelSinBorder()
{
}

/////////////////////////////////////////////SubTitledPanelSeparatorV/////////////////////////////////////
SubTitledPanelSeparatorV::SubTitledPanelSeparatorV(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long  )
	:wxPanel(parent,winid,pos,size,wxNO_BORDER)
{
	m_separacion = 15;
	SetMinSize( wxSize(0,m_separacion) );
	SetMaxSize( wxSize(0,m_separacion) );
	SetSize( wxSize(0,m_separacion) );
	#if defined(__WXGTK__)
	SetBackgroundColour(parent->GetBackgroundColour());
	#endif
}

SubTitledPanelSeparatorV::~SubTitledPanelSeparatorV()
{
}
/////////////////////////////////////////////SubTitledPanelSeparatorH/////////////////////////////////////
SubTitledPanelSeparatorH::SubTitledPanelSeparatorH(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long  )
	:wxPanel(parent,winid,pos,size,wxNO_BORDER)
{
	m_separacion = 15;
	SetMinSize( wxSize(m_separacion,0) );
	SetMaxSize( wxSize(m_separacion,0) );
	SetSize( wxSize(m_separacion,0) );
	#if defined(__WXGTK__)
	SetBackgroundColour(parent->GetBackgroundColour());
	#endif
}

SubTitledPanelSeparatorH::~SubTitledPanelSeparatorH()
{
}

/////////////////////////////////////////////GinkgoBoxPanel/////////////////////////////////////
GinkgoBoxPanel::GinkgoBoxPanel(wxWindow *parent,
         wxWindowID winid ,
         const wxPoint& pos,
         const wxSize& size ,
			long style):wxPanel(parent, winid, pos, size, style)
{
	m_backgroudColor = wxColour(248,247,243);
	m_borderColor = wxColour(216, 215, 209, 255);
	SetBackgroundColour(m_backgroudColor);

	this->Connect( wxEVT_PAINT, wxPaintEventHandler( GinkgoBoxPanel::OnPaint ), NULL, this );
}

GinkgoBoxPanel::~GinkgoBoxPanel()
{
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( GinkgoBoxPanel::OnPaint ), NULL, this );	
}

void GinkgoBoxPanel::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	dc.SetBrush(wxBrush(m_borderColor,wxTRANSPARENT));
	dc.SetPen(wxPen(m_borderColor, 1, wxSOLID));

	dc.DrawRectangle(wxRect(wxPoint(0,0), wxPoint(dc.GetSize().x-1, dc.GetSize().y-1)));
	event.Skip(false);
}

/////////////////////////////////////////////GinkgoGauge/////////////////////////////////////
GinkgoGauge::GinkgoGauge(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
				long style): wxPanel(parent, winid, pos, size, style),
				m_dirty(false)
{
	SetMinSize(wxSize(150, 15));
	m_progress = 0.0f;
	wxFont font(8, GetFont().GetFamily(), GetFont().GetStyle(), GetFont().GetWeight());
	SetFont(font);
	wxWindow::SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	wxClientDC dc(this);
	dc.SetFont(font);
	int height,width;
	dc.GetTextExtent(wxT("000% "), &width, &height, NULL, NULL, NULL);
	m_maxSizeOfText = GetSize().x - (width +15);

	RecreateBuffer(size);

	
	this->Connect( wxEVT_IDLE, wxIdleEventHandler(GinkgoGauge::OnIdle), NULL, this );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( GinkgoGauge::OnPaint ), NULL, this );
	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( GinkgoGauge::OnEraseBackground ), NULL, this);
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( GinkgoGauge::OnSize ), NULL, this );
}

GinkgoGauge::~GinkgoGauge()
{
	this->Disconnect(wxEVT_IDLE, wxIdleEventHandler(GinkgoGauge::OnIdle), NULL, this );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( GinkgoGauge::OnPaint ), NULL, this );
	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( GinkgoGauge::OnEraseBackground ), NULL, this);
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( GinkgoGauge::OnSize ), NULL, this );
}	

void GinkgoGauge::SetStatus(const wxString& status)
{
	if (m_status != status) {
		m_status = status;
		wxClientDC dc(this);
		dc.SetFont(GetFont());
		int width,height;
		dc.GetTextExtent(m_status, &width, &height, NULL, NULL, NULL);
		while (width > m_maxSizeOfText && m_status.size() > 2) {
			m_status = m_status.substr(0, m_status.size() - 2);				
			dc.GetTextExtent(m_status, &width, &height, NULL, NULL, NULL);
		} 
		m_dirty = true;
	}
}

wxString GinkgoGauge::GetStatus()
{
	return m_status;
}

void GinkgoGauge::SetProgress(float progress)
{
	if (progress != m_progress && progress >= 0.0f && progress <= 1.0f) {
		m_progress = progress;
		m_dirty = true;
	}
}

float GinkgoGauge::GetProgress()
{
	return m_progress;
}

void GinkgoGauge::OnIdle(wxIdleEvent &WXUNUSED(event))
{
	if (m_dirty) {
		Refresh();
		m_dirty = false;
	}
}

void GinkgoGauge::OnPaint(wxPaintEvent &event)
{
	wxBufferedPaintDC dc(this, m_bufferBitmap);
	PrepareDC(dc);

	dc.SetFont(GetFont());
	dc.SetPen( wxPen( m_borderColour, 1, wxSOLID ) );
	dc.SetBrush(wxColour(255,255,255));

	int width = dc.GetSize().x;
	int height = dc.GetSize().y;
	wxRect cr(0,0, width, height);

	dc.DrawRectangle(cr);

	cr.width = (int) ( (float)width * m_progress );

	dc.GradientFillLinear(cr, m_startColour, m_endColour, wxEAST);

	wxRasterOperationMode func = dc.GetLogicalFunction();
	dc.SetLogicalFunction(wxXOR);

	wxCoord tx = 0, ty = 0;
	wxString str = wxString::Format(wxT("%.00f%% "), 100.0f * m_progress) + m_status;
	dc.GetTextExtent(str, &tx, &ty, NULL, NULL, NULL);


	dc.DrawText(str, (width >> 1)  - (tx >> 1), (height >> 1) - (ty >> 1) );
	dc.SetLogicalFunction(func);
	event.Skip(false);
}

void GinkgoGauge::OnEraseBackground( wxEraseEvent&  )
{
}


void GinkgoGauge::OnSize(wxSizeEvent& )
{
	RecreateBuffer();
}

void GinkgoGauge::SetMinSize(const wxSize &minSize)
{
	wxPanel::SetMinSize(minSize);
	RecreateBuffer();
}

void GinkgoGauge::SetSize(const wxSize &minSize)
{
	wxPanel::SetSize(minSize);
	RecreateBuffer();
}

void GinkgoGauge::SetColorSchema(TColorSchema schema)
{
	switch (schema) {
		case TCS_BLUE:
			m_fontColour = wxColour(0,0,0);
			m_startColour = wxColour(224, 237, 255);
			m_endColour = wxColour(119, 169, 255);
			m_borderColour = wxColour(58, 143, 255);
			break;
		case TCS_RED:
			m_fontColour = wxColour(0,0,0);
			m_startColour = wxColour(255, 224, 237);
			m_endColour = wxColour(255, 169, 119);
			m_borderColour = wxColour(255, 0, 0);
			break;
		case TCS_GREY:
			m_fontColour = wxColour(0,0,0);
			m_startColour = wxColour(237, 237, 237);
			m_endColour = wxColour(100, 100, 100);
			m_borderColour = wxColour(143, 143, 143);
			break;
		case TCS_GREEN:
			m_fontColour = wxColour(0,0,0);
			m_startColour = wxColour(237, 255, 224);
			m_endColour = wxColour(120, 198, 83);
			m_borderColour = wxColour(143, 255, 58);
			break;
	}
	SetForegroundColour(m_fontColour);
}

bool GinkgoGauge::RecreateBuffer(const wxSize& size)
{
    wxSize sz = size;
    if (sz == wxDefaultSize)
        sz = GetClientSize();

    if (sz.x < 1 || sz.y < 1)
        return false;

	 wxClientDC dc(this);
	 dc.SetFont(GetFont());
	 int height,width;
	 dc.GetTextExtent(wxT("000% "), &width, &height, NULL, NULL, NULL);
	 m_maxSizeOfText = sz.x - (width +15);
	 m_maxSizeOfText = std::max<int>(m_maxSizeOfText, 200);

    if (!m_bufferBitmap.Ok() || m_bufferBitmap.GetWidth() < sz.x || m_bufferBitmap.GetHeight() < sz.y)
        m_bufferBitmap = wxBitmap(sz.x, sz.y);

    return m_bufferBitmap.Ok();
}



