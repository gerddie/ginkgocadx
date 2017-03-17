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

#include <wx/xml/xml.h>
#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include "wtrapezoide.h"
#include "wpolygon.h"
#include <api/ievento.h>
#include <api/icontexto.h>
#include <api/iwidgetsrenderer.h>
#include <api/iwidgetsmanager.h>
#include <vtk/gl.h>
#include <api/westilo.h>
#include <cmath>


//region "Implementacion de WTrapezoideBuilder , se construye exactamente igual a un rectangulo"

GNC::GCS::Widgets::WTrapezoideBuilder::WTrapezoideBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid,  int numMaxTrapezoides) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
        m_MouseDown = false;
        m_Estado = WBS_Ninguno;
        m_Dentro = true;
        m_NumMaxTrapezoides = numMaxTrapezoides;
}

GNC::GCS::Widgets::WTrapezoideBuilder::~WTrapezoideBuilder()
{
}

void GNC::GCS::Widgets::WTrapezoideBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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

                m_Stop= evento.iP;

                GNC::GCS::Vector a = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[0]);
                GNC::GCS::Vector b = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[1]);
                m_Vertices[0] = m_Start;
                m_Vertices[1] = a;
                m_Vertices[2] = m_Stop;
                m_Vertices[3] = b;

                m_pManager->Modificado();
                TVector diff = m_Stop - m_Start;
                if (std::abs(diff.x) > evento.c->RelacionImagenPantalla().NormaInfinito() && std::abs(diff.y) > evento.c->RelacionImagenPantalla().NormaInfinito()) {
                        GNC::GCS::Widgets::TPolygonVertexList vertexList;
                        for (int i = 0; i < 4; ++i) {
                                GNC::GCS::Nodo n;
                                n = m_Vertices[i];
                                vertexList.push_back(n);
                        }
                        WPolygon* w = new WPolygon(m_pManager, evento.c->GetRenderer()->GetVID(), vertexList, evento.c, "Trapezoide");
                        m_pManager->InsertarWidget(w);
                        ConsumirEvento();
                        m_pManager->LanzarEventoCreacion(w);
                }
        } else if (evento.ButtonDown(m_ButtonMask)) {
                if (m_Estado != WBS_Ninguno) {
                        return;
                }

                //si ya hay uno pues nada

                if (m_NumMaxTrapezoides > 0) {
                        int numSemillas = 0;
                        for (GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); it !=  m_pManager->GetListaWidgets().end(); ++it) {
                                if ( (*it)->GetGID() == m_GID) {
                                        numSemillas++;
                                        if (numSemillas >= m_NumMaxTrapezoides) {
                                                return;
                                        }
                                }
                        }
                }

                m_MouseDown = true;

                m_Start= evento.iP;
                m_Stop = m_Start;

                GNC::GCS::Vector a = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[0]);
                GNC::GCS::Vector b = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[1]);
                m_Vertices[0] = m_Start;
                m_Vertices[1] = a;
                m_Vertices[2] = m_Stop;
                m_Vertices[3] = b;

                m_pManager->Modificado();
                m_Estado = WBS_Creando;
                ConsumirEvento();
        } else if (evento.Dragging() && m_MouseDown) {
                if (m_Estado != WBS_Creando) {
                        return;
                }

                m_Stop= evento.iP;

                GNC::GCS::Vector a = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[0]);
                GNC::GCS::Vector b = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[1]);
                m_Vertices[0] = m_Start;
                m_Vertices[1] = a;
                m_Vertices[2] = m_Stop;
                m_Vertices[3] = b;

                m_pManager->Modificado();
                ConsumirEvento();
        } else if( evento.Moving() ) {
                GTRACE("Moviendose");
                m_NodoMoviendose= evento.iP;
                m_Estado = WBS_Ninguno;
        }
}

void GNC::GCS::Widgets::WTrapezoideBuilder::OnKeyEvents(TEventoTeclado&)
{

}

void GNC::GCS::Widgets::WTrapezoideBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

        if (m_Estado == WBS_Creando) {
                glLineWidth(2.0f);
                //glEnable(GL_LINE_STIPPLE);
                //glLineStipple(3, 0xAAAA);
                glColor4f(0.0f, 0.5f, 0.0f, 0.75f);
                glBegin(GL_LINE_LOOP);
                glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
                glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
                glVertex2d(m_Vertices[2].x, m_Vertices[2].y);
                glVertex2d(m_Vertices[3].x, m_Vertices[3].y);
                glEnd();

                //glDisable(GL_LINE_STIPPLE);
        } else {

        }
}


GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WTrapezoideBuilder::GetCursor()
{
        return GNC::GCS::Widgets::CUR_CREAR_RECTANGULO;
}

//endregion
