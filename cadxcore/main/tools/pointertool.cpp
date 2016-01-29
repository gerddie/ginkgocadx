/*
 *  
 *  $Id: pointertool.cpp $
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
#include <export/contracts/iwidgetscontract.h>
#include <api/iwidgetsmanager.h>
#include "pointertool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>
#include <widgets/wseleccion.h>

#include <iostream>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>


GNC::PointerTool::PointerTool() {
	m_pSeleccionBuilder = NULL;
}

GNC::PointerTool::~PointerTool() 
{
	if (m_pSeleccionBuilder != NULL)
	{		
		// DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
		delete m_pSeleccionBuilder;
		m_pSeleccionBuilder = NULL;
	}
}

GNC::GCS::ITool* GNC::PointerTool::NewTool()
{
	return new GNC::PointerTool();
}

void GNC::PointerTool::Connect()
{
	if (m_pSeleccionBuilder != NULL)
	{		
		// DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
		delete m_pSeleccionBuilder;
		m_pSeleccionBuilder = NULL;
	}
	m_pSeleccionBuilder = new TSeleccionBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(m_pSeleccionBuilder->GetCursor());		
}

void GNC::PointerTool::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	if (m_pSeleccionBuilder != NULL)
	{		
		// DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
		delete m_pSeleccionBuilder;
		m_pSeleccionBuilder = NULL;
	}
}
