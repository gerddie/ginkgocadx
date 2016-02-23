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
