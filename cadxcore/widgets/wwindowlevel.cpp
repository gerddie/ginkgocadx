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


#include <cmath>

#include <api/globals.h>
#include "wwindowlevel.h"
#include <main/managers/widgetsmanager.h>
#include <api/westilo.h>

#include <vtkgl.h>


//region "Implementacion de WWindowLevelBuilder"

GNC::GCS::Widgets::WWindowLevelBuilder::WWindowLevelBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
        m_pObservador = NULL;
        m_MouseDown = false;
        m_Changed = false;
}

GNC::GCS::Widgets::WWindowLevelBuilder::~WWindowLevelBuilder()
{
}

void GNC::GCS::Widgets::WWindowLevelBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
        if (!m_pManager) {
                return;
        }
        if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
                m_MouseDown = false;

                TVector stop(evento.sX, evento.sY);
                if (m_pObservador != NULL) {
                        m_pObservador->OnAjuste(m_Start, stop);
                }
                if (m_pObservador != NULL) {
                        m_pObservador->OnEnd();
                }
                if (m_Changed) {
                        ConsumirEvento();
                }
                //m_pManager->Modificado();
        } else if (evento.ButtonDown(m_ButtonMask)) {

                m_MouseDown = true;
                m_Changed = false;
                if (m_pObservador != NULL) {
                        m_pObservador->OnStart();
                }
                m_Start.Asignar(evento.sX, evento.sY);

                //m_pManager->Modificado();
                ConsumirEvento();
        } else if (evento.Dragging() && m_MouseDown) {
                TVector stop(evento.sX, evento.sY);
                if (m_Start.DistanciaEuclideaCuadrado(stop) > TVector::EpsilonDeReales()) {
                        m_Changed = true;
                }
                if (m_pObservador != NULL) {
                        m_pObservador->OnAjuste(m_Start, stop);
                }

                //m_pManager->Modificado();
                ConsumirEvento();
        } else if( evento.Moving() ) {
                GTRACE("Moviendose");
                m_NodoMoviendose.Asignar(evento.sX, evento.sY);
        }

        GTRACE("GNC::GCS::Widgets::WWindowLevelBuilder::OnMouseEvents(wxMouseEvent&)");
}

void GNC::GCS::Widgets::WWindowLevelBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
        GTRACE("GNC::GCS::Widgets::WWindowLevelBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WWindowLevelBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WWindowLevelBuilder::GetCursor()
{
        return GNC::GCS::Widgets::CUR_WINDOW_LEVEL;
}

//endregion
