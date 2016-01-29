/*
 *  
 *  $Id: contenedorherramientas.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
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
