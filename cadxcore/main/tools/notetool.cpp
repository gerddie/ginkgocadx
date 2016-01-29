/*
 *  
 *  $Id: notetool.cpp $
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
#include "notetool.h"
#if defined(ENABLE_NOTETOOL)
#include <export/contracts/iwidgetscontract.h>

#include <api/globals.h>
#include <api/iwidgetsmanager.h>

#include <widgets/wnota.h>

#include <resources/ginkgoresourcesmanager.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>


GNC::NoteTool::NoteTool() {
	m_pBuilder = NULL;
}

GNC::NoteTool::~NoteTool() {
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}


//------------------------------------------------------------------------------
//region "ITool/IContract realization"

GNC::GCS::ITool* GNC::NoteTool::NewTool()
{
	return new GNC::NoteTool();
}

void GNC::NoteTool::Connect()
{
	if (m_pBuilder != NULL)
	{
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
	m_pBuilder = new GNC::GCS::Widgets::WNotaBuilder(WidgetsContract->GetManager(), GetTriggerButton());
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
}

void GNC::NoteTool::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	if (m_pBuilder != NULL)
	{
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}

//endregion
#endif
