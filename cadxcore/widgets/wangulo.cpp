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

#include <ostream>
#include <sstream>
#include <cmath>

#include <api/globals.h>
#include <api/math/geometry3d.h>
#include <api/internationalization/internationalization.h>
#include "wangulo.h"
#include "wtextbox.h"
#include <api/ievento.h>
#include <api/icontexto.h>
#include <api/iwidgetsrenderer.h>
#include <eventos/modificacionimagen.h>
#include "main/managers/widgetsmanager.h"
#include <vtk/gl.h>
#include <cairo/cairo.h>
#include "openglhelper.h"
#include <wx/xml/xml.h>

//region "Implementacion de WAnguloBuilder"

GNC::GCS::Widgets::WAnguloBuilder::WAnguloBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask) :
        GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask),
        m_Changed(false),
        m_Estado(WBS_Ninguno),
        m_Modo(MA_Normal),
        m_NodoActual(0),
        m_pAnguloTemp(NULL)
{
}

GNC::GCS::Widgets::WAnguloBuilder::~WAnguloBuilder()
{
        if (m_pAnguloTemp != NULL) {
                delete m_pAnguloTemp;
                m_pAnguloTemp = NULL;
                m_NodoActual = 0;
        }
}

void GNC::GCS::Widgets::WAnguloBuilder::SetModo(ModoAngulo modo)
{
        if (m_pAnguloTemp == NULL) {
                delete m_pAnguloTemp;
                m_pAnguloTemp = NULL;
                m_NodoActual = 0;
        }
        m_Modo = modo;
}

void GNC::GCS::Widgets::WAnguloBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
        if (!m_pManager) {
                return;
        }
        GTRACE("GNC::GCS::Widgets::WAnguloBuilder::OnMouseEvents(wxMouseEvent&)");
        if (m_Estado == WBS_Creando && evento.ButtonUp(m_ButtonMask)) {
                if (m_pAnguloTemp != NULL) {
                        if (m_Modo == MA_Normal) { // Angulo normal
                                if (m_NodoActual == 1) {
                                        m_pAnguloTemp->m_Nodos[1] = evento.iP;
                                        if ( (m_pAnguloTemp->m_Nodos[1] - m_pAnguloTemp->m_Nodos[0]).Norma2Cuadrado() < GNC::GCS::Vector::EpsilonDeReales() ) {
                                                m_NodoActual = 0;
                                                delete m_pAnguloTemp;
                                                m_Estado = WBS_Ninguno;
                                                m_pAnguloTemp = NULL;
                                        } else {
                                                m_pAnguloTemp->m_Nodos[2] = evento.iP;
                                                m_NodoActual = 2;
                                                m_pAnguloTemp->Recalcular(evento.c->GetRenderer(), true);
                                                ConsumirEvento();
                                        }
                                } else {
                                        m_pAnguloTemp->m_Nodos[2] = evento.iP;
                                        if ( (m_pAnguloTemp->m_Nodos[2] - m_pAnguloTemp->m_Nodos[1]).Norma2Cuadrado() < GNC::GCS::Vector::EpsilonDeReales()) {
                                                m_NodoActual = 0;
                                                delete m_pAnguloTemp;
                                                m_Estado = WBS_Ninguno;
                                                m_pAnguloTemp = NULL;
                                        } else {
                                                m_pAnguloTemp->Recalcular(evento.c->GetRenderer(), true);
                                                m_pManager->InsertarWidget(m_pAnguloTemp);
                                                LanzarEventoCreacion(m_pAnguloTemp);
                                                m_pAnguloTemp = NULL;
                                                m_Estado = WBS_Ninguno;
                                                m_NodoActual = 0;
                                                ConsumirEvento();
                                        }
                                }
                        } else { // Angulo de Cobb
                                m_pAnguloTemp->m_Nodos[m_NodoActual] = evento.iP;
                                m_pAnguloTemp->Recalcular(evento.c->GetRenderer(), true);
                                if (m_NodoActual == 1) {
                                        if ( (m_pAnguloTemp->m_Nodos[1] - m_pAnguloTemp->m_Nodos[0]).Norma2Cuadrado() < GNC::GCS::Vector::EpsilonDeReales() ) {
                                                m_NodoActual = 0;
                                                delete m_pAnguloTemp;
                                                m_Estado = WBS_Ninguno;
                                                m_pAnguloTemp = NULL;
                                        } else {
                                                m_NodoActual = 2;
                                                ConsumirEvento();
                                        }
                                } else {
                                        if ( (m_pAnguloTemp->m_Nodos[3] - m_pAnguloTemp->m_Nodos[2]).Norma2Cuadrado() < GNC::GCS::Vector::EpsilonDeReales()) {
                                                m_NodoActual = 0;
                                                delete m_pAnguloTemp;
                                                m_Estado = WBS_Ninguno;
                                                m_pAnguloTemp = NULL;
                                        } else {
                                                m_pManager->InsertarWidget(m_pAnguloTemp);

                                                m_pManager->LanzarEventoCreacion(m_pAnguloTemp);
                                                m_pAnguloTemp = NULL;
                                                m_Estado = WBS_Ninguno;
                                                m_NodoActual = 0;
                                                ConsumirEvento();
                                        }
                                }
                        }
                        m_pManager->Modificado();
                }
        } else if (evento.ButtonDown(m_ButtonMask)) {
                if (m_Estado == WBS_Ninguno) {
                        m_Estado = WBS_Creando;
                        if (m_pAnguloTemp == NULL) {
                                if (m_pAnguloTemp != NULL) {
                                        delete m_pAnguloTemp;
                                }
                                m_pAnguloTemp = new GNC::GCS::Widgets::WAngulo(m_pManager, evento.c->GetRenderer()->GetVID(), m_Modo, "Angle");
                                if (m_Modo == MA_Cobb) {
                                        m_pAnguloTemp->SetCompleted(false);
                                }
                                m_pAnguloTemp->m_Nodos[0] = m_pAnguloTemp->m_Nodos[1] = m_pAnguloTemp->m_Nodos[2] = evento.iP;
                                m_pAnguloTemp->Recalcular(evento.c->GetRenderer(), true);
                                m_NodoActual = 1;
                                ConsumirEvento();
                        }
                } else {
                        if (m_pAnguloTemp != NULL) {
                                if (m_Modo == MA_Cobb) {  // Angulo de Cobb
                                        if (m_NodoActual == 2) {
                                                m_pAnguloTemp->SetCompleted(true);
                                                m_pAnguloTemp->m_Nodos[2] = evento.iP;
                                                m_NodoActual = 3;
                                                m_pAnguloTemp->m_Nodos[3] = evento.iP;
                                        }
                                }
                                ConsumirEvento();
                        } else {
                                m_Estado = WBS_Ninguno;
                        }
                }
                m_pManager->Modificado();
        } else if (evento.Dragging() && m_Estado == WBS_Creando) {
                if (m_pAnguloTemp != NULL) {
                        m_pAnguloTemp->m_Nodos[m_NodoActual] = evento.iP;
                        m_pAnguloTemp->Recalcular(evento.c->GetRenderer(), true);
                        m_pManager->Modificado();
                        ConsumirEvento();
                }
        } else if (evento.Moving() && m_Estado == WBS_Creando) {
                if (m_pAnguloTemp != NULL) {
                        m_pAnguloTemp->m_Nodos[m_NodoActual] = evento.iP;
                        m_pAnguloTemp->Recalcular(evento.c->GetRenderer(), true);
                        m_pManager->Modificado();
                        ConsumirEvento();
                }
        }
}

void GNC::GCS::Widgets::WAnguloBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
        GTRACE("GNC::GCS::Widgets::WAnguloBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WAnguloBuilder::Render(GNC::GCS::Contexto3D* c)
{

        if (m_pAnguloTemp == NULL) {
                return;
        }
        if (c->GetRenderer()->GetVID() == m_pAnguloTemp->GetVID()) {

                glLineWidth(m_LineWidthProp);

                if (m_Modo == MA_Normal) {
                        if (m_NodoActual < 2) {
                                glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
                                glBegin(GL_LINE_STRIP);
                                glVertex2d(m_pAnguloTemp->m_Nodos[0].x, m_pAnguloTemp->m_Nodos[0].y);
                                glVertex2d(m_pAnguloTemp->m_Nodos[1].x, m_pAnguloTemp->m_Nodos[1].y);
                                glEnd();
                        } else {
                                m_pAnguloTemp->Render(c);
                        }

                } else if (m_Modo == MA_Cobb) {
                        if (m_NodoActual < 3) {
                                glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
                                glBegin(GL_LINES);
                                glVertex2d(m_pAnguloTemp->m_Nodos[0].x, m_pAnguloTemp->m_Nodos[0].y);
                                glVertex2d(m_pAnguloTemp->m_Nodos[1].x, m_pAnguloTemp->m_Nodos[1].y);
                                glEnd();
                        } else {
                                m_pAnguloTemp->Render(c);
                        }
                }
        }

}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WAnguloBuilder::GetCursor()
{
        if (m_Modo == MA_Normal) {
                return GNC::GCS::Widgets::CUR_CREAR_ANGULO_NORMAL;
        } else {
                return GNC::GCS::Widgets::CUR_CREAR_ANGULO_COBB;
        }
}

//endregion

//region "Constructor y destructor"

GNC::GCS::Widgets::WAngulo::WAngulo(IWidgetsManager* pManager, long vid, ModoAngulo modo, const char* nombre) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre,0,TID_USER_ANNOTATION_WIDGET), GNC::GCS::Widgets::IWidgetSerializable()
{
        m_Modo = modo;

        m_MouseDown = false;
        m_Oculto = false;

        m_pTextBox = new GNC::GCS::Widgets::WTextBox(m_pManager, m_VID, m_Nodos[1], GNC::GCS::Widgets::WTextBox::TNM_Dynamic, "ruleBox", "");
        m_pTextBox->SetTexto("0");
        m_pManager->InsertarWidget(m_pTextBox);

        Recalcular(NULL, false);
        GNC::GCS::Events::EventoModificacionImagen evt1(m_pManager->GetVista());
}

GNC::GCS::Widgets::WAngulo::~WAngulo()
{
        m_pManager->EliminarWidget(m_pTextBox, false);
        LanzarEventoDestruccion();
}

//endregion

//region "Serializado y desserializado"
GNC::GCS::Widgets::WAngulo::WAngulo(IWidgetsManager* pManager, long vid, wxXmlNode* nodo):GNC::GCS::Widgets::IWidget(pManager, vid, "Ángulo"), GNC::GCS::Widgets::IWidgetSerializable(nodo)
{
        if(nodo->GetName() != wxT("angle_widget")) {
                std::cerr<< "Ha ocurrido un error al desserializar el widget regla"<<std::endl;
        }

        if(nodo->GetAttribute(wxT("mode"),wxT("normal")) == wxT("normal")) {
                m_Modo = MA_Normal;
        } else {
                m_Modo = MA_Cobb;
        }

        wxXmlNode *child = nodo->GetChildren();
        while (child) {
                if(child->GetName() == wxT("node0")) {
                        m_Nodos[0] = GNC::GCS::Nodo::Deserializar(child);
                } else if(child->GetName() == wxT("node1")) {
                        m_Nodos[1] = GNC::GCS::Nodo::Deserializar(child);
                } else if(child->GetName() == wxT("node2")) {
                        m_Nodos[2] = GNC::GCS::Nodo::Deserializar(child);
                } else if(child->GetName() == wxT("node3")) {
                        m_Nodos[3] = GNC::GCS::Nodo::Deserializar(child);
                }
                child = child->GetNext();
        }

        m_MouseDown = false;

        m_pTextBox = new GNC::GCS::Widgets::WTextBox(m_pManager, m_VID, m_Nodos[1], GNC::GCS::Widgets::WTextBox::TNM_Dynamic, "ruleBox", "");
        m_pTextBox->SetTexto("0");
        m_pManager->InsertarWidget(m_pTextBox);

        Recalcular(NULL, false);

        GNC::GCS::Events::EventoModificacionImagen evt1(m_pManager->GetVista());
}

void GNC::GCS::Widgets::WAngulo::SetCompleted(bool completed)
{
        m_pTextBox->Ocultar(!completed);
}

wxXmlNode* GNC::GCS::Widgets::WAngulo::Serializar(const std::string& nombreMedico)
{
        wxXmlNode* resultado = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("angle_widget"));
        if(m_Modo == MA_Normal) {
                resultado->AddAttribute(wxT("mode"),wxT("normal"));
        } else {
                resultado->AddAttribute(wxT("mode"),wxT("cobb"));
        }
        //almacenamos los dos nodos
        wxXmlNode* nodo = m_Nodos[0].Serializar();
        nodo->SetName(wxT("node0"));
        resultado->AddChild(nodo);

        nodo = m_Nodos[1].Serializar();
        nodo->SetName(wxT("node1"));
        resultado->AddChild(nodo);

        nodo = m_Nodos[2].Serializar();
        nodo->SetName(wxT("node2"));
        resultado->AddChild(nodo);

        nodo = m_Nodos[3].Serializar();
        nodo->SetName(wxT("node3"));
        resultado->AddChild(nodo);

        //metadatos
        SerializarMetadatos(resultado,nombreMedico);
        return resultado;
}
//endregion

void GNC::GCS::Widgets::WAngulo::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
        if (EstaOculto()) {
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
                        m_Nodos[0] += delta;
                        m_Nodos[1] += delta;
                        m_Nodos[2] += delta;
                        if (m_Modo == MA_Cobb) {
                                m_Nodos[3] += delta;
                        }
                } else if (m_Nodos[0].EstaIluminado()) {
                        m_Nodos[0] += delta;
                } else if (m_Nodos[1].EstaIluminado()) {
                        m_Nodos[1] += delta;
                } else if (m_Nodos[2].EstaIluminado()) {
                        m_Nodos[2] += delta;
                } else if (m_Modo == MA_Cobb && m_Nodos[3].EstaIluminado()) {
                        m_Nodos[3] += delta;
                }

                Recalcular(evento.c->GetRenderer(), false);
                LanzarEventoModificacion();
                ConsumirEvento();

                return;
        }
        //------------------------------------------------------
        // LeftDown
        else if (evento.LeftDown()) {

                if (EventoConsumido() && !evento.m_controlDown) {
                        SeleccionarNodo(m_Nodos[0], false);
                        SeleccionarNodo(m_Nodos[1], false);
                        SeleccionarNodo(m_Nodos[2], false);
                        SeleccionarNodo(m_Nodos[3], false);
                        Seleccionar(false);
                        return;
                }

                bool dentro = false;

                m_PosCursor = evento.iP;

                const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

                if (!dentro && m_Nodos[0].Hits(evento.iP, factor) ) {
                        SeleccionarNodo(m_Nodos[0], true);
                        Seleccionar(false);
                        dentro = true;
                } else {
                        SeleccionarNodo(m_Nodos[0], false);
                }

                if (!dentro && m_Nodos[1].Hits(evento.iP, factor) ) {
                        SeleccionarNodo(m_Nodos[1], true);
                        Seleccionar(false);
                        dentro = true;
                } else {
                        SeleccionarNodo(m_Nodos[1], false);
                }

                if (!dentro && m_Nodos[2].Hits(evento.iP, factor) ) {
                        SeleccionarNodo(m_Nodos[2], true);
                        Seleccionar(false);
                        dentro = true;
                } else {
                        SeleccionarNodo(m_Nodos[2], false);
                }

                if (m_Modo == MA_Cobb && !dentro &&  m_Nodos[3].Hits(evento.iP, factor) ) {
                        SeleccionarNodo(m_Nodos[3], true);
                        Seleccionar(false);
                        dentro = true;
                } else {
                        SeleccionarNodo(m_Nodos[3], false);
                }

                if (!dentro) {
                        if (HitTest(evento.iP.x, evento.iP.y, factor) ) {
                                if (evento.m_controlDown) { // Inversion de seleccion
                                        InvertirSeleccion();
                                } else {
                                        Seleccionar(true);
                                }
                                dentro = true;
                        } else {
                                if (!evento.m_controlDown) {
                                        Seleccionar(false);
                                } else {
                                        ConsumirEvento();
                                        if (EstaSeleccionado()) {
                                                m_MouseDown = true;
                                        }
                                }
                        }
                }

                if (dentro) {
                        m_MouseDown = true;
                        ConsumirEvento();
                }
                return;
        }
        //------------------------------------------------------
        // LeftUP
        else if (evento.LeftUp()) {
                if (m_MouseDown) {
                        ActualizarTimestampModificacion();
                        m_MouseDown = false;
                        ConsumirEvento();
                }
        }
        //------------------------------------------------------
        // Moving
        else if (evento.Moving()) {
                if (EventoConsumido()) {
                        IluminarNodo(m_Nodos[0], false);
                        IluminarNodo(m_Nodos[1], false);
                        IluminarNodo(m_Nodos[2], false);
                        IluminarNodo(m_Nodos[3], false);
                        Iluminar(false);
                        return;
                }

                bool dentro = false;

                m_PosCursor = evento.iP;

                const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

                if (!dentro && m_Nodos[0].Hits(evento.iP, factor) ) {
                        IluminarNodo(m_Nodos[0], true);
                        Iluminar(false);
                        dentro = true;
                } else {
                        IluminarNodo(m_Nodos[0], false);
                }

                if (!dentro && m_Nodos[1].Hits(evento.iP, factor) ) {
                        IluminarNodo(m_Nodos[1], true);
                        Iluminar(false);
                        dentro = true;
                } else {
                        IluminarNodo(m_Nodos[1], false);
                }

                if (!dentro && m_Nodos[2].Hits(evento.iP, factor) ) {
                        IluminarNodo(m_Nodos[2], true);
                        Iluminar(false);
                        dentro = true;
                } else {
                        IluminarNodo(m_Nodos[2], false);
                }

                if (m_Modo == MA_Cobb && !dentro &&  m_Nodos[3].Hits(evento.iP, factor) ) {
                        IluminarNodo(m_Nodos[3], true);
                        Iluminar(false);
                        dentro = true;
                } else {
                        IluminarNodo(m_Nodos[3], false);
                }

                if (!dentro) {
                        if (HitTest(evento.iP.x, evento.iP.y, factor) ) {
                                Iluminar(true);
                                dentro = true;
                        } else {
                                Iluminar(false);
                        }
                }

                if (dentro) {
                        ConsumirEvento();
                }
                return;
        }
}

void GNC::GCS::Widgets::WAngulo::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
        if (evento.GetKeyCode() == WXK_ESCAPE) {
                Seleccionar(false);
        }
}

bool GNC::GCS::Widgets::WAngulo::HitTest(float x, float y, const GNC::GCS::Vector& rel)
{
        bool hits = false;

        GNC::GCS::Vector dpos(x,y);
        const GNC::GCS::Vector::TReal umbralCuadrado = (GNC::GCS::Vector::TReal)m_Nodos[0].m_Size * (GNC::GCS::Vector::TReal)m_Nodos[0].m_Size;

        if (m_Modo == MA_Normal) {

                if ( m_Nodos[0].DistanciaEuclideaCuadrado(m_Nodos[1]) / 4.0f > dpos.DistanciaEuclideaCuadrado(m_Nodos[0].PuntoMedio(m_Nodos[1]))) { // Hit Test Recta 1

                        GNC::GCS::Vector vdir = m_Nodos[1] - m_Nodos[0];
                        GNC::GCS::Vector orto = (vdir / rel).VectorOrtogonal() * rel;

                        GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(dpos, dpos + orto, m_Nodos[0], m_Nodos[1]);
                        GNC::GCS::Vector diff = (inter - dpos).ValorAbsoluto();
                        if ((diff / rel).Norma2Cuadrado() < umbralCuadrado) {
                                hits = true;
                        }
                }
                if (!hits) { // Hit Test Recta 2
                        if ( m_Nodos[1].DistanciaEuclideaCuadrado(m_Nodos[2]) / 4.0f > dpos.DistanciaEuclideaCuadrado(m_Nodos[1].PuntoMedio(m_Nodos[2]))) { // Recta 1

                                GNC::GCS::Vector vdir = m_Nodos[2] - m_Nodos[1];
                                GNC::GCS::Vector orto = (vdir / rel).VectorOrtogonal() * rel;

                                GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(dpos, dpos + orto, m_Nodos[1], m_Nodos[2]);
                                GNC::GCS::Vector diff = (inter - dpos).ValorAbsoluto();
                                if ((diff / rel).Norma2Cuadrado() < umbralCuadrado) {
                                        hits = true;
                                }
                        }
                }
        } else {
                if ( m_Nodos[0].DistanciaEuclideaCuadrado(m_Nodos[1]) / 4.0f > dpos.DistanciaEuclideaCuadrado(m_Nodos[0].PuntoMedio(m_Nodos[1]))) { // Hit Test Recta 1

                        GNC::GCS::Vector vdir = m_Nodos[1] - m_Nodos[0];
                        GNC::GCS::Vector orto = (vdir / rel).VectorOrtogonal() * rel;

                        GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(dpos, dpos + orto, m_Nodos[0], m_Nodos[1]);
                        GNC::GCS::Vector diff = (inter - dpos).ValorAbsoluto();
                        if ((diff / rel).Norma2Cuadrado() < umbralCuadrado) {
                                hits = true;
                        }
                }
                if (!hits) { // Hit Test Recta 2
                        if ( m_Nodos[2].DistanciaEuclideaCuadrado(m_Nodos[3]) / 4.0f > dpos.DistanciaEuclideaCuadrado(m_Nodos[2].PuntoMedio(m_Nodos[3]))) { // Recta 1

                                GNC::GCS::Vector vdir = m_Nodos[3] - m_Nodos[2];
                                GNC::GCS::Vector orto = (vdir / rel).VectorOrtogonal() * rel;

                                GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(dpos, dpos + orto, m_Nodos[2], m_Nodos[3]);
                                GNC::GCS::Vector diff = (inter - dpos).ValorAbsoluto();
                                if ((diff / rel).Norma2Cuadrado() < umbralCuadrado) {
                                        hits = true;
                                }
                        }
                }
        }

        return hits;
}

bool GNC::GCS::Widgets::WAngulo::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
        if (m_Modo == MA_Normal) {
                return m_Nodos[0].DentroDePoligono2(vertices, numVertices) && m_Nodos[1].DentroDePoligono2(vertices, numVertices) && m_Nodos[2].DentroDePoligono2(vertices, numVertices);

        } else {
                return m_Nodos[0].DentroDePoligono2(vertices, numVertices) && m_Nodos[1].DentroDePoligono2(vertices, numVertices) && m_Nodos[2].DentroDePoligono2(vertices, numVertices) && m_Nodos[3].DentroDePoligono2(vertices, numVertices);
        }
}

void GNC::GCS::Widgets::WAngulo::Render(GNC::GCS::Contexto3D* c)
{
        if (m_Oculto) {
                return;
        }

        if (m_Modo == MA_Normal)
                this->RenderNormal(c);
        else if (m_Modo == MA_Cobb)
                this->RenderCobb(c);
}

void GNC::GCS::Widgets::WAngulo::RenderNormal(GNC::GCS::Contexto3D* c)
{
        GNC::GCS::Vector escala = c->RelacionImagenPantalla();

        TVector v1 = m_Nodos[0] - m_Nodos[1];
        TVector v2 = m_Nodos[2] - m_Nodos[1];

        //norma of unscaled vector
        float n1 = (v1/escala).Norma2();
        float n2 = (v2/escala).Norma2();

        if (n1 == 0.0f || n2 == 0.0f) {
                return;
        }

        // Sombra
        float fsombrax, fsombray;
        if (m_Seleccionado && m_MouseDown) {
                fsombrax = WIDGET_OFFSET_X_SOMBRA_SELECCIONADO * escala.x;
                fsombray = WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO * escala.y;
        } else {
                fsombrax = WIDGET_OFFSET_X_SOMBRA * escala.x;
                fsombray = WIDGET_OFFSET_Y_SOMBRA * escala.y;
        }

        glLineWidth(m_LineWidthProp);

        // Sombra cuerpo
        wColorSombra.Aplicar();
        glBegin(GL_LINE_STRIP);
        glVertex2d(m_Nodos[0].x + fsombrax, m_Nodos[0].y - fsombray);
        glVertex2d(m_Nodos[1].x + fsombrax, m_Nodos[1].y - fsombray);
        glVertex2d(m_Nodos[2].x + fsombrax, m_Nodos[2].y - fsombray);
        glEnd();

        // Sombra puntas de flecha
        float anguloPunta = M_PI_4 / 2.0f;

        TVector puntaA0 = m_Nodos[0] + ((v1/escala).RotacionSobreZ(M_PI+anguloPunta).Normalizar() * escala * 15.0f);
        TVector puntaA1 = m_Nodos[0] + ((v1/escala).RotacionSobreZ(M_PI-anguloPunta).Normalizar() * escala * 15.0f);

        TVector puntaB0 = m_Nodos[2] + ((v2/escala).RotacionSobreZ(M_PI+anguloPunta).Normalizar() * escala * 15.0f);
        TVector puntaB1 = m_Nodos[2] + ((v2/escala).RotacionSobreZ(M_PI-anguloPunta).Normalizar() * escala * 15.0f);

        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaA0.x + fsombrax, puntaA0.y - fsombray);
        glVertex2d(m_Nodos[0].x + fsombrax, m_Nodos[0].y - fsombray);
        glVertex2d(puntaA1.x + fsombrax, puntaA1.y - fsombray);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaB0.x + fsombrax, puntaB0.y - fsombray);
        glVertex2d(m_Nodos[2].x + fsombrax, m_Nodos[2].y - fsombray);
        glVertex2d(puntaB1.x + fsombrax, puntaB1.y - fsombray);
        glEnd();

        // sombra punto origen
        glBegin(GL_POINTS);
        glVertex2d(m_Nodos[1].x + fsombrax, m_Nodos[1].y - fsombray);
        glEnd();

        TMatriz mRot;
        mRot = TMatriz::MatrizRotacion(m_Angulo/60.0f);

        //we have to un scale and scale to print a circle instead an ellipse
        TVector va = ((v1/escala).Normalizado() * std::min(n1, n2))*escala * 0.3f;
        TVector vb = ((v2/escala).Normalizado() * std::min(n1, n2))*escala * 0.3f;

        double normaVa = (va/escala).Norma2();

        TVector puntoTexto;

        // Arco
        wColorAngulo.Aplicar();
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 60; ++i) {
                if ( i == 30) {
                        puntoTexto = m_Nodos[1] + va;
                }
                glVertex2d(m_Nodos[1].x + va.x, m_Nodos[1].y + va.y);
                va /= escala;
                //all va vector has the same norma unscaled
                va = mRot.ProductoMatricial(va).Normalizar() * normaVa;
                va *= escala;
        }
        glVertex2d(m_Nodos[1].x + vb.x, m_Nodos[1].y + vb.y);
        glEnd();

        // Arco interior
        va = ((v1/escala).Normalizado() * std::min(n1, n2))*escala * 0.3f;
        wColorAnguloInterior.Aplicar();

        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(m_Nodos[1].x, m_Nodos[1].y);
        for (int i = 0; i < 60; ++i) {
                glVertex2d(m_Nodos[1].x + va.x, m_Nodos[1].y + va.y);
                va /= escala;
                va = mRot.ProductoMatricial(va).Normalizar() * normaVa;
                va *= escala;
        }
        glVertex2d(m_Nodos[1].x + vb.x, m_Nodos[1].y + vb.y);
        glEnd();

        // Cuerpo
        wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_LINE_STRIP);
        glVertex2d(m_Nodos[0].x, m_Nodos[0].y);
        glVertex2d(m_Nodos[1].x, m_Nodos[1].y);
        glVertex2d(m_Nodos[2].x, m_Nodos[2].y);
        glEnd();


        // Dibujo highlights puntos activos
        for (int i = 0; i < 4; ++i) {
                if (m_Nodos[i].m_Iluminado) {
                        wDibujarElipseDegradado(m_Nodos[i], wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
                }
        }

        // Nodos
        if (m_Seleccionado && m_MouseDown)
                glPointSize(WIDGET_SIZE_PUNTO * WIDGET_INCREMENTO_DETALLE_SELECCIONADO);
        else
                glPointSize(WIDGET_SIZE_PUNTO);


        // Puntas de flecha
        wAplicarColor(m_Iluminado || m_Nodos[0].m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaA0.x, puntaA0.y);
        glVertex2d(m_Nodos[0].x, m_Nodos[0].y);
        glVertex2d(puntaA1.x, puntaA1.y);
        glEnd();

        wAplicarColor(m_Iluminado || m_Nodos[2].m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaB0.x, puntaB0.y);
        glVertex2d(m_Nodos[2].x, m_Nodos[2].y);
        glVertex2d(puntaB1.x, puntaB1.y);
        glEnd();


        wAplicarColor(m_Iluminado || m_Nodos[1].m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_POINTS);
        glVertex2d(m_Nodos[1].x, m_Nodos[1].y);
        glEnd();
}

void GNC::GCS::Widgets::WAngulo::RenderCobb(GNC::GCS::Contexto3D* c)
{
        TVector v1 = m_Nodos[1] - m_Nodos[0];
        TVector v2 = m_Nodos[3] - m_Nodos[2];

        GNC::GCS::Vector escala = c->RelacionImagenPantalla();

        float n1 = (v1/escala).Norma2();
        float n2 = (v2/escala).Norma2();

        if (n1 == 0.0f || n2 == 0.0f) {
                return;
        }

        // Sombra
        float fsombrax, fsombray;
        if (m_Seleccionado && m_MouseDown) {
                fsombrax = WIDGET_OFFSET_X_SOMBRA_SELECCIONADO * escala.x;
                fsombray = WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO * escala.y;
        } else {
                fsombrax = WIDGET_OFFSET_X_SOMBRA * escala.x;
                fsombray = WIDGET_OFFSET_Y_SOMBRA * escala.y;
        }

        glLineWidth(m_LineWidthProp);

        // Sombra
        wColorSombra.Aplicar();

        // Sombra cuerpo
        glBegin(GL_LINES);
        glVertex2d(m_Nodos[0].x + fsombrax, m_Nodos[0].y - fsombray);
        glVertex2d(m_Nodos[1].x + fsombrax, m_Nodos[1].y - fsombray);
        glVertex2d(m_Nodos[2].x + fsombrax, m_Nodos[2].y - fsombray);
        glVertex2d(m_Nodos[3].x + fsombrax, m_Nodos[3].y - fsombray);
        glEnd();

        // Sombra puntos origen 0 y 2
        wColorSombra.Aplicar();
        glPointSize(WIDGET_SIZE_PUNTO);
        glBegin(GL_POINTS);
        glVertex2d(m_Nodos[0].x + fsombrax, m_Nodos[0].y - fsombray);
        glVertex2d(m_Nodos[2].x + fsombrax, m_Nodos[2].y - fsombray);
        glEnd();


        // Sombra puntas de flecha
        float anguloPunta = M_PI_4 / 2.0f;

        TVector puntaA0 = m_Nodos[1] + ((v1/escala).RotacionSobreZ(M_PI+anguloPunta).Normalizar() * escala * 15.0f);
        TVector puntaA1 = m_Nodos[1] + ((v1/escala).RotacionSobreZ(M_PI-anguloPunta).Normalizar() * escala * 15.0f);

        TVector puntaB0 = m_Nodos[3] + ((v2/escala).RotacionSobreZ(M_PI+anguloPunta).Normalizar() * escala * 15.0f);
        TVector puntaB1 = m_Nodos[3] + ((v2/escala).RotacionSobreZ(M_PI-anguloPunta).Normalizar() * escala * 15.0f);

        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaA0.x + fsombrax, puntaA0.y - fsombray);
        glVertex2d(m_Nodos[1].x + fsombrax, m_Nodos[1].y - fsombray);
        glVertex2d(puntaA1.x + fsombrax, puntaA1.y - fsombray);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaB0.x + fsombrax, puntaB0.y - fsombray);
        glVertex2d(m_Nodos[3].x + fsombrax, m_Nodos[3].y - fsombray);
        glVertex2d(puntaB1.x + fsombrax, puntaB1.y - fsombray);
        glEnd();

        TVector puntoTexto;

        TVector interseccion = TVector::InterseccionEntreRectas(m_Nodos[0], m_Nodos[1], m_Nodos[2], m_Nodos[3]);
        // Arco
        {
                TVector p0 = m_Nodos[0].PuntoMedio(m_Nodos[1]);
                TVector p1 = m_Nodos[3].PuntoMedio(m_Nodos[2]);
                puntoTexto = p0.PuntoMedio(p1);

                // Sombra de arco
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(3, 0xAAAA);
                wColorSombra.Aplicar();
                glBegin(GL_LINES);
                glVertex2d(p0.x, p0.y);
                glVertex2d(p1.x, p1.y);
                glEnd();
                glDisable(GL_LINE_STIPPLE);

                glEnable(GL_LINE_STIPPLE);
                glLineStipple(3, 0xAAAA);
                wColorAngulo.Aplicar();
                glBegin(GL_LINES);
                glVertex2d(p0.x, p0.y);
                glVertex2d(p1.x, p1.y);
                glEnd();
                glDisable(GL_LINE_STIPPLE);
        }

        //arco
        TMatriz mRot;

        mRot = TMatriz::MatrizRotacion(m_Angulo/60.0f);


        float normaMenor = std::min(n1, n2);
        TVector va;

        if (normaMenor == n1) {
                va = m_Nodos[0].PuntoMedio(m_Nodos[1]);
        } else {
                va = m_Nodos[3].PuntoMedio(m_Nodos[2]);
        }
        va -= interseccion;

        // Sombra de arco
        wColorSombra.Aplicar();

        GNC::GCS::Vector tva = va;
        double normaVa = (va/escala).Norma2();

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 60; ++i) {
                glVertex2d(interseccion.x + tva.x + fsombrax, interseccion.y + tva.y + fsombray);
                tva /= escala;
                tva = mRot.ProductoMatricial(tva).Normalizar() * normaVa;
                tva *= escala;
        }
        glEnd();

        // Arco
        wColorAngulo.Aplicar();

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 60; ++i) {
                glVertex2d(interseccion.x + va.x, interseccion.y + va.y);
                va /= escala;
                va = mRot.ProductoMatricial(va).Normalizar() * normaVa;
                va *= escala;
        }
        glEnd();


        // Dibujo highlights puntos activos
        for (int i = 0; i < 4; ++i) {
                if (m_Nodos[i].m_Iluminado) {
                        wDibujarElipseDegradado(m_Nodos[i], wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
                }
        }

        // Cuerpo
        wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);

        glBegin(GL_LINES);
        glVertex2d(m_Nodos[0].x, m_Nodos[0].y);
        glVertex2d(m_Nodos[1].x, m_Nodos[1].y);
        glVertex2d(m_Nodos[2].x, m_Nodos[2].y);
        glVertex2d(m_Nodos[3].x, m_Nodos[3].y);
        glEnd();

        // Puntas de flechas
        wAplicarColor(m_Iluminado || m_Nodos[1].m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaA0.x, puntaA0.y);
        glVertex2d(m_Nodos[1].x, m_Nodos[1].y);
        glVertex2d(puntaA1.x, puntaA1.y);
        glEnd();

        wAplicarColor(m_Iluminado || m_Nodos[3].m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_LINE_STRIP);
        glVertex2d(puntaB0.x, puntaB0.y);
        glVertex2d(m_Nodos[3].x, m_Nodos[3].y);
        glVertex2d(puntaB1.x, puntaB1.y);
        glEnd();

        // Nodos
        if (m_Seleccionado && m_MouseDown)
                glPointSize(WIDGET_SIZE_PUNTO * WIDGET_INCREMENTO_DETALLE_SELECCIONADO);
        else
                glPointSize(WIDGET_SIZE_PUNTO);

        wAplicarColor(m_Iluminado || m_Nodos[0].m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_POINTS);
        glVertex2d(m_Nodos[0].x, m_Nodos[0].y);
        glEnd();

        wAplicarColor(m_Iluminado || m_Nodos[2].m_Iluminado, m_MouseDown, m_Seleccionado);
        glBegin(GL_POINTS);
        glVertex2d(m_Nodos[2].x, m_Nodos[2].y);
        glEnd();

        // Interseccion
        interseccion = TVector::InterseccionEntreRectas(m_Nodos[0], m_Nodos[1], m_Nodos[2], m_Nodos[3]);

        if (m_MouseDown && m_Iluminado)
                glPointSize(WIDGET_INCREMENTO_DETALLE_SELECCIONADO * WIDGET_SIZE_PUNTO);
        else
                glPointSize(WIDGET_SIZE_PUNTO);

        // Sombra
        wColorSombra.Aplicar();
        glBegin(GL_POINTS);
        glVertex2d(interseccion.x + fsombrax, interseccion.y - fsombray);
        glEnd();

        // Punto
        wColorAngulo.Aplicar();
        glBegin(GL_POINTS);
        glVertex2d(interseccion.x, interseccion.y);
        glEnd();
}

void GNC::GCS::Widgets::WAngulo::Recalcular(GNC::GCS::IWidgetsRenderer* renderer, bool building)
{
        GNC::GCS::Vector3D worldNodes[4];
        if (renderer != NULL && renderer->m_pImageViewer != NULL) {
                const double tindex = renderer->m_pImageViewer->GetTindex();
                for (int i = 0; i < 4; i++) {
                        double imagePosition[3] = { m_Nodos[i].x, m_Nodos[i].y, tindex };
                        renderer->m_pImageViewer->CoordenadasImagenACoordenadasMundo(imagePosition, worldNodes[i].v);
                }
        } else {
                return;
        }

        m_Modificado = true;
        //std::cout << *this << std::endl;

        if (m_Modo == MA_Normal) {
                GNC::GCS::Vector3D w1 = worldNodes[0] - worldNodes[1];
                GNC::GCS::Vector3D w2 = worldNodes[2] - worldNodes[1];

                GNC::GCS::Vector i1 = m_Nodos[0] - m_Nodos[1];
                GNC::GCS::Vector i2 = m_Nodos[2] - m_Nodos[1];

                m_Angulo = w1.Angulo(w2);
                if (i1.ProductoVectorialZ(i2) > 0.0) {
                        m_Angulo *= -1.0;
                }
        } else {
                GNC::GCS::Vector3D w1 = worldNodes[1] - worldNodes[0];
                GNC::GCS::Vector3D w2 = worldNodes[3] - worldNodes[2];

                GNC::GCS::Vector i1 = m_Nodos[1] - m_Nodos[0];
                GNC::GCS::Vector i2 = m_Nodos[3] - m_Nodos[2];

                float n1 = w1.Norma2Cuadrado();
                float n2 = w2.Norma2Cuadrado();

                if (n1 == 0.0f || n2 == 0.0f) {
                        return;
                }

                float normaMenor = std::min(n1, n2);

                if (normaMenor == n1) {
                        m_Angulo = w1.Angulo(w2);
                        if (i1.ProductoVectorialZ(i2) > 0.0) {
                                m_Angulo *= -1.0;
                        }
                } else {
                        m_Angulo = w2.Angulo(w1);
                        if (i2.ProductoVectorialZ(i1) > 0.0) {
                                m_Angulo *= -1.0;
                        }
                }

                if (m_Angulo > M_PI ) {
                        m_Angulo = m_Angulo - M_PI;
                } else if (m_Angulo < -M_PI ) {
                        m_Angulo = m_Angulo + M_PI;
                }
        }

        std::stringstream os;
        os.setf(ios::floatfield, ios::fixed );
        os.precision(2);
        os.fill('0');

        os << (std::abs(m_Angulo) * 180.0 / M_PI)  << " º";

        std::string medida = os.str();

        m_pTextBox->SetTexto(medida);
        if (building) {
                if (m_Modo == MA_Normal) {
                        m_pTextBox->SetPosition(m_Nodos[1]);
                } else {
                        m_pTextBox->SetPosition(m_Nodos[1].PuntoMedio(m_Nodos[2]));
                }
        }
}

void GNC::GCS::Widgets::WAngulo::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
{
        TMapaPropiedades& primerMapa = listaMapaPropiedades.front();
        if(m_Modo == GNC::GCS::Widgets::MA_Normal) {
                primerMapa[_Std("Type")] = _Std("Normal");
        } else {
                primerMapa[_Std("Type")] = _Std("Cobb");
        }
        {
                std::stringstream os;
                os.setf(ios::floatfield, ios::fixed );
                os.precision(2);
                os.fill('0');

                os << (std::abs(m_Angulo) * 180.0 / M_PI)  << " º";

                std::string medida = os.str();
                primerMapa[_Std("Value")] = medida;
        }
}

//endregion

void GNC::GCS::Widgets::WAngulo::OffscreenRender(GNC::GCS::Contexto3D* c)
{
        const GNC::GCS::Vector& escala = c->RefRelacionMundoPantallaOffscreen();
        float Angulo;

        if (m_Modo == MA_Normal) {
                TVector v1 = m_Nodos[0] - m_Nodos[1];
                TVector v2 = m_Nodos[2] - m_Nodos[1];

                Angulo = v1.Angulo(v2);
        } else {
                TVector v1 = m_Nodos[1] - m_Nodos[0];
                TVector v2 = m_Nodos[3] - m_Nodos[2];

                float n1 = v1.Norma2Cuadrado();
                float n2 = v2.Norma2Cuadrado();

                if (n1 == 0.0f || n2 == 0.0f) {
                        return;
                }

                float normaMenor = std::min(n1, n2);

                if (normaMenor == n1) {
                        Angulo = v1.Angulo(v2);
                } else {
                        Angulo = v2.Angulo(v1);
                }

                if (Angulo > M_PI ) {
                        Angulo = Angulo - M_PI;
                } else if (Angulo < -M_PI ) {
                        Angulo = Angulo + M_PI;
                }
        }

        if (m_Modo == MA_Normal) {

                TVector Nodos[3] = { c->Reproyectar(m_Nodos[0]) * c->factorReescalado, c->Reproyectar(m_Nodos[1]) * c->factorReescalado, c->Reproyectar(m_Nodos[2]) * c->factorReescalado };

                TVector v1 = Nodos[0] - Nodos[1];
                TVector v2 = Nodos[2] - Nodos[1];

                float n1 = v1.Norma2();
                float n2 = v2.Norma2();

                if (n1 == 0.0f || n2 == 0.0f) {
                        return;
                }
                cairo_set_line_width(c->cr, m_LineWidthProp);

                float anguloPunta = M_PI_4 / 2.0f;
                TVector vOrtogonalA = (Nodos[1] - Nodos[0]).VectorOrtonormal() * escala * 15.0f;
                TVector puntaA0 = Nodos[0] + vOrtogonalA.RotacionSobreZ(M_PI_2 + anguloPunta);
                TVector puntaA1 = Nodos[0] + vOrtogonalA.RotacionSobreZ(M_PI_2 - anguloPunta);

                TVector vOrtogonalB = (Nodos[1] - Nodos[2]).VectorOrtonormal() * escala * 15.0f;
                TVector puntaB0 = Nodos[2] + vOrtogonalB.RotacionSobreZ(M_PI_2 + anguloPunta);
                TVector puntaB1 = Nodos[2] + vOrtogonalB.RotacionSobreZ(M_PI_2 - anguloPunta);

                TMatriz mRot;
                mRot = TMatriz::MatrizRotacion(Angulo/60.0f);

                TVector va = v1.Normalizado() * std::min(n1, n2) * 0.3f;
                TVector vb = v2.Normalizado() * std::min(n1, n2) * 0.3f;

                TVector puntoTexto;

                // Arco
                cairo_set_source_rgba(c->cr, wColorAngulo.r, wColorAngulo.g, wColorAngulo.b, wColorAngulo.a);

                cairo_move_to(c->cr, Nodos[1].x + va.x, Nodos[1].y + va.y);
                va = mRot.ProductoMatricial(va);
                for (int i = 1; i < 60; ++i) {
                        if ( i == 30) {
                                puntoTexto = Nodos[1] + va;
                        }
                        cairo_line_to(c->cr, Nodos[1].x + va.x, Nodos[1].y + va.y);
                        va = mRot.ProductoMatricial(va);
                }
                cairo_line_to(c->cr, Nodos[1].x + vb.x, Nodos[1].y + vb.y);
                cairo_stroke(c->cr);

                // Arco interior
                va = v1.Normalizado() * std::min(n1, n2) * 0.3f;
                cairo_set_source_rgba(c->cr, wColorAnguloInterior.r, wColorAnguloInterior.g, wColorAnguloInterior.b, wColorAnguloInterior.a);

                cairo_move_to(c->cr, Nodos[1].x, Nodos[1].y);
                cairo_line_to(c->cr, Nodos[1].x + va.x, Nodos[1].y + va.y);
                for (int i = 0; i < 60; ++i) {
                        cairo_line_to(c->cr, Nodos[1].x + va.x, Nodos[1].y + va.y);
                        va = mRot.ProductoMatricial(va);
                }
                cairo_line_to(c->cr, Nodos[1].x + vb.x, Nodos[1].y + vb.y);
                cairo_close_path(c->cr);
                cairo_fill(c->cr);

                // Cuerpo
                cairo_set_source_rgba(c->cr, wColorNormal.r, wColorNormal.g, wColorNormal.b, wColorNormal.a);

                cairo_move_to(c->cr, Nodos[0].x, Nodos[0].y);
                cairo_line_to(c->cr, Nodos[1].x, Nodos[1].y);
                cairo_line_to(c->cr, Nodos[2].x, Nodos[2].y);
                cairo_stroke(c->cr);

                // Puntas de flecha
                cairo_move_to(c->cr, puntaA0.x, puntaA0.y);
                cairo_line_to(c->cr, Nodos[0].x, Nodos[0].y);
                cairo_line_to(c->cr, puntaA1.x, puntaA1.y);
                cairo_stroke(c->cr);

                cairo_move_to(c->cr, puntaB0.x, puntaB0.y);
                cairo_line_to(c->cr, Nodos[2].x, Nodos[2].y);
                cairo_line_to(c->cr, puntaB1.x, puntaB1.y);
                cairo_stroke(c->cr);

                cairo_arc(c->cr, Nodos[1].x, Nodos[1].y, ( (WIDGET_SIZE_PUNTO / 2.0f) * escala.x), 0, M_2PI);
                cairo_fill(c->cr);
        } else {
                TVector Nodos[4] = { c->Reproyectar(m_Nodos[0]) * c->factorReescalado, c->Reproyectar(m_Nodos[1]) * c->factorReescalado, c->Reproyectar(m_Nodos[2]) * c->factorReescalado, c->Reproyectar(m_Nodos[3]) * c->factorReescalado };
                TVector v1 = Nodos[1] - Nodos[0];
                TVector v2 = Nodos[3] - Nodos[2];

                float n1 = v1.Norma2Cuadrado();
                float n2 = v2.Norma2Cuadrado();

                if (n1 == 0.0f || n2 == 0.0f) {
                        return;
                }

                cairo_set_line_width(c->cr, m_LineWidthProp);

                float anguloPunta = M_PI_4 / 2.0f;
                TVector vOrtogonalA = (Nodos[0] - Nodos[1]).VectorOrtonormal() * escala * 15.0f;
                TVector puntaA0 = Nodos[1] + vOrtogonalA.RotacionSobreZ(M_PI_2 + anguloPunta);
                TVector puntaA1 = Nodos[1] + vOrtogonalA.RotacionSobreZ(M_PI_2 - anguloPunta);

                TVector vOrtogonalB = (Nodos[2] - Nodos[3]).VectorOrtonormal() * escala * 15.0f;
                TVector puntaB0 = Nodos[3] + vOrtogonalB.RotacionSobreZ(M_PI_2 + anguloPunta);
                TVector puntaB1 = Nodos[3] + vOrtogonalB.RotacionSobreZ(M_PI_2 - anguloPunta);

                float normaMenor = std::min(n1, n2);
                TVector va;

                if (normaMenor == n1) {
                        va = Nodos[0].PuntoMedio(Nodos[1]);
                } else {
                        va = Nodos[3].PuntoMedio(Nodos[2]);
                }

                TVector interseccion = TVector::InterseccionEntreRectas(Nodos[0], Nodos[1], Nodos[2], Nodos[3]);
                // Arco
                {
                        TVector p0 = Nodos[0].PuntoMedio(Nodos[1]);
                        TVector p1 = Nodos[3].PuntoMedio(Nodos[2]);

                        wColorAngulo.AplicarCairo(c->cr);
                        cairo_move_to(c->cr, p0.x, p0.y);
                        cairo_line_to(c->cr, p1.x, p1.y);
                        cairo_stroke(c->cr);
                }

                TMatriz mRot;

                mRot = TMatriz::MatrizRotacion(Angulo/60.0f);
                va -= interseccion;

                // Arco
                wColorAngulo.AplicarCairo(c->cr);

                /////////////////////////////
                cairo_move_to(c->cr, interseccion.x + va.x, interseccion.y + va.y);
                for (int i = 1; i <= 60; ++i) {

                        va = mRot.ProductoMatricial(va);
                        cairo_line_to(c->cr, interseccion.x + va.x, interseccion.y + va.y);
                }
                cairo_stroke(c->cr);

                // Cuerpo
                wColorNormal.AplicarCairo(c->cr);
                cairo_move_to(c->cr, Nodos[0].x, Nodos[0].y);
                cairo_line_to(c->cr, Nodos[1].x, Nodos[1].y);
                cairo_stroke(c->cr);

                cairo_move_to(c->cr, Nodos[2].x, Nodos[2].y);
                cairo_line_to(c->cr, Nodos[3].x, Nodos[3].y);
                cairo_stroke(c->cr);

                cairo_move_to(c->cr, puntaA0.x, puntaA0.y);
                cairo_line_to(c->cr, Nodos[1].x, Nodos[1].y);
                cairo_line_to(c->cr, puntaA1.x, puntaA1.y);
                cairo_stroke(c->cr);

                cairo_move_to(c->cr, puntaB0.x, puntaB0.y);
                cairo_line_to(c->cr, Nodos[3].x, Nodos[3].y);
                cairo_line_to(c->cr, puntaB1.x, puntaB1.y);
                cairo_stroke(c->cr);

                cairo_arc(c->cr, Nodos[0].x, Nodos[0].y,  (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
                cairo_fill(c->cr);

                cairo_arc(c->cr, Nodos[2].x, Nodos[2].y,  (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
                cairo_fill(c->cr);

                // Interseccion
                interseccion = TVector::InterseccionEntreRectas(Nodos[0], Nodos[1], Nodos[2], Nodos[3]);
                cairo_arc(c->cr, interseccion.x, interseccion.y,  (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
                cairo_fill(c->cr);

                // Punto
                wColorAngulo.AplicarCairo(c->cr);
                cairo_arc(c->cr, interseccion.x, interseccion.y,  (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
                cairo_fill(c->cr);
        }
}
