/*
 *  
 *  $Id: ginkgointeractorstyleimage2d.h $
 *  Ginkgo CADx Project
 *
 *  Code adapted from vtkINRIA3D
=========================================================================

Program:   vtkINRIA3D
Module:    $Id: ginkgointeractorstyleimage2d.h $
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

#include <api/api.h>
#include <vtkInteractorStyleImage.h>
#include "../vtkginkgoimageviewer.h"

// Motion flags (See VTKIS_PICK and so on in vtkInteractorStyleImage.h)
#define VTKIS_MEASURE      5050
#define VTKIS_ZSLICE_MOVE  5051

class wxTimer;

class EXTAPI GinkgoInteractorStyleImage2D : public vtkInteractorStyleImage
{
 public:
  static GinkgoInteractorStyleImage2D *New();
  vtkTypeMacro (GinkgoInteractorStyleImage2D, vtkInteractorStyleImage);
  
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();
  virtual void OnMouseWheelForward();
  virtual void OnMouseWheelBackward();
  virtual void OnChar();
  
  void SetView (vtkGinkgoImageViewer* view)
  {
    this->View = view;
  }
  
  vtkGetObjectMacro (View, vtkGinkgoImageViewer);
  vtkSetMacro (ZSliceStep, int);
  vtkGetMacro (ZSliceStep, int);

  vtkSetMacro (WindowStep, double);
  vtkGetMacro (WindowStep, double);

  vtkSetMacro (LevelStep,  double);
  vtkGetMacro (LevelStep,  double);

  vtkSetMacro (PreviewMode,  bool);
  vtkGetMacro (PreviewMode,  bool);


  virtual void StartZSliceMove();
  virtual void ZSliceMove();
  virtual void ZSliceWheelForward();
  virtual void ZSliceWheelBackward();
  virtual void EndZSliceMove();
  virtual void FullPage();
  virtual void StartMeasure();
  virtual void Measure();
  virtual void EndMeasure();
  virtual void WindowLevel ();
  virtual void WindowLevelWheelForward ();
  virtual void WindowLevelWheelBackward ();

  void PropagateCameraFocalAndPosition();

  void DoZoom(double factor, bool launchTimer = true);

  vtkRenderer* GetCurrentRenderer() { return CurrentRenderer; }
  
 protected:
  GinkgoInteractorStyleImage2D();
  ~GinkgoInteractorStyleImage2D();

 private:
  
 private:
  vtkGinkgoImageViewer*  View;
  int                    ZSliceStep;
  double                 WindowStep;
  double                 LevelStep;
  bool						PreviewMode;
  wxTimer*				TimerZoom;
};

