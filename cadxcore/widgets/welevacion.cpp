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
#include <api/iwidgetsmanager.h>
#include <api/iwidgetsrenderer.h>
#include <vtk/vtkginkgoimageviewer.h>
#include "welevacion.h"
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <api/ievento.h>
#include <vtkgl.h>
#include <cmath>

#include <vtkImageData.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkImageActor.h>

//region "Implementacion de WElevacionBuilder"

GNC::GCS::Widgets::WElevacionBuilder::WElevacionBuilder(GNC::GCS::IWidgetsManager* pManager, GNC::GCS::Widgets::Elevacion::ListaRepresentaciones* pRepresentaciones, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
        m_MouseDown = false;
        m_Estado = WBS_Ninguno;
        m_pRepresentaciones = pRepresentaciones;
        m_ListaCreada = false;
        m_RadioCacheado = 0.0f;
        m_LIndex = 0;
        m_pRepActiva = nullptr;
        m_Dentro = true;
        m_RepresentacionActiva = 0;
        m_pObservador = nullptr;
}

GNC::GCS::Widgets::WElevacionBuilder::~WElevacionBuilder()
{
        for (TipoIteradorListaNodos it = m_Nodos.begin(); it != m_Nodos.end(); ++it) {
                delete *it;
        }
        m_Nodos.clear();
        DestroyDisplayList();
}

void GNC::GCS::Widgets::WElevacionBuilder::SetRepresentacionActiva(int idRepresentacion)
{
        m_RepresentacionActiva = idRepresentacion;
        GNC::GCS::Widgets::Elevacion::ListaRepresentaciones::iterator it = m_pRepresentaciones->begin();
        for (int i = 0; i < idRepresentacion; i++, ++it);
        m_pRepActiva = *it;
}

void GNC::GCS::Widgets::WElevacionBuilder::SetObservador(GNC::GCS::Widgets::Elevacion::IObservadorElevacion* pObservador)
{
        m_pObservador=pObservador;
}

void GNC::GCS::Widgets::WElevacionBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
        if (!m_pManager) {
                return;
        }

        GNC::GCS::Widgets::Elevacion::ListaRepresentaciones::iterator it = m_pRepresentaciones->begin();
        for (int i = 0; i < m_RepresentacionActiva; i++, ++it);
        m_pRepActiva = *it;

        if (evento.Entering()) {
                m_Dentro = true;
                m_pManager->Modificado();
        } else if (evento.Leaving()) {
                m_Dentro = false;
                m_pManager->Modificado();
        }

        if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
                m_MouseDown = false;
                if (m_Estado != WBS_Creando) {
                        m_Estado = WBS_Ninguno;
                        return;
                }
                m_Estado = WBS_Ninguno;

                m_NodoMoviendose= evento.iP;
                if(m_pObservador!=NULL) {
                        m_pObservador->OnTerminarInsertar();
                }
                for (TipoIteradorListaNodos it = m_Nodos.begin(); it != m_Nodos.end(); ++it) {
                        delete *it;
                }
                m_Nodos.clear();
                evento.Skip(false);
                m_pManager->Modificado();
                m_Estado = WBS_Ninguno;
                GTRACE("Levanta el boton pasamos a ninguno");
        } else if (evento.ButtonDown(m_ButtonMask)) {
                if (m_Estado != WBS_Ninguno) {
                        return;
                }
                m_MouseDown = true;

                for (TipoIteradorListaNodos it = m_Nodos.begin(); it != m_Nodos.end(); ++it) {
                        delete *it;
                }
                m_Nodos.clear();

                GNC::GCS::Nodo* nodo= new GNC::GCS::Nodo();
                nodo->Asignar(evento.iP.x, evento.iP.y);
                m_Nodos.push_back(nodo);
                //aviso que han soltado
                m_pManager->Modificado();
                GTRACE("Hace click pasamos a creando");
                if(m_pObservador!=NULL) {
                        m_pObservador->OnPuntoInsertado(evento.iP.x, evento.iP.y);
                }
                m_Estado = WBS_Creando;
                evento.Skip(false);
        } else if (evento.Dragging() && m_MouseDown) {
                if (m_Estado != WBS_Creando) {
                        return;
                }
                //mando punto para que suban la valoracion!!!!!!
                GTRACE("esta haciendo dragging con el boton pulsado..........");
                GNC::GCS::Nodo* nodo= new GNC::GCS::Nodo();
                nodo->Asignar(evento.iP.x, evento.iP.y);
                m_Nodos.push_back(nodo);
                if(m_pObservador!=NULL) {
                        m_pObservador->OnPuntoInsertado(evento.iP.x, evento.iP.y);
                }
                m_pManager->Modificado();
                evento.Skip(false);
        } else if( evento.Moving() ) {
                GTRACE("Moviendose "<< evento.wX <<","<<evento.wY);
                m_NodoMoviendose= evento.iP;
                m_Estado = WBS_Ninguno;
                m_pManager->Modificado();
                evento.Skip(false);
        }

        GTRACE("GNC::GCS::Widgets::WElevacionBuilder::OnMouseEvents(wxMouseEvent&)");
}

void GNC::GCS::Widgets::WElevacionBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
        GTRACE("GNC::GCS::Widgets::WElevacionBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WElevacionBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{
        float radioCirculo;

        radioCirculo = m_pRepActiva->m_RadioCircunferencia;


        if (m_Estado == WBS_Creando) {
                if (radioCirculo != m_RadioCacheado) {
                        BuildDisplayList();
                }
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                for(TipoIteradorListaNodos it=m_Nodos.begin(); it!=m_Nodos.end(); ++it) {
                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        glTranslatef((*it)->x, (*it)->y, 0.0f);
                        glColor4f(m_pRepActiva->colorCircunferencia.r, m_pRepActiva->colorCircunferencia.g, m_pRepActiva->colorCircunferencia.b, m_pRepActiva->colorCircunferencia.a);
                        glCallList(m_LIndex);
                        glPopMatrix();
                }
        } else {
                if (m_Dentro) {
                        if (radioCirculo != m_RadioCacheado) {
                                BuildDisplayList();
                        }
                        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        glTranslatef(m_NodoMoviendose.x, m_NodoMoviendose.y, 0.0f);
                        glColor4f(m_pRepActiva->colorCircunferencia.r, m_pRepActiva->colorCircunferencia.g, m_pRepActiva->colorCircunferencia.b, m_pRepActiva->colorCircunferencia.a);
                        glCallList(m_LIndex);
                        glPopMatrix();
                }
        }
}

//endregion

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WElevacionBuilder::GetCursor()
{
        return GNC::GCS::Widgets::CUR_TRANSPARENTE;
}


//region "Helpers"
void GNC::GCS::Widgets::WElevacionBuilder::BuildDisplayList()
{

        if (m_pRepActiva == NULL ) {
                return;
        }
        if (m_ListaCreada) {
                glDeleteLists(m_LIndex, 1);
        }

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        m_LIndex = glGenLists(1);
        m_ListaCreada = true;

        glNewList(m_LIndex, GL_COMPILE);

        glBegin(GL_TRIANGLE_FAN);
        float step = M_2PI / 30;
        m_RadioCacheado = m_pRepActiva->m_RadioCircunferencia;

        for ( float angulo = 0; angulo < M_2PI; angulo += step) {
                glVertex3f(m_RadioCacheado * cos (angulo) , m_RadioCacheado * sin (angulo), 0.0f);

        }
        glEnd();

        glEndList();

        glPopMatrix();

}

void GNC::GCS::Widgets::WElevacionBuilder::DestroyDisplayList()
{
        if (m_ListaCreada) {
                glDeleteLists(m_LIndex, 1);
                m_ListaCreada = false;
        }
}

