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
#include "wchromacomplejo.h"
#include <api/ievento.h>
#include <api/iwidgetsrenderer.h>
#include <api/iwidgetsmanager.h>
#include <api/icontexto.h>
#include "main/managers/widgetsmanager.h"
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>
#include <wx/xml/xml.h>

#define WRADIO_CRUZ 8.0f
#define WLONGITUD_LINEA 15.0f

//region "Implementacion de WSemillasBuilder"

GNC::GCS::Widgets::WChromaComplejoBuilder::WChromaComplejoBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid, int numeroPuntos, GNC::GCS::Widgets::Observadores::IObservadorChromaComplejo* pObservador) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid), GNC::GCS::Widgets::Notificadores::INotificadorChromaComplejo(pObservador)
{
	m_MouseDown = false;
	m_NumMaxSemillas = 1;
	m_Estado = WBS_Ninguno;
	m_numeroPuntos = numeroPuntos;
}

GNC::GCS::Widgets::WChromaComplejoBuilder::~WChromaComplejoBuilder()
{

}

void GNC::GCS::Widgets::WChromaComplejoBuilder::SetNumeroPuntos(int numeroPuntos)
{
	m_numeroPuntos = numeroPuntos;
	for (GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); it !=  m_pManager->GetListaWidgets().end(); ++it) {
		if ( (*it)->GetGID() == m_GID) {
			GNC::GCS::Widgets::WChromaComplejo* semilla = dynamic_cast<GNC::GCS::Widgets::WChromaComplejo*>((*it));
			if(semilla != NULL)
			{
				semilla->SetNumeroPuntos(numeroPuntos);
			}
		}
	}
}

void GNC::GCS::Widgets::WChromaComplejoBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	GTRACE("GNC::GCS::Widgets::WSemillasBuilder::OnMouseEvents(wxMouseEvent&)");
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {

		m_MouseDown = false;
		switch(m_Estado) {
			case WBS_Ninguno:
				// ¿Error?
				break;
			case WBS_Creando:
				{
					GNC::GCS::Widgets::WChromaComplejo* semilla = new GNC::GCS::Widgets::WChromaComplejo(m_pManager, evento.c->GetRenderer()->GetVID(), m_ListaNodos, m_numeroPuntos, evento.ControlDown(), "chromacomplejo", m_GID);
					
					if (m_NumMaxSemillas > 0) {
						int numSemillas = 0;
						for (GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); it !=  m_pManager->GetListaWidgets().end(); ++it) {
							if ( (*it)->GetGID() == m_GID) {
								numSemillas++;
								if (numSemillas >= m_NumMaxSemillas) {
									m_pManager->EliminarWidget(it);
									break;
								}
							}
						}
					}
					m_pManager->InsertarWidget(semilla);
					m_pManager->LanzarEventoCreacion(semilla);
					m_pManager->Modificado();
					ConsumirEvento();
				}
				break;
			case WBS_Seleccionando:
				break;
			case WBS_Moviendo:
				break;
		}
		m_Estado = WBS_Ninguno;

	} else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}

		if (m_NumMaxSemillas > 0) {
			int numSemillas = 0;
			for (GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); it !=  m_pManager->GetListaWidgets().end(); ++it) {
				if ( (*it)->GetGID() == m_GID) {
					numSemillas++;
					if (numSemillas >= m_NumMaxSemillas) {
						m_pManager->EliminarWidget(it);
						break;
					}
				}
			}
		}
		
		m_ListaNodos.clear();
		m_ListaNodos.push_back(GNC::GCS::Nodo(evento.iP));
		m_MouseDown = true;
		m_Estado = WBS_Creando;
		m_pManager->Modificado();
		ConsumirEvento();

	} else if (evento.Dragging() && m_MouseDown) {
		if (m_Estado == WBS_Creando) {
			m_ListaNodos.push_back(GNC::GCS::Nodo(evento.iP));
			m_pManager->Modificado();
			ConsumirEvento();
		}
	} else if (evento.Moving()) {
		m_PosCursor = evento.iP;
		m_pManager->Modificado();
		evento.Skip(false);
	}
}

void GNC::GCS::Widgets::WChromaComplejoBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evt)
{
	NotificarKeyEvent(evt);
}

void DibujaAspa(const GNC::GCS::Nodo& nodo, float radioCruz, bool rallado = false, float fsombrax = 0.0f, float fsombray = 0.0f)
{
	// Cruz
	if(rallado) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0xAAAA);
	}
	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x - radioCruz + fsombrax, nodo.y - fsombray, 0.0f);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x + radioCruz + fsombrax, nodo.y - fsombray, 0.0f);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x + fsombrax, nodo.y - radioCruz - fsombray, 0.0f);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x + fsombrax, nodo.y + radioCruz - fsombray, 0.0f);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glEnd();
	if(rallado)
		glDisable(GL_LINE_STIPPLE);
}

void DibujaAspaInclinada(const GNC::GCS::Nodo& nodo, float radioCruz, bool rallado = false, float fsombrax = 0.0f, float fsombray = 0.0f)
{
	// Cruz
	if(rallado) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0xAAAA);
	}
	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glVertex3f(nodo.x + radioCruz + fsombrax, nodo.y - radioCruz - fsombray, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glVertex3f(nodo.x + radioCruz + fsombrax, nodo.y + radioCruz - fsombray, 0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glVertex3f(nodo.x - radioCruz + fsombrax, nodo.y + radioCruz - fsombray, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(nodo.x + fsombrax, nodo.y - fsombray, 0.0f);
	glVertex3f(nodo.x - radioCruz + fsombrax, nodo.y - radioCruz - fsombray, 0.0f);
	glEnd();
	if(rallado)
		glDisable(GL_LINE_STIPPLE);
}

void GNC::GCS::Widgets::WChromaComplejoBuilder::DibujarVarita(GNC::GCS::Contexto3D* c)
{
	GNC::GCS::Vector escala = c->RelacionImagenPantalla();
	float radioCruz = WRADIO_CRUZ * escala.x;
	float longitudLinea = WLONGITUD_LINEA * escala.x;

	// Sombra
	float fsombrax, fsombray;

	GNC::GCS::Nodo m_Nodo;
	if (m_MouseDown)
	{
		 m_Nodo = m_ListaNodos.back();
		fsombrax = WIDGET_OFFSET_X_SOMBRA_SELECCIONADO * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO * escala.y;
	}
	else
	{
		m_Nodo = m_PosCursor;
		fsombrax = WIDGET_OFFSET_X_SOMBRA * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA * escala.y;
	}

	wColorSombra.Aplicar();

	glLineWidth(m_LineWidthProp);
	// cruz
	DibujaAspa(m_Nodo, radioCruz,true,fsombrax,fsombray);

	//linea de la cruz 45 grados xa abajo
	glBegin(GL_LINES);
	glVertex3f(m_Nodo.x + fsombrax, m_Nodo.y - fsombray, 0.0f);
	glVertex3f(m_Nodo.x + longitudLinea + fsombrax, m_Nodo.y + longitudLinea + fsombray, 0.0f);
	glEnd();

	// Cuerpo
	glLineWidth(m_DraggingLineWidthProp);
	wAplicarColor(m_Nodo.m_Iluminado, m_MouseDown, m_Nodo.m_Seleccionado);

	// Cruz
	DibujaAspa(m_Nodo, radioCruz, true);

	//linea de la cruz 45 grados xa abajo
	
	glBegin(GL_LINES);
	glVertex3f(m_Nodo.x, m_Nodo.y, 0.0f);
	glVertex3f(m_Nodo.x + longitudLinea, m_Nodo.y + longitudLinea, 0.0f);
	glEnd();
}

void GNC::GCS::Widgets::WChromaComplejoBuilder::Render(GNC::GCS::Contexto3D* c)
{		
	if (m_Estado == WBS_Creando && m_ListaNodos.size() != 0) {
		GNC::GCS::Vector escala = c->RelacionImagenPantalla();

		float radioCruz = WRADIO_CRUZ * escala.x;

		//se pinta la linea:
		glLineWidth(m_LineWidthProp);
		wAplicarColor(false, false, false);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0xAAAA);
		glBegin(GL_LINE_STRIP);
		for (TListaNodos::iterator it = m_ListaNodos.begin(); it != m_ListaNodos.end(); ++it) {
			glVertex2d((*it).x, (*it).y);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		wAplicarColor(true, false, true);
		glLineWidth(m_LineWidthProp);
		//se pintan los nodos representativos
		if(m_ListaNodos.size()<= (unsigned int)m_numeroPuntos)
		{
			// Dibujo las aspas
			for(TListaNodos::iterator it = m_ListaNodos.begin(); it!=m_ListaNodos.end(); ++it)
			{
				// Cruz
				DibujaAspaInclinada((*it),radioCruz);
			}
		} else {
			//metemos el último
			DibujaAspaInclinada(m_ListaNodos.back(),radioCruz);

			for(int i = 0; i < m_numeroPuntos - 1; i++) {
				int indice = (m_ListaNodos.size() / (m_numeroPuntos-1))*i;

				TListaNodos::iterator it = m_ListaNodos.begin();
				for (int i = 0; i < indice; i++, ++it);
				GNC::GCS::Nodo& nodo = *it;
				DibujaAspaInclinada(nodo, radioCruz);
			}
		}
	}

	DibujarVarita(c);
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WChromaComplejoBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_TRANSPARENTE;
}

//endregion


//region "Constructor y destructor"

GNC::GCS::Widgets::WChromaComplejo::WChromaComplejo(IWidgetsManager* pManager, long vid, GNC::GCS::Widgets::WChromaComplejo::TListaNodos listaNodos, int numeroPuntos, bool isAgregar, const char* nombre, long gid, bool oculto) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid)
{
	m_ListaNodos = listaNodos;
	m_numeroPuntos = numeroPuntos;
	m_isAgregar = isAgregar;
////////////////////////////////////	m_Nodo.m_Size = WRADIO_CRUZ;
	m_MouseDown = false;
	m_UID = (long)this;
	m_Oculto = oculto;
}

GNC::GCS::Widgets::WChromaComplejo::~WChromaComplejo()
{
	LanzarEventoDestruccion();
}

//endregion

void GNC::GCS::Widgets::WChromaComplejo::OnMouseEvents(GNC::GCS::Events::EventoRaton& /*evento*/)
{
}

void GNC::GCS::Widgets::WChromaComplejo::OnKeyEvents(GNC::GCS::Events::EventoTeclado& /*evento*/)
{
}

bool GNC::GCS::Widgets::WChromaComplejo::HitTest(float , float , const GNC::GCS::Vector&)
{
	return false;
}

bool GNC::GCS::Widgets::WChromaComplejo::HitTest(GNC::GCS::Vector* /*vertices*/, int /*numVertices*/)
{
	return false;
}

GNC::GCS::Widgets::WChromaComplejo::TListaNodos GNC::GCS::Widgets::WChromaComplejo::GetListaNodos()
{
	TListaNodos resultado;
	if(m_ListaNodos.size()<= (unsigned int)m_numeroPuntos)
	{
		for(TListaNodos::iterator it = m_ListaNodos.begin(); it!=m_ListaNodos.end(); ++it)
		{
			resultado.push_back((*it));
		}
	} else {
		//metemos el último
		resultado.push_back(m_ListaNodos.back());		
		for(int i = 0; i< m_numeroPuntos-1; i++) {
			int indice = (m_ListaNodos.size() / (m_numeroPuntos-1) )*i;
			// Dibujo los puntos

			TListaNodos::iterator it = m_ListaNodos.begin();
			for (int i = 0; i < indice; i++, ++it);
			GNC::GCS::Nodo& nodo = *it;
			resultado.push_back(nodo);
		}
	}
	return resultado;
}

void GNC::GCS::Widgets::WChromaComplejo::SetNumeroPuntos(int numeroPuntos)
{
	m_numeroPuntos = numeroPuntos;
	Modificar(true);
	m_pManager->NotificarModificacion(this);
}

void GNC::GCS::Widgets::WChromaComplejo::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	GNC::GCS::Vector escala = c->RelacionImagenPantalla();

	float radioCruz = WRADIO_CRUZ * escala.x;

	//se pinta la linea:
	wAplicarColor(false, false, false);
	glLineWidth(m_LineWidthProp);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(3, 0xAAAA);
	glBegin(GL_LINE_STRIP);
	for (TListaNodos::iterator it = m_ListaNodos.begin(); it != m_ListaNodos.end(); ++it) {
		glVertex2d((*it).x, (*it).y);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	//se pintan los nodos representativos
	wAplicarColor(true, false, true);
	glLineWidth(m_LineWidthProp);
	TListaNodos puntos = GetListaNodos();
	for(TListaNodos::iterator it = puntos.begin(); it!=puntos.end(); ++it)
	{
		// Dibujo los puntos
		glPointSize(WIDGET_SIZE_PUNTO);
		DibujaAspaInclinada((*it),radioCruz);
	}
}

//region Interfaz de eventos ginkgo

