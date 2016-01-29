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
#include "wcamerarotation.h"
#include <main/managers/widgetsmanager.h>
#include <api/iwidgetsrenderer.h>
#include <api/westilo.h>
#include <vtkgl.h>
#include <vtkSmartPointer.h>

#include <cmath>

#include <vtk/vtkginkgoimageviewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkMath.h>
#include <vtkCamera.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>

//region "Implementacion de WPanBuilder"
#include <api/icontexto.h>


GNC::GCS::Widgets::WCameraRotation::WCameraRotation(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	m_Dentro = true;
	m_Changed = false;
}

GNC::GCS::Widgets::WCameraRotation::~WCameraRotation()
{
}

void GNC::GCS::Widgets::WCameraRotation::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	
	if (!m_pManager) {
		return;
	}
	if ( (m_MouseDown && evento.ButtonUp(m_ButtonMask)) || evento.Leaving()) {
		m_MouseDown = false;
		if (m_Estado != WBS_Creando) {
			m_Estado = WBS_Ninguno;
			return;
		}
		vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
		vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());

		if (is != NULL) {
			is->EndSpin();
		}
		if (m_Changed) {
			ConsumirEvento();
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
		m_Changed = false;
		m_Start = evento.iP;

		vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
		vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());

		if (is != NULL) {
			is->StartSpin();
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

		if (m_Start.DistanciaEuclideaCuadrado(evento.iP) > TVector::EpsilonDeReales()) {
			m_Changed = true;
		}

		if (is != NULL) {
			vtkRenderWindowInteractor *rwi = is->GetInteractor();

			double *center = is->GetCurrentRenderer()->GetCenter();

			  double newAngle = 
				vtkMath::DegreesFromRadians( atan2( rwi->GetEventPosition()[1] - center[1],
													rwi->GetEventPosition()[0] - center[0] ) );

			  double oldAngle = 
				vtkMath::DegreesFromRadians( atan2( rwi->GetLastEventPosition()[1] - center[1],
													rwi->GetLastEventPosition()[0] - center[0] ) );
  
			  vtkCamera *camera = is->GetCurrentRenderer()->GetActiveCamera();
			  camera->Roll( newAngle - oldAngle );
			  camera->OrthogonalizeViewUp();
      
			  rwi->Render();
			  //acumulate rotation...
			  evento.c->pRenderer->m_pImageViewer->RotateCamera(newAngle - oldAngle);
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

void GNC::GCS::Widgets::WCameraRotation::OnKeyEvents(TEventoTeclado&)
{

}

void GNC::GCS::Widgets::WCameraRotation::Render(GNC::GCS::Contexto3D* /*c*/)
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

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WCameraRotation::GetCursor()
{
	if (m_Estado == WBS_Creando && m_Changed) {
		return GNC::GCS::Widgets::CUR_MANO_CERRADA;
	} else {
		return GNC::GCS::Widgets::CUR_MANO_ABIERTA;
	}
}

//endregion
