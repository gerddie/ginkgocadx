/*
 *  
 *  $Id: brightnesscontrasttool.cpp $
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
#include "brightnesscontrasttool.h"
#if defined(ENABLE_BRIGHTNESSCONTRASTTOOL)

#include <api/globals.h>
#include <export/contracts/iwidgetscontract.h>
#include <api/iwidgetsmanager.h>
#include <widgets/wbrightnesscontrast.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

GNC::BrightnessContrastTool::BrightnessContrastTool() : m_pBuilder(NULL)
{
	m_pBuilder = NULL;
}

GNC::BrightnessContrastTool::~BrightnessContrastTool() {

}

//------------------------------------------------------------------------------
//region "ITool/IContract realization"

GNC::GCS::ITool* GNC::BrightnessContrastTool::NewTool()
{
	return new GNC::BrightnessContrastTool();
}

void GNC::BrightnessContrastTool::Connect()
{
	if (m_pBuilder != NULL)
	{
		delete m_pBuilder;
		m_pBuilder = NULL;
	}

	m_pBuilder = new GNC::GCS::Widgets::WBrightnessContrastBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long) this);
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
}

void GNC::BrightnessContrastTool::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	if (m_pBuilder != NULL)
	{		
		// DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}

//endregion
#endif