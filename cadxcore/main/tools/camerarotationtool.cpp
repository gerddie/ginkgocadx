/*
*  
*  $Id: toolpan.cpp $
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
#include "camerarotationtool.h"


#include <export/contracts/iwidgetscontract.h>

#include <api/globals.h>
#include <api/iwidgetsmanager.h>
#include <resources/ginkgoresourcesmanager.h>
#include <widgets/wcamerarotation.h>

#include <iostream>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>


GNC::CameraRotationTool::CameraRotationTool() {
	m_pBuilder = NULL;
}

GNC::CameraRotationTool::~CameraRotationTool() 
{
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}

GNC::GCS::ITool* GNC::CameraRotationTool::NewTool()
{
	return new GNC::CameraRotationTool();
}

void GNC::CameraRotationTool::Connect()
{
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
	m_pBuilder = new TBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
}

void GNC::CameraRotationTool::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	// DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}
