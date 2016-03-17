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


#include <sstream>
#include <cmath>
#include <cstring>
#include <cairo/cairo.h>

//#define _GINKGO_TRACE
#include <api/globals.h>
#include <api/helpers/helpertexto.h>
#include <api/iwidgetsmanager.h>
#include <api/controllers/icontroladorlog.h>
#include <api/ievento.h>
#include <api/icontexto.h>
#include <api/math/geometry3d.h>
#include <api/westilo.h>

#include <eventos/modificacionimagen.h>

#include <main/entorno.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/configurationcontroller.h>

#include "wstaticrule.h"

#include <vtkgl.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkPointData.h>
#include <vtk/vtkginkgoimageviewer.h>


//region "Constructor y destructor"

#define ANNOTATOR_STATIC_RULE_COLOR 0.0f, 1.0f, 0.0f, 0.5f
#define RULE_MARGIN 5
#define SIZE_BIG_MARKS 5
#define SIZE_SMALL_MARKS 2

GNC::GCS::Widgets::WStaticRule::WStaticRule(IWidgetsManager* pManager, long vid, const char* nombre, long gid) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid)
{
        GNC::GCS::ConfigurationController::Instance()->readBoolUser("/GinkgoCore/Tools/CornerAnotations", "IsShown", m_Oculto, true);
        m_TopLevel = true;
        m_ReservaRecursos = false;
        m_color = GNC::GCS::GLHelper::TColor(ANNOTATOR_STATIC_RULE_COLOR);

        GNC::GCS::Events::EventoModificacionImagen evt2(m_pManager->GetVista());
        GNC::GCS::IEventsController::Instance()->Registrar(this, evt2);
}

GNC::GCS::Widgets::WStaticRule::~WStaticRule()
{}

//endregion


//region "Interfaz generica"

void GNC::GCS::Widgets::WStaticRule::OnMouseEvents(GNC::GCS::Events::EventoRaton& /*evento*/)
{

}

void GNC::GCS::Widgets::WStaticRule::OnKeyEvents(GNC::GCS::Events::EventoTeclado& /*evento*/)
{
}

bool GNC::GCS::Widgets::WStaticRule::HitTest(float , float , const GNC::GCS::Vector&)
{
        return false;
}

bool GNC::GCS::Widgets::WStaticRule::HitTest(GNC::GCS::Vector* , int )
{
        return false;
}
void GNC::GCS::Widgets::WStaticRule::Render(GNC::GCS::Contexto3D* c)
{
        if(m_Oculto && c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
                return;
        }

        vtkGinkgoImageViewer* pImageViewer = c->GetRenderer()->m_pImageViewer;

        double viewport[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // { x, y, ancho, alto }. Convenio de coordenadas: {x, y} == {bottom, left}, {ancho, alto} == {top, right}
        glGetDoublev(GL_VIEWPORT, viewport);

        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho( 0, viewport[2] , viewport[3] , 0, -1, 1 );

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();


        {
                GNC::GCS::Vector3D screenToImage(c->RelacionImagenPantalla().x,c->RelacionImagenPantalla().y,0);
                double spacing[3];
                pImageViewer->GetSpacing(spacing);
                GNC::GCS::Vector3D spacingVector(spacing[0], spacing[1], spacing[2]);
                GNC::GCS::Vector3D screenToWorld = spacingVector * screenToImage;

                GNC::GCS::Vector ruleMeasure(100.0, 100.0);
                GNC::GCS::Vector ruleFragments(10.0, 10.0);

                // Dibujado de la regla
                glLineWidth(m_LineWidthProp);

                const double ruleMeasureScreen[2] = {ruleMeasure.x / screenToWorld.x, ruleMeasure.y / screenToWorld.y};
                const double xLimitsRule[2] = {(viewport[2]/2)-(0.5*ruleMeasureScreen[0]), (viewport[2]/2)+(0.5*ruleMeasureScreen[0])};
                const double yLimitsRule[2] = {(viewport[3]/2)-(0.5*ruleMeasureScreen[1]), (viewport[3]/2)+(0.5*ruleMeasureScreen[1])};

                glColor4f(m_color.RF(), m_color.GF(), m_color.BF(), m_color.AF());
                glBegin(GL_LINES);
                glVertex2d(xLimitsRule[0], viewport[3] - RULE_MARGIN);
                glVertex2d(xLimitsRule[1], viewport[3] - RULE_MARGIN);
                glEnd();

                //marks
                for (int i = 0; i <= ruleFragments.x; ++i) {
                        glBegin(GL_LINES);
                        glVertex2d(xLimitsRule[0] + (i*ruleMeasureScreen[0]/ruleFragments.x), viewport[3] - RULE_MARGIN);
                        if (i%5==0) {
                                glVertex2d(xLimitsRule[0] + (i*ruleMeasureScreen[0]/ruleFragments.x), viewport[3] - RULE_MARGIN - SIZE_BIG_MARKS);
                        } else {
                                glVertex2d(xLimitsRule[0] + (i*ruleMeasureScreen[0]/ruleFragments.x), viewport[3] - RULE_MARGIN - SIZE_SMALL_MARKS);
                        }
                        glEnd();
                }

                //yRule
                glBegin(GL_LINES);
                glVertex2d(RULE_MARGIN,yLimitsRule[0]);
                glVertex2d(RULE_MARGIN,yLimitsRule[1]);
                glEnd();

                //marks
                for (int i = 0; i <=  ruleFragments.y; ++i) {
                        glBegin(GL_LINES);
                        glVertex2d(RULE_MARGIN, yLimitsRule[0] + (i*ruleMeasureScreen[1]/ruleFragments.y));
                        if (i%5==0) {
                                glVertex2d(RULE_MARGIN + SIZE_BIG_MARKS, yLimitsRule[0] + (i*ruleMeasureScreen[1]/ruleFragments.y) );
                        } else {
                                glVertex2d(RULE_MARGIN + SIZE_SMALL_MARKS, yLimitsRule[0] + (i*ruleMeasureScreen[1]/ruleFragments.y) );
                        }
                        glEnd();
                }
        }

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_TEXTURE);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);

        glPopAttrib();
}

void GNC::GCS::Widgets::WStaticRule::Modificar (bool /*modificada*/)
{

}

void GNC::GCS::Widgets::WStaticRule::Seleccionar(bool )
{
}

void GNC::GCS::Widgets::WStaticRule::Iluminar(bool )
{
}

void GNC::GCS::Widgets::WStaticRule::Ocultar(bool oculto)
{
        if(oculto != m_Oculto) {
                m_Oculto = oculto;
        }
}

//endregion


//region "Interfaz especifica"

//endregion


//region "Estado interno"

//region Interfaz de eventos ginkgo

void GNC::GCS::Widgets::WStaticRule::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
        if (evt == NULL) {
                std::cerr << "Error: Evento nulo" << std::endl;
                return;
        }
        switch (evt->GetCodigoEvento()) {

        case ginkgoEVT_Core_ModificacionImagen: {
                /*		GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
                		if (pEvt != NULL && pEvt->GetTipo() == GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas) {
                			Modificar(true);
                			m_Estado->InvalidarTodas();
                		}*/
        }
        break;

        }
}

//endregion

void GNC::GCS::Widgets::WStaticRule::OffscreenRender(GNC::GCS::Contexto3D* /*c*/)
{

}

