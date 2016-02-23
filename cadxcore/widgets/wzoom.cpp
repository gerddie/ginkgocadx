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
#include "wzoom.h"
#include <main/managers/widgetsmanager.h>
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>

//region "Implementacion de WZoomBuilder"
#include <api/icontexto.h>

GNC::GCS::Widgets::WZoomBuilder::WZoomBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	m_Dentro = true;
	m_Start = 0.0;
	m_Changed = false;
	m_Stop = 0.0;
}

GNC::GCS::Widgets::WZoomBuilder::~WZoomBuilder()
{
}

void GNC::GCS::Widgets::WZoomBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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
		
		vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
		vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());
		std::cout << "ending " << std::endl;

		if (is != NULL) {
			is->EndZoom();
		std::cout << "ending zoom " << std::endl;
		}

		m_Estado = WBS_Ninguno;
		if (m_Changed) {
			ConsumirEvento();
		}
	}
	else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}

		m_Changed = false;
		m_MouseDown = true;
		
		m_Start = evento.sY;
		m_Stop = m_Start;		

		m_Estado = WBS_Creando;
		std::cout << "Creando " << evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor()->GetEnabled() << std::endl;
		evento.Skip(false);
	}
	else if (evento.Dragging() && m_MouseDown) {
		if (m_Estado != WBS_Creando) {
			return;
		}
		
		m_Stop = evento.sY;
		
		double delta = (m_Start - m_Stop) / 1.0;
		
		if (delta != 0.0) {
			m_Changed = true;
			int steps = 0;
			if (delta > 0.0)  {
				steps = (int)std::floor(delta);
			}
			else {
				steps = (int)std::ceil(delta);
			}
			if (steps != 0) {
				m_Start = evento.sY;
		std::cout << "Dragging " << delta << std::endl;

			vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
			GinkgoInteractorStyleImage2D* is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());
				if (is != NULL) {
					if (is->GetState() != VTKIS_ZOOM) {
		std::cout << "starting zoom " << std::endl;
						is->StartZoom();
					}
					double factor = 0.1 *steps * is->GetMouseWheelMotionFactor();
					//this->Dolly(pow((double)1.1, factor));
					is->DoZoom(factor, false);
				}

			}
		}
		
		evento.Skip(false);
	}
	else if( evento.Moving() ){
		m_NodoMoviendose = evento.iP;
		m_Estado = WBS_Ninguno;
	}

}

void GNC::GCS::Widgets::WZoomBuilder::OnKeyEvents(TEventoTeclado&)
{

}

void GNC::GCS::Widgets::WZoomBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WZoomBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_LUPA;
}

//endregion
