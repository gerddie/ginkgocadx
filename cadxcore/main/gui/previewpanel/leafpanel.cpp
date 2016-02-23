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

#include <wx/event.h>
#include <api/globals.h>
#include <wx/dcbuffer.h>
#include "leafpanel.h"
#include <api/controllers/ieventscontroller.h>
#include <main/controllers/thumbnailcontroller.h>


#define FONDO_INACTIVO_E wxColour(194,194,194)
#define FONDO_INACTIVO_O wxColour(234,234,234)
#define FONDO_ACTIVO   wxColour(255,225,85)

#define TEXT_LENGTH 80


namespace GNC {
	namespace GUI {

		LeafPanel::LeafPanel(GNC::GCS::IVista* pView, INodoHistorial* pPadre,  GNC::GUI::IPreviewLeaf* pLeaf, bool rowEven) :
				LeafPanelBase(pPadre->GetWxWindow()),
				INodoHistorial(pPadre),
				m_modificado(false),
				m_pLeaf(pLeaf),
				m_dibujada(false),
				m_pView(pView),
				m_rowEven(rowEven)
		{
			wxPanel::SetBackgroundColour(wxBG_STYLE_CUSTOM);
			//truncate string...
			wxString tmp = wxString::FromUTF8(pLeaf->GetLabel().c_str());
			wxClientDC dc(this);
			dc.SetFont(m_pLabel->GetFont());
			int x,y;
			dc.GetTextExtent(tmp, &x, &y, NULL, NULL, NULL);

			while(x > TEXT_LENGTH)
			{
				dc.GetTextExtent(tmp, &x, &y, NULL, NULL, NULL);
				tmp = tmp.substr(0,tmp.size()-1);
			}
			m_pLabel->SetLabel(tmp);
			m_pLabel1->SetLabel(wxString::FromUTF8(pLeaf->GetLabel2().c_str()));
			this->SetToolTip(wxString::FromUTF8(pLeaf->GetLabel().c_str()));
			m_pLabel->SetToolTip(wxString::FromUTF8(pLeaf->GetLabel().c_str()));
			//end truncate
			m_pThumbnail->Hide();
			Layout();
		}

		LeafPanel::~LeafPanel()
		{
			if (!m_pThumbnail->IsShown() && m_dibujada) {
				GNC::GCS::ThumbnailController::Instance()->removeRequest(this);
			}
		}

		void LeafPanel::Seleccionar(bool seleccionar,bool force)
		{
			if(m_seleccionado!=seleccionar || force){
				m_seleccionado = seleccionar;
				Refresh();
			}
		}

		wxWindow* LeafPanel::GetWxWindow()
		{
			return this;
		}

		void LeafPanel::SetImage(long , wxImage* pImage)
		{
			if(pImage != NULL && pImage->IsOk()) {
				wxBitmap overBitmap = m_pLeaf->GetOverlayBitmap();
				if (overBitmap.Ok())
				{
					wxImage& img = (*pImage);
					img.Paste(overBitmap.ConvertToImage(), 2,2);
				}
				wxBitmap bitmap(*pImage);
				m_pThumbnail->SetBitmap(bitmap);
				m_pThumbnail->Show();
			}
			Layout();
		}

		void LeafPanel::GetNodeSize(int &x, int &y)
		{
			GetSize(&x,&y);
		}

		void LeafPanel::OnLeafDClick(wxMouseEvent& event)
		{
			m_pLeaf->OnPreviewDClick(this);
			event.Skip(false);
		}

		void LeafPanel::OnChar(wxKeyEvent &event)
		{
			event.ResumePropagation(10);
			event.Skip(true);
		}

		void LeafPanel::OnLeafClick(wxMouseEvent &event)
		{
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GUI::Events::EventoSeleccionarHistorial(m_pView, this));
			m_pLeaf->OnPreviewLDown(this);
			event.Skip(false);
		}

		void LeafPanel::OnMouseWheel(wxMouseEvent& event)
		{
			event.ResumePropagation(2);
			event.Skip(true);
		}

		void LeafPanel::OnMenuMouse(wxMouseEvent& event)
		{
			m_pLeaf->OnShowMenu(this);
			event.Skip(false);
		}

		void LeafPanel::OnPaint(wxPaintEvent &event)
		{
			//background...
			wxPaintDC dc(this);
			const int width = dc.GetSize().x;
			const int height = dc.GetSize().y;

			if (m_seleccionado) {
				dc.SetBrush(wxBrush(FONDO_ACTIVO));
				dc.SetPen(wxPen(FONDO_ACTIVO, 1, wxSOLID));
				m_pLabel->SetBackgroundColour(FONDO_ACTIVO);
				m_pLabel1->SetBackgroundColour(FONDO_ACTIVO);
			} else {
				if (m_rowEven) {
					dc.SetBrush(wxBrush(FONDO_INACTIVO_E));
					dc.SetPen(wxPen(FONDO_INACTIVO_E, 1, wxSOLID));
					m_pLabel->SetBackgroundColour(FONDO_INACTIVO_E);
					m_pLabel1->SetBackgroundColour(FONDO_INACTIVO_E);
				} else {
					dc.SetBrush(wxBrush(FONDO_INACTIVO_O));
					dc.SetPen(wxPen(FONDO_INACTIVO_O, 1, wxSOLID));
					m_pLabel->SetBackgroundColour(FONDO_INACTIVO_O);
					m_pLabel1->SetBackgroundColour(FONDO_INACTIVO_O);
				}
			}
			dc.DrawRectangle(0,0,width,height);
			//
			if (!m_dibujada) {
				//la primera vez que se pinta se lanza el timer que generara el thumbnail (bajo demanda)
				GNC::GCS::ThumbnailController::Instance()->requestThumbnail(m_pLeaf->GetFilePk(), this);
				m_dibujada = true;
			}
			LeafPanelBase::OnPaint(event);

			wxColour borderColor(0,0,0);
			dc.SetBrush(wxBrush(borderColor,wxTRANSPARENT));
			dc.SetPen(wxPen(borderColor, 1, wxSOLID));
			dc.DrawLine(wxPoint(0,0), wxPoint(0, dc.GetSize().y-1));
			dc.DrawLine(wxPoint(0, dc.GetSize().y-1), wxPoint(dc.GetSize().x - 1, dc.GetSize().y-1));
			dc.DrawLine(wxPoint(dc.GetSize().x - 1, 0), wxPoint(dc.GetSize().x - 1, dc.GetSize().y-1));
		}

		void LeafPanel::OnEraseBackground( wxEraseEvent& event )
		{
			event.Skip(false);
		}
	}
}

