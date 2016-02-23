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


#include "resettool.h"
#if defined(ENABLE_RESETTOOL)
#include <export/contracts/iwidgetscontract.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/modificacionimagen.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>


#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <resources/ginkgoresourcesmanager.h>

GNC::GCS::ITool* GNC::ResetTool::NewTool()
{
	return new GNC::ResetTool();
}

GNC::ResetTool::ResetTool()
{
}
GNC::ResetTool::~ResetTool()
{
}
		
bool GNC::ResetTool::ExecuteAction()
{	
	GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(WidgetsContract->GetManager()->GetVista(),GNC::GCS::Events::EventoModificacionImagen::VisualizacionImagenModificada);
	pEvt->EnableResetWindowLevel();
	pEvt->EnableResetBrightnessContrast();
	pEvt->EnableResetCurrentPoint();
	pEvt->EnableResetZoom();
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(pEvt);
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
	return true;
}

#endif
