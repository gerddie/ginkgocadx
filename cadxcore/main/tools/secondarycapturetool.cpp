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


#include "secondarycapturetool.h"
#include <export/contracts/iwidgetscontract.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/modificacionimagen.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>
#include <main/entorno.h>
#include <commands/secondarycapturedicomizecommand.h>
#include <main/controllers/commandcontroller.h>


#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtk/vtkginkgoimageviewer.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>

#include <resources/ginkgoresourcesmanager.h>

GNC::GCS::ITool* GNC::SecondaryCaptureTool::NewTool()
{
        return new GNC::SecondaryCaptureTool();
}

GNC::SecondaryCaptureTool::SecondaryCaptureTool()
{
}
GNC::SecondaryCaptureTool::~SecondaryCaptureTool()
{
}

bool GNC::SecondaryCaptureTool::ExecuteAction()
{
        std::string tempName = GNC::Entorno::Instance()->CreateGinkgoTempFile();
        {
                vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
                        vtkSmartPointer<vtkWindowToImageFilter>::New();
                windowToImageFilter->SetInput(WidgetsContract->GetViewerActivo()->GetRenderWindow());
                windowToImageFilter->SetInputBufferTypeToRGB(); //also record the alpha (transparency) channel
                windowToImageFilter->Update();

                vtkSmartPointer<vtkJPEGWriter> writer =
                        vtkSmartPointer<vtkJPEGWriter>::New();
                writer->SetFileName(tempName.c_str());
                writer->SetQuality(97);
                writer->ProgressiveOn();
                writer->SetInputConnection(windowToImageFilter->GetOutputPort());
                writer->Write();
        }
        GADAPI::SecondaryCaptureDicomizeCommandParameters* pParams = new GADAPI::SecondaryCaptureDicomizeCommandParameters(WidgetsContract->GetManager()->GetVista(), tempName);
        GADAPI::SecondaryCaptureDicomizeCommand* pCmd = new GADAPI::SecondaryCaptureDicomizeCommand(pParams);
        GNC::CommandController::Instance()->ProcessAsync("SC", pCmd, WidgetsContract->GetManager()->GetVista());

        return true;
}
