/*
 *  
 *  $Id: resettool.cpp $
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