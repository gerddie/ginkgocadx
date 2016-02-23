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
#include "wrectangulo.h"
#include <api/ievento.h>
#include <api/icontexto.h>
#include <api/iwidgetsrenderer.h>
#include "main/managers/widgetsmanager.h"
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>

//region "Implementacion de WRectanguloBuilder"

GNC::GCS::Widgets::WRectanguloBuilder::WRectanguloBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	m_Dentro = true;
}

GNC::GCS::Widgets::WRectanguloBuilder::~WRectanguloBuilder()
{
}

void GNC::GCS::Widgets::WRectanguloBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	//------------------------------------------------------
	// Dragging
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
		m_MouseDown = false;
		if (m_Estado != WBS_Creando) {
			m_Estado = WBS_Ninguno;
			return;
		}

		m_Estado = WBS_Ninguno;

		m_Stop= evento.iP;

		m_Vertices[0].Asignar(m_Start.x, m_Start.y);
		m_Vertices[1].Asignar(m_Stop.x, m_Start.y);
		m_Vertices[2].Asignar(m_Stop.x, m_Stop.y);
		m_Vertices[3].Asignar(m_Start.x, m_Stop.y);

		TVector diff = m_Stop - m_Start;
		if (std::abs(diff.x) > 0.001f && std::abs(diff.y) > evento.c->RelacionImagenPantalla().NormaInfinito()) {
			WRectangulo* w = new WRectangulo(m_Vertices, m_pManager, evento.c->GetRenderer()->GetVID(), "rectangulo", m_GID);
			m_pManager->InsertarWidget(w);
			m_pManager->LanzarEventoCreacion(w);
		}

		m_pManager->Modificado();
		m_Estado = WBS_Ninguno;
		ConsumirEvento();
	}
	else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}
		GTRACE("WRectanguloBuilder::OnMouseEvents:: Left down)");
		m_MouseDown = true;

		m_Start= evento.iP;
		m_Stop = m_Start;

		m_Vertices[0].Asignar(m_Start.x, m_Start.y);
		m_Vertices[1].Asignar(m_Stop.x, m_Start.y);
		m_Vertices[2].Asignar(m_Stop.x, m_Stop.y);
		m_Vertices[3].Asignar(m_Start.x, m_Stop.y);

		m_pManager->Modificado();
		m_Estado = WBS_Creando;
		ConsumirEvento();
	}
	else if (evento.Dragging() && m_MouseDown) {
		if (m_Estado != WBS_Creando) {
			return;
		}
		GTRACE("WRectanguloBuilder::OnMouseEvents:: Creando...)");

		m_Stop= evento.iP;

		m_Vertices[0].Asignar(m_Start.x, m_Start.y);
		m_Vertices[1].Asignar(m_Stop.x, m_Start.y);
		m_Vertices[2].Asignar(m_Stop.x, m_Stop.y);
		m_Vertices[3].Asignar(m_Start.x, m_Stop.y);

		m_pManager->Modificado();
		ConsumirEvento();
	}
	else if( evento.Moving() ){
		m_NodoMoviendose= evento.iP;
		m_Estado = WBS_Ninguno;
	}
}

void GNC::GCS::Widgets::WRectanguloBuilder::OnKeyEvents(TEventoTeclado&)
{

}

void GNC::GCS::Widgets::WRectanguloBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

	if (m_Estado == WBS_Creando) {
		glLineWidth(2.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0xAAAA);
		glColor4f(0.0f, 0.5f, 0.0f, 0.75f);
		glBegin(GL_LINE_LOOP);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
		glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
		glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
		glEnd();

		glDisable(GL_LINE_STIPPLE);
	}
	else {

	}
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WRectanguloBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_RECTANGULO;
}

//endregion



//region "Constructor y destructor"

GNC::GCS::Widgets::WRectangulo::WRectangulo(TVector bb[4], IWidgetsManager* pManager, long vid, const char* nombre, long gid) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid)
{
	m_Vertices[0] = bb[0];
	m_Vertices[1] = bb[1];
	m_Vertices[2] = bb[2];
	m_Vertices[3] = bb[3];
	m_MouseDown = false;
	m_UID = (long)this;
}

GNC::GCS::Widgets::WRectangulo::~WRectangulo()
{
	LanzarEventoDestruccion();
}

//endregion

void GNC::GCS::Widgets::WRectangulo::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if(m_Oculto){
		return;
	}
	//------------------------------------------------------
	// Leaving
	if (evento.Leaving() ) {
		Iluminar(false);
	}
	//------------------------------------------------------
	// Entering
	else if (evento.Entering() ) {
		if (m_MouseDown) {
			Iluminar(true);
		}
	}
	//------------------------------------------------------
	// Dragging
	else if (evento.Dragging() && m_MouseDown) {
		Vector delta = evento.iP - m_PosCursor;
		m_PosCursor = evento.iP;

		if (EstaSeleccionado())  {
			m_Vertices[0] += delta;
			m_Vertices[1] += delta;
			m_Vertices[2] += delta;
			m_Vertices[3] += delta;
		}
		else if (m_Vertices[0].m_Seleccionado) {
			m_Vertices[3].x += delta.x;
			m_Vertices[0] += delta;
			m_Vertices[1].y += delta.y;
		}
		else if (m_Vertices[1].m_Seleccionado) {
			m_Vertices[2].x += delta.x;
			m_Vertices[1] += delta;
			m_Vertices[0].y += delta.y;
		}
		else if (m_Vertices[2].m_Seleccionado) {
			m_Vertices[1].x += delta.x;
			m_Vertices[2] += delta;
			m_Vertices[3].y += delta.y;
		}
		else if (m_Vertices[3].m_Seleccionado) {
			m_Vertices[0].x += delta.x;
			m_Vertices[3] += delta;
			m_Vertices[2].y += delta.y;
		}
		LanzarEventoModificacion();
		Modificar(true);
		ConsumirEvento();
		return;
	}
	//------------------------------------------------------
	// LeftDown
	else if (evento.LeftDown()) {

		if (EventoConsumido() && !evento.m_controlDown) {
			SeleccionarNodo(m_Vertices[0], false);
			SeleccionarNodo(m_Vertices[1], false);
			SeleccionarNodo(m_Vertices[2], false);
			SeleccionarNodo(m_Vertices[3], false);
			Seleccionar(false);
			return;
		}

		bool dentro = false;

		m_PosCursor= evento.iP;

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		for (int i = 0; i < 4 && !dentro; ++i)
		{
			if (m_Vertices[i].Hits(evento.iP, factor)) {
				SeleccionarNodo(m_Vertices[i], true);
				IluminarNodo(m_Vertices[i], true);
				for (int j = 0; j < 4; ++j)
				{
					if (i != j) {
						SeleccionarNodo(m_Vertices[i], false);
					}
				}
				Seleccionar(false);
				dentro = true;
			}
			else {
				SeleccionarNodo(m_Vertices[i], false);
				IluminarNodo(m_Vertices[i], false);
			}
		}

		if (!dentro) {
			if (evento.iP.DentroDePoligono2(m_Vertices, 4)) {
				if (evento.m_controlDown) { // Inversion de seleccion
					InvertirSeleccion();
				}
				else{
					Seleccionar(true);
				}
				dentro = true;
			}
			else { // Click fuera del nodo
				if (!evento.m_controlDown) {
					Seleccionar(false);
				}
				else {
					ConsumirEvento();
					if (EstaSeleccionado()) {
						m_MouseDown = true;
						m_PosCursor= evento.iP;
					}
				}
			}
		}

		if (dentro) {
			m_MouseDown = true;
			m_PosCursor= evento.iP;
			ConsumirEvento();
		}

		return;
	}
	//------------------------------------------------------
	// LeftUP
	else if (evento.LeftUp()) {
		if (m_MouseDown) {
			m_MouseDown = false;
			ConsumirEvento();
		}
	}
	//------------------------------------------------------
	// Moving
	else if (evento.Moving()) {
		if (EventoConsumido()) {
			IluminarNodo(m_Vertices[0], false);
			IluminarNodo(m_Vertices[1], false);
			IluminarNodo(m_Vertices[2], false);
			IluminarNodo(m_Vertices[3], false);
			Iluminar(false);
			return;
		}

		bool dentro = false;

		m_PosCursor= evento.iP;

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		for (int i = 0; i < 4 && !dentro; ++i)
		{
			if (m_Vertices[i].Hits(evento.iP, factor)) {
				IluminarNodo(m_Vertices[i], true);
				for (int j = 0; j < 4; ++j)
				{
					if (j != i) {
						IluminarNodo(m_Vertices[j], false);
					}
				}
				Iluminar(false);
				dentro = true;
			}
			else {
				IluminarNodo(m_Vertices[i], false);
			}
		}

		if (!dentro) {

			if (evento.iP.DentroDePoligono2(m_Vertices, 4)) {
				Iluminar(true);
				dentro = true;
			}
			else {
				Iluminar(false);
			}
		}

		if (dentro) {
			ConsumirEvento();
		}

		return;
	}
}

void GNC::GCS::Widgets::WRectangulo::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WRectangulo::HitTest(float /*x*/, float /*y*/, const GNC::GCS::Vector&)
{
	return false;
}

bool GNC::GCS::Widgets::WRectangulo::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	return
		m_Vertices[0].DentroDePoligono2(vertices, numVertices) &&
		m_Vertices[1].DentroDePoligono2(vertices, numVertices) &&
		m_Vertices[2].DentroDePoligono2(vertices, numVertices) &&
		m_Vertices[3].DentroDePoligono2(vertices, numVertices);
}

void GNC::GCS::Widgets::WRectangulo::Render(GNC::GCS::Contexto3D* /*c*/)
{
	if(m_Oculto){
		return;
	}

	wColorSombra.Aplicar();
	glBegin(GL_QUADS);
	glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
	glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
	glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
	glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
	glEnd();

	if (m_Seleccionado) {
		glLineWidth(2.0f);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
		glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
		glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
		glEnd();
	}

	if (m_Vertices[0].m_Seleccionado || m_Vertices[0].m_Iluminado) {
		glPointSize(5.0f);
		if (m_Vertices[0].m_Seleccionado) {
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		}

		glBegin(GL_LINE_STRIP);
		glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
		glEnd();

		glBegin(GL_POINTS);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
		glEnd();
	}
	else if (m_Vertices[1].m_Seleccionado || m_Vertices[1].m_Iluminado) {
		if (m_Vertices[1].m_Seleccionado) {
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		}

		glBegin(GL_LINE_STRIP);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
		glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
		glEnd();

		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
		glEnd();

	}
	else if (m_Vertices[2].m_Seleccionado || m_Vertices[2].m_Iluminado) {
		if (m_Vertices[2].m_Seleccionado) {
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		}

		glBegin(GL_LINE_STRIP);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
		glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
		glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
		glEnd();

		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
		glEnd();
	}
	else if (m_Vertices[3].m_Seleccionado || m_Vertices[3].m_Iluminado) {
		if (m_Vertices[3].m_Seleccionado) {
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		}

		glBegin(GL_LINE_STRIP);
		glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
		glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
		glEnd();

		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
		glEnd();
	}
}

void GNC::GCS::Widgets::WRectangulo::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	const GNC::GCS::Vector& escala = c->RefRelacionMundoPantallaOffscreen();

	TVector Vertices[4] = {c->Reproyectar(m_Vertices[0]) * c->factorReescalado, c->Reproyectar(m_Vertices[1]) * c->factorReescalado, c->Reproyectar(m_Vertices[2]) * c->factorReescalado, c->Reproyectar(m_Vertices[3]) * c->factorReescalado};

	cairo_set_line_width(c->cr, m_LineWidthProp);

	// Color del cuerpo
	wColorNormal.AplicarCairo(c->cr);

	// Dibujo  de los puntos
	cairo_arc(c->cr, Vertices[0].x, Vertices[0].y, (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
	cairo_fill(c->cr);
	cairo_arc(c->cr, Vertices[1].x, Vertices[1].y, (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
	cairo_fill(c->cr);
	cairo_arc(c->cr, Vertices[2].x, Vertices[2].y, (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
	cairo_fill(c->cr);
	cairo_arc(c->cr, Vertices[3].x, Vertices[3].y, (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
	cairo_fill(c->cr);

	// Dibujo rectangulo
	cairo_move_to(c->cr, Vertices[0].x, Vertices[0].y);
	cairo_line_to(c->cr, Vertices[1].x, Vertices[1].y);
	cairo_line_to(c->cr, Vertices[2].x, Vertices[2].y);
	cairo_line_to(c->cr, Vertices[3].x, Vertices[3].y);
	cairo_line_to(c->cr, Vertices[0].x, Vertices[0].y);
	cairo_stroke(c->cr);
}

