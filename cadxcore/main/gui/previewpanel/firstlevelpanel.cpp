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

#include "firstlevelpanel.h"
#include <sstream>
#include <wx/dcbuffer.h>
#include <main/entorno.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorlog.h>
#include <resources/ginkgoresourcesmanager.h>
#include "leafpanel.h"

#define TEXT_LENGTH 75
#define LEAF_HEIGHT 120
#define FIRST_LEVEL_HEADER_HEIGHT 30

#define FONDO_FIRST wxColour(194,194,194)

namespace GNC {
	namespace GUI {		
		FirstLevelPanel::FirstLevelPanel(GNC::GCS::IVista* pView, INodoHistorial* pParent,  GNC::GUI::IPreviewNode* pNode): FirstLevelPanelBase(pParent->GetWxWindow()), 
			INodoHistorial(pParent),
			m_pView(pView)
		{
			wxPanel::SetBackgroundColour(*wxWHITE);

			m_pNode = pNode;
					
			//truncate string...
			wxString tmp = wxString::FromUTF8(pNode->GetLabel().c_str());
			wxClientDC dc(this);
			dc.SetFont(m_pLabel->GetFont());
			int x,y;
			dc.GetTextExtent(tmp, &x, &y, NULL, NULL, NULL);

			while(x > TEXT_LENGTH)
			{
				dc.GetTextExtent(tmp, &x, &y, NULL, NULL, NULL);
				tmp = tmp.substr(0,tmp.size()-2);
			}
			m_pLabel->SetLabel(tmp);
		
			this->SetToolTip(wxString::FromUTF8(pNode->GetLabel().c_str()));
			m_pLabel->SetToolTip(wxString::FromUTF8(pNode->GetLabel().c_str()));
			//end truncate
			Layout();
		}

		FirstLevelPanel::~FirstLevelPanel()
		{
		}

		void FirstLevelPanel::GetNodeSize(int &x, int &y)
		{
			GetSize(&x,&y);
		}

		void FirstLevelPanel::OnChar(wxKeyEvent &event)
		{
			event.ResumePropagation(10);
			event.Skip(true);
		}
		
		wxWindow* FirstLevelPanel::GetWxWindow()
		{
			return this;
		}

		void FirstLevelPanel::OnPaint(wxPaintEvent& event)
		{
			wxPaintDC dc(this);
			const int width = dc.GetSize().x;
			const int height = dc.GetSize().y;

			dc.SetBrush(wxBrush(FONDO_FIRST));
			dc.SetPen(wxPen(FONDO_FIRST, 1, wxSOLID));
			m_pLabel->SetBackgroundColour(FONDO_FIRST);				
			
			dc.DrawRectangle(0,0,width,height);
			//
			FirstLevelPanelBase::OnPaint(event);

			wxColour borderColor(0,0,0);
			dc.SetBrush(wxBrush(borderColor,wxTRANSPARENT));
			dc.SetPen(wxPen(borderColor, 1, wxSOLID));
			dc.DrawRectangle(0,0,width,height - 1);
		}

		void FirstLevelPanel::OnSize(wxSizeEvent & event)
		{
			Layout();
			for (wxWindowList::iterator it = m_children.begin(); it != m_children.end(); ++it)
			{
				(*it)->Layout();
			}
			event.Skip();
		}

		void FirstLevelPanel::OnMouseWheel(wxMouseEvent& event)
		{
			event.ResumePropagation(2);
			event.Skip(true);
		}


		void FirstLevelPanel::OnEraseBackground( wxEraseEvent& event )
		{
			event.Skip(false);
		}
	};
};
