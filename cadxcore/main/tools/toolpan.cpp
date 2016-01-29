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
#include "toolpan.h"

#if defined(ENABLE_PANTOOL)
#include <export/contracts/iwidgetscontract.h>

#include <api/globals.h>
#include <api/iwidgetsmanager.h>
#include <resources/ginkgoresourcesmanager.h>

#include <iostream>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>


GNC::ToolPan::ToolPan() {
	m_pBuilder = NULL;
}

GNC::ToolPan::~ToolPan() 
{
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}

GNC::GCS::ITool* GNC::ToolPan::NewTool()
{
	return new GNC::ToolPan();
}

void GNC::ToolPan::Connect()
{
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
	m_pBuilder = new TBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
}

void GNC::ToolPan::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	// DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}
#endif