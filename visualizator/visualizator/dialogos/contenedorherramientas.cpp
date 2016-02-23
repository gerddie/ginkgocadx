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

#include <api/globals.h>
#include "contenedorherramientas.h"
#include <iostream>
namespace GNKVisualizator{
	namespace GUI{
		ContenedorHerramientas::ContenedorHerramientas(wxWindow* pPadre) : wxControl(pPadre, -1, wxDefaultPosition, wxDefaultSize)//wxScrolledWindow(pPadre, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL)
		{
			Hide();
			SetMinSize(wxSize(300, -1));
			Connect(wxEVT_SIZE, wxSizeEventHandler(ContenedorHerramientas::OnSize));
			m_pSizer = new wxBoxSizer(wxVERTICAL);
			this->SetSizer(m_pSizer);
			m_pSizer->Fit(this);
			Layout();
		}

		ContenedorHerramientas::~ContenedorHerramientas()
		{
			 Disconnect(wxEVT_SIZE, wxSizeEventHandler(ContenedorHerramientas::OnSize));
		}

		void ContenedorHerramientas::InsertarPanel(wxWindow* child)
		{			
			m_pSizer->Add(child, 1, wxEXPAND, 0);
			Layout();
		}

		void ContenedorHerramientas::EliminarPanel(wxWindow* child)
		{
			m_pSizer->Detach(child);
			Layout();
		}

		void ContenedorHerramientas::OnSize(wxSizeEvent& WXUNUSED(event))
		{
			Layout();
		}

		void ContenedorHerramientas::ActualizarVirtualSize() {
			Layout();
		}
	}
}
