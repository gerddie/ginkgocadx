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

#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif
#include <list>
#include <vtkgl.h>
#include <cmath>
#include <cairo/cairo.h>

#include <wx/xml/xml.h>
#include <wx/menu.h>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/math/geometry3d.h>

#include "wnota.h"
#include "wtextbox.h"
#include <api/ievento.h>
#include <api/iwidgetsrenderer.h>
#include <api/icontexto.h>
#include <api/iwidgetsmanager.h>

#include <api/westilo.h>


//====================================================================================================
//= Builder de notas
//====================================================================================================
GNC::GCS::Widgets::WNotaBuilder::WNotaBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask) : IWidgetBuilder(pManager, buttonMask)
{
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	m_pTempWidget = NULL;
}

GNC::GCS::Widgets::WNotaBuilder::~WNotaBuilder()
{
	if (m_pTempWidget != NULL) {
		delete m_pTempWidget;
	}
}

void GNC::GCS::Widgets::WNotaBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
		m_MouseDown = false;
		if (m_Estado != WBS_Creando) {
			m_Estado = WBS_Ninguno;
			return;
		}
		m_Estado = WBS_Ninguno;
		m_Posicion= evento.iP;
		m_pManager->InsertarWidget(m_pTempWidget);
		m_pManager->LanzarEventoCreacion(m_pTempWidget);
		WTextBox* pTmp = m_pTempWidget;
		m_pTempWidget = NULL;
		pTmp->DoEditText(evento.c->pWindow);
		evento.Skip(false);
		m_pManager->Modificado();

	} else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}
		m_MouseDown = true;
		m_Posicion= evento.iP;
		if (m_pTempWidget != NULL) {
			delete m_pTempWidget;
		}
		m_pTempWidget = new WTextBox(m_pManager, evento.c->GetRenderer()->GetVID(), m_Posicion, WTextBox::TNM_Static, "TextNote");
		m_pManager->LanzarEventoCreacion(m_pTempWidget);
		m_pManager->Modificado();
		m_Estado = WBS_Creando;
		evento.Skip(false);
	} else if (evento.Dragging() && m_MouseDown) {
		if (m_Estado != WBS_Creando) {
			return;
		}
		m_Posicion= evento.iP;
		if (m_pTempWidget != NULL) {
			m_pTempWidget->m_Posicion = m_Posicion;
		}
		m_pManager->Modificado();
		evento.Skip(false);
	}else if( evento.Moving() ){
	}
}

void GNC::GCS::Widgets::WNotaBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
}

void GNC::GCS::Widgets::WNotaBuilder::Render(GNC::GCS::Contexto3D* c)
{
	if (m_Estado != WBS_Creando) {
		return;
	}
	if (m_pTempWidget == NULL) {
		return;
	}
	m_pTempWidget->Render(c);
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WNotaBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_ANOTACION;
}
