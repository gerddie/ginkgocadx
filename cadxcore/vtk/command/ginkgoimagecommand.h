/*
 *
 *  $Id: ginkgoimagecommand.h $
 *  Ginkgo CADx Project
 *
 *  Code adapted from vtkINRIA3D
=========================================================================

Program:   vtkINRIA3D
Module:    $Id: ginkgoimagecommand.h $
Language:  C++
Author:    $Author: filus $
Date:      $Date: 2008-01-14 09:21:23 +0100 (lun, 14 ene 2008) $
Version:   $Revision: 608 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
See Copyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#pragma once

#include<vtkCommand.h>
#include<vtkObjectFactory.h>


class vtkInteractorStyleImage;
class vtkInteractorStyleImage2D;
class vtkGinkgoImageViewer;

class GinkgoImageCommand : public vtkCommand
{
public:
        GinkgoImageCommand();

        static  GinkgoImageCommand* New()
        {
                return new GinkgoImageCommand;
        }

        void Execute(vtkObject *   caller,
                     unsigned long event,
                     void *        callData);

        void SetView(vtkGinkgoImageViewer *p_view);

        //BTX
        enum EventIds {
                ZSliceMoveEvent=(vtkCommand::UserEvent+1),
                StartZSliceMoveEvent,
                EndZSliceMoveEvent,
                StartMeasureEvent,
                MeasureEvent,
                EndMeasureEvent,
                StartAngleEvent,
                AngleEvent,
                EndAngleEvent,
                FullPageEvent,
                ResetPositionEvent,
                ZoomEvent,
                ResetZoomEvent
        };
        //ETX

private:
        void ChangeZSlice(GinkgoInteractorStyleImage2D* p_isi);
        void EndWindowing();
        void Windowing(GinkgoInteractorStyleImage2D* p_isi);
        void StartWindowing();
        void StartPicking(vtkInteractorStyleImage* p_isi);
        void EndPicking (void);
        void Zoom (vtkInteractorStyleImage* p_isi);

private:
        vtkGinkgoImageViewer* View;
        double          InitialWindow;
        double          InitialLevel;

        bool            WindowEventStatus;

};

