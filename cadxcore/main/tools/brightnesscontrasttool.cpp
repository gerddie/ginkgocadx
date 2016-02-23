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
