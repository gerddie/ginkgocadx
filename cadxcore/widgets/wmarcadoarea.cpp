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
#include <api/icontexto.h>
#include "wmarcadoarea.h"
#include <api/iwidgetsrenderer.h>
#include <api/ievento.h>
#include "main/managers/widgetsmanager.h"
#include <api/controllers/icontroladorpermisos.h>

#include <iterator>
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>


//====================================================================================================
//= Builder de marcado
//====================================================================================================

//region "Implementacion de WMarcadoAreaBuilder"

GNC::GCS::Widgets::WMarcadoAreaBuilder::WMarcadoAreaBuilder(ModoMarcado modo, vtkImageData* /*pImageData*/, GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid, GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid), GNC::GCS::Widgets::Notificadores::INotificadorMarcadoArea(pObservador)
{
        m_NumNodos = 0;
        m_MouseDown = false;
        m_Estado = WBS_Ninguno;
        SetRadioNodo(5.0f);
        m_pPoligonoCreado = NULL;
        m_pImageData = NULL;
        m_Modo = modo;
        m_LineWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.lineWidth").ObtenerValor<float>();
        m_DraggingLineWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.draggingLineWidth").ObtenerValor<float>();
}

GNC::GCS::Widgets::WMarcadoAreaBuilder::~WMarcadoAreaBuilder()
{
        m_Vertices.clear();
        if (m_Modo == MP_Lazo) {
                if (m_pPoligonoCreado) {
                        NotificarDestruccion(m_pPoligonoCreado);
                        m_pManager->EliminarWidget(m_pPoligonoCreado, false);
                        m_pPoligonoCreado = NULL;
                }
        }
}

void GNC::GCS::Widgets::WMarcadoAreaBuilder::EliminarWidget(bool notificar)
{
        if (m_Modo == MP_Lazo) {
                if (m_pPoligonoCreado) {
                        if(notificar) {
                                NotificarDestruccion(m_pPoligonoCreado);
                        }
                        m_pManager->EliminarWidget(m_pPoligonoCreado, false);
                        m_pPoligonoCreado = NULL;
                }
        }
}

void GNC::GCS::Widgets::WMarcadoAreaBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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
                m_PosCursor= evento.iP;

                if (m_Modo == MP_Lazo && m_pPoligonoCreado != NULL) {
                        NotificarDestruccion(m_pPoligonoCreado);
                        m_pManager->EliminarWidget(m_pPoligonoCreado, false);
                        m_pPoligonoCreado = NULL;
                }
                if(m_Vertices.size()>2) {
                        m_pPoligonoCreado = new WMarcadoArea(m_Modo, m_pManager, evento.c->GetRenderer()->GetVID(), m_Vertices, m_GID, m_pObservadorMarcado, "poligono");
                        m_pManager->InsertarWidget(m_pPoligonoCreado);
                        NotificarCreacion(m_pPoligonoCreado);
                        m_pManager->LanzarEventoCreacion(m_pPoligonoCreado);
                }
                m_Vertices.clear();
                evento.Skip(false);
                m_pManager->Modificado();

        } else if (evento.ButtonDown(m_ButtonMask)) {
                if (m_Estado != WBS_Ninguno) {
                        return;
                }

                m_MouseDown = true;
                if (m_Modo == MP_Lazo && m_pPoligonoCreado != NULL) {
                        NotificarDestruccion(m_pPoligonoCreado);
                        m_pManager->EliminarWidget(m_pPoligonoCreado, false);
                        m_pPoligonoCreado = NULL;
                }

                //aviso que han soltado
                m_pManager->Modificado();
                m_Estado = WBS_Creando;
                ConsumirEvento();

        } else if (evento.Dragging() && m_MouseDown) {

                if (m_Estado != WBS_Creando) {
                        return;
                }

                GTRACE(" ---->"<<(evento.iP - m_PosCursor).Norma2Cuadrado()<<" minimo= " << m_DistanciaMinimaCuadrado);

                if (!m_Vertices.empty() && ((evento.iP - m_PosCursor)/evento.c->RelacionImagenPantalla()).Norma2Cuadrado() < m_DistanciaMinimaCuadrado) {
                        return;
                }
                m_PosCursor = evento.iP;
                m_Vertices.push_back(m_PosCursor);
                m_pManager->Modificado();
                ConsumirEvento();
        } else if( evento.Moving() ) {
                m_PosCursor= evento.iP;
                m_Estado = WBS_Ninguno;
                ConsumirEvento();
        }
}

void GNC::GCS::Widgets::WMarcadoAreaBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
        NotificarKeyEvent(evento);
}

void GNC::GCS::Widgets::WMarcadoAreaBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

        if (m_Estado != WBS_Creando) {
                return;
        }

        glLineWidth(m_LineWidthProp);
        wColorNormal.Aplicar();

        VerticesPoligono::iterator prev = m_Vertices.begin();
        if (prev != m_Vertices.end()) {
                VerticesPoligono::iterator curr = prev;
                ++curr;
                for (; curr != m_Vertices.end(); ++curr) {
                        glBegin(GL_LINES);
                        glVertex2d((*prev).x, (*prev).y);
                        glVertex2d((*curr).x, (*curr).y);
                        glEnd();
                        prev = curr;
                }
        }

        if (m_Vertices.size() > 2) {
                glEnable(GL_LINE_STIPPLE);
                glColor4f(0.7f, 0.7f, 0.0f, 1.0);
                glLineStipple(3, 0xAAAA);
                glBegin(GL_LINES);
                glVertex2d(m_Vertices.front().x, m_Vertices.front().y);
                glVertex2d(m_Vertices.back().x, m_Vertices.back().y);
                glEnd();
                glDisable(GL_LINE_STIPPLE);
        }
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WMarcadoAreaBuilder::GetCursor()
{
        return GNC::GCS::Widgets::CUR_CREAR_POLIGONO;
}

//endregion


//====================================================================================================
//= Widget de marcado
//====================================================================================================

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//region "Constructor y destructor"

GNC::GCS::Widgets::WMarcadoArea::WMarcadoArea(GNC::GCS::Widgets::WMarcadoAreaBuilder::ModoMarcado modo, IWidgetsManager* pManager,
                unsigned int vid, VerticesPoligono& vertices, long gid,
                GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador, const char* nombre) :
        GNC::GCS::Widgets::IWidget(pManager, vid, nombre,gid), GNC::GCS::Widgets::Notificadores::INotificadorMarcadoArea(pObservador),
        m_AreaMetrica(0.0f)
{
        m_Vertices = vertices;

        m_RadioNodo = 0.0f;
        m_Modo = modo;
        Recalcular();
        m_MouseDown = false;
        m_PosCursor = m_Centro;

}

GNC::GCS::Widgets::WMarcadoArea::~WMarcadoArea()
{
        if(m_Modo != GNC::GCS::Widgets::WMarcadoAreaBuilder::MP_Normal) {
                LanzarEventoDestruccion();
        }
}

//endregion


void GNC::GCS::Widgets::WMarcadoArea::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
        if(EstaOculto()) {
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

                if (EstaSeleccionado()) {
                        for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
                                (*it) += delta;
                        }

                        Recalcular();

                        Modificar(true);

                        LanzarEventoModificacion();
                }

                ConsumirEvento();

                return;
        }
        //------------------------------------------------------
        // LeftDown
        else if (evento.LeftDown()) {

                if (EventoConsumido() && !evento.m_controlDown) {
                        Seleccionar(false);
                        return;
                }

                bool dentro = false;

                if (evento.iP.DentroDePoligono2(m_Vertices)) {
                        dentro = true;
                        if (evento.m_controlDown) { // Inversion de seleccion
                                InvertirSeleccion();
                        } else {
                                Seleccionar(true);
                        }
                } else {
                        if (!evento.m_controlDown) {
                                Seleccionar(false);
                        } else {
                                ConsumirEvento();
                                if (EstaSeleccionado()) {
                                        m_MouseDown = true;
                                        m_PosCursor= evento.iP;
                                }
                        }
                }

                if (dentro) {
                        m_MouseDown = true;
                        m_PosCursor= evento.iP;
                        m_Modificado = true;
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
                        NotificarModificacion(this);
                }
        }
        //------------------------------------------------------
        // Moving
        else if (evento.Moving()) {

                if (EventoConsumido()) {
                        Iluminar(false);
                        return;
                }

                bool dentro = false;

                m_PosCursor= evento.iP;

                if (evento.iP.DentroDePoligono2(m_Vertices)) {
                        Iluminar(true);
                        dentro = true;
                } else {
                        Iluminar(false);
                }
                if (dentro) {
                        ConsumirEvento();
                }
        }

}

void GNC::GCS::Widgets::WMarcadoArea::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
        if (evento.GetKeyCode() == WXK_ESCAPE) {
                Seleccionar(false);
        }
}

bool GNC::GCS::Widgets::WMarcadoArea::HitTest(float x, float y, const GNC::GCS::Vector& u)
{
        GNC::GCS::Vector punto(x, y);
        GNC::GCS::Vector umbral(u.x * u.x, u.y * u.y);

        bool hits = HitTest(&punto, 1);

        if (! hits ) {
                punto.Asignar(x - umbral.x, y - umbral.y);
                hits = HitTest(&punto, 1);
        }
        if (! hits ) {
                punto.Asignar(x, y - umbral.y);
                hits = HitTest(&punto, 1);
        }
        if (! hits ) {
                punto.Asignar(x + umbral.x, y + umbral.y);
                hits = HitTest(&punto, 1);
        }
        if (! hits ) {
                punto.Asignar(x + umbral.x, y);
                hits = HitTest(&punto, 1);
        }
        if (! hits ) {
                punto.Asignar(x + umbral.x, y - umbral.y);
                hits = HitTest(&punto, 1);
        }
        if (! hits ) {
                punto.Asignar(x , y - umbral.y);
                hits = HitTest(&punto, 1);
        }
        if (! hits ) {
                punto.Asignar(x - umbral.x, y - umbral.y);
                hits = HitTest(&punto, 1);
        }
        if (! hits ) {
                punto.Asignar(x - umbral.x, y);
                hits = HitTest(&punto, 1);
        }
        return hits;
}

bool GNC::GCS::Widgets::WMarcadoArea::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
        bool hits = true;

        for (VerticesPoligono::iterator it = m_Vertices.begin(); hits && it != m_Vertices.end(); ++it) {
                if (!(*it).DentroDePoligono2(vertices, numVertices) ) {
                        hits = false;
                }
        }

        return hits;
}

void GNC::GCS::Widgets::WMarcadoArea::Render(GNC::GCS::Contexto3D* c)
{
        if(m_Oculto) {
                return;
        }

        GNC::GCS::Vector escala = c->RelacionImagenPantalla();
        glLineWidth(m_LineWidthProp);

        glBegin(GL_POINTS);
        for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
                glVertex2d((*it).x , (*it).y );
        }
        glEnd();

        // Dibujo el poligono
        wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_LINE_LOOP);
        for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
                glVertex2d((*it).x, (*it).y);
        }
        glEnd();

        m_Modificado = false;
}

void GNC::GCS::Widgets::WMarcadoArea::Seleccionar(bool seleccionado)
{
        if (m_Seleccionado != seleccionado) {
                m_Seleccionado = seleccionado;
                m_Modificado = true;
        }
}

void GNC::GCS::Widgets::WMarcadoArea::Iluminar(bool iluminado)
{
        if (m_Iluminado != iluminado) {
                m_Iluminado = iluminado;
                m_Modificado = true;
        }
}

void GNC::GCS::Widgets::WMarcadoArea::Ocultar(bool oculto)
{
        if(oculto != m_Oculto) {
                m_Oculto = oculto;
                m_Modificado = true;
        }
}

//endregion


void GNC::GCS::Widgets::WMarcadoArea::Recalcular()
{
        m_Centro = GNC::GCS::Vector::Centroide(m_Vertices);
        m_Perimetro = GNC::GCS::Vector::PerimetroDePoligono(m_Vertices);
}

//endregion







