/*
 *  
 *  $Id: wpan.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#include <api/globals.h>
#include "wtrackball.h"
#include <main/managers/widgetsmanager.h>
#include <api/iwidgetsrenderer.h>
#include <vtkgl.h>
#include <vtkSmartPointer.h>

#include <cmath>

#include <vtk/vtkginkgoimageviewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>

//region "Implementacion de WPanBuilder"
#include <api/icontexto.h>


GNC::GCS::Widgets::WTrackballBuilder::WTrackballBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	m_Dentro = true;
}

GNC::GCS::Widgets::WTrackballBuilder::~WTrackballBuilder()
{
}

void GNC::GCS::Widgets::WTrackballBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	
	if (!m_pManager) {
		return;
	}
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask) || evento.Leaving()) {
		m_MouseDown = false;
		if (m_Estado != WBS_Creando) {
			m_Estado = WBS_Ninguno;
			return;
		}
		vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
		vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());

		if (is != NULL) {
			if (is->GetState() == VTKIS_ROTATE) {
				is->EndRotate();
			} else {
				is->EndSpin();
			}
		}

		m_Estado = WBS_Ninguno;
		//m_pManager->Modificado();
		//no se consume el evento para poder mostrar las propiedades de un widget
	}
	else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}
		m_MouseDown = true;

		vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
		vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());

		if (is != NULL) {
			if (evento.ControlDown()) {
				is->StartSpin();
			} else {
				is->StartRotate();
			}
		}

		m_Estado = WBS_Creando;
		evento.Skip(false);
	}
	else if (evento.Dragging() && m_MouseDown) {
		if (m_Estado != WBS_Creando) {
			return;
		}
		vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
		vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());

		if (is != NULL) {
			if (is->GetState() == VTKIS_ROTATE) {
				is->Rotate();
			} else {
				is->Spin();
			}
			is->PropagateCameraFocalAndPosition();
		}
		
		//m_pManager->Modificado();
		evento.Skip(false);
	}
	else if( evento.Moving() ){
		GTRACE("Moviendose");
		m_NodoMoviendose = evento.iP;
		m_Estado = WBS_Ninguno;
	}

}

void GNC::GCS::Widgets::WTrackballBuilder::OnKeyEvents(TEventoTeclado&)
{

}

void GNC::GCS::Widgets::WTrackballBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

	/*
	TVector box = TVector(100, 10) * c->RefRelacionImagenPantalla();
	
	box = TVector(100, 10) * c->RefRelacionImagenPantalla();
	TVector pos = TVector(std::max(m_Start.x - box.x, std::min(m_Start.x + box.x, m_Stop.x)), m_Start.y);
	
	if (m_Estado == WBS_Creando) {
		glLineWidth(2.0f);
		glColor4f(0.0f, 0.75f, 0.0f, 1.0f);
		
		glBegin(GL_LINES);
		
			glVertex2d(m_Start.x - box.x, m_Start.y);
			glVertex2d(m_Start.x + box.x, m_Start.y);
		
			glVertex2d(m_Start.x - box.x, m_Start.y - box.y);
			glVertex2d(m_Start.x - box.x, m_Start.y + box.y);
		
			glVertex2d(m_Start.x + box.x, m_Start.y - box.y);
			glVertex2d(m_Start.x + box.x, m_Start.y + box.y);
		
		glEnd();
		
		glColor4f(0.75f, 0.0f, 0.0f, 1.0f);
		
		glLineWidth(1.5);
		glBegin(GL_LINES);
			glVertex2d(pos.x, pos.y - box.y);
			glVertex2d(pos.x, pos.y + box.y);
		glEnd();
	}
	else {

	}
	*/
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WTrackballBuilder::GetCursor()
{
	if (m_Estado == WBS_Creando) {
		return GNC::GCS::Widgets::CUR_MANO_CERRADA;
	} else {
		return GNC::GCS::Widgets::CUR_MANO_ABIERTA;
	}
}

//endregion
