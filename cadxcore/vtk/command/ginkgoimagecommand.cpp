/*
 *  
 *  $Id: ginkgoimagecommand.cpp $
 *  Ginkgo CADx Project
 *
 *  Code adapted from vtkINRIA3D
=========================================================================

Program:   vtkINRIA3D
Module:    $Id: ginkgoimagecommand.cpp $
Language:  C++
Author:    $Author: filus $
Date:      $Date: 2008-06-06 18:10:57 +0200 (vie, 06 jun 2008) $
Version:   $Revision: 880 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
See Copyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#ifdef _MSC_VER
#  pragma warning (disable : 4018)
#endif

#include <string.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>
#include <vtk/vtkginkgoimageviewer.h>

#include <vtkAbstractPropPicker.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>


#include "ginkgoimagecommand.h"


GinkgoImageCommand::GinkgoImageCommand() :
  View(NULL), InitialWindow(0), InitialLevel(0),
  WindowEventStatus(false)
{
}

void
GinkgoImageCommand::Execute(vtkObject*    caller,
                                 unsigned long event,
                                 void*         /*callData*/)
{
  if (this->View->GetInput() == NULL){
    return;
  }

  if (event == vtkCommand::KeyPressEvent)
  {

    vtkRenderWindowInteractor *rwi = this->View->GetRenderWindow()->GetInteractor();
    char key = rwi->GetKeyCode();

    if ( key=='i' || key=='I')
    {
      //int mode;
      //mode = this->View->GetInterpolationMode();
      //this->View->SetInterpolationMode((++mode)%2);
      //this->View->Render();
      return;
    }

    return;
  }

  if (event == vtkCommand::StartWindowLevelEvent) {
    this->StartWindowing();
    return;
  }
  if (event == vtkCommand::EndWindowLevelEvent) {
    this->EndWindowing();
    return;
  }
  if (event == vtkCommand::WindowLevelEvent) {
    this->Windowing(GinkgoInteractorStyleImage2D::SafeDownCast(caller));
    return;
  }
  if (event == vtkCommand::ResetWindowLevelEvent) {
    //this->View->SyncResetWindowLevel();
    //std::cout << "GinkgoImageCommand::ResetWindowLevelEvent()" << std::endl;
    this->View->ResetToDefaultWindowLevel();
    this->View->SyncSetWindow ( this->View->GetWindow() );
    this->View->SyncSetLevel ( this->View->GetLevel() );
    this->View->Render();
    return;
  }
  if (event == GinkgoImageCommand::ResetPositionEvent) {
  	 //std::cout << "GinkgoImageCommand::ResetPositionEvent()" << std::endl;
    //this->View->SyncResetCurrentPoint();
    //this->View->Render();
    return;
  }
  if (event == GinkgoImageCommand::ZoomEvent) {
	 //std::cout << "GinkgoImageCommand::ZoomEvent()" << std::endl;
    this->Zoom(GinkgoInteractorStyleImage2D::SafeDownCast(caller));
    return;
  }
  if (event == GinkgoImageCommand::ResetZoomEvent) {
  	 //std::cout << "GinkgoImageCommand::ResetZoomEvent()" << std::endl;
    //this->View->SyncResetZoom();
    //this->View->Render();
    return;
  }
  if (event == vtkCommand::StartPickEvent) {
    this->StartPicking(vtkInteractorStyleImage::SafeDownCast(caller));
    return;
  }
  if (event == vtkCommand::EndPickEvent) {
    this->EndPicking(/*static_cast<vtkInteractorStyleImage *>(caller)*/);
    return;
  }
  if (event == GinkgoImageCommand::StartMeasureEvent) {
    //std::cout<<"StartMeasureEvent\n";
    return;
  }
  if (event == GinkgoImageCommand::MeasureEvent) {
    //std::cout<<"MeasureEvent\n";
    return;
  }
  if (event == GinkgoImageCommand::EndMeasureEvent) {
    //std::cout<<"EndMeasureEvent\n";
    return;
  }
  if (event == GinkgoImageCommand::StartZSliceMoveEvent) {
    return;
  }
  if (event == GinkgoImageCommand::ZSliceMoveEvent) {
    this->ChangeZSlice(GinkgoInteractorStyleImage2D::SafeDownCast(caller));
    return;
  }
  if (event == GinkgoImageCommand::EndZSliceMoveEvent) {
    return;
  }
  if (event == GinkgoImageCommand::FullPageEvent) {
    return;
  }

}

void
GinkgoImageCommand::StartWindowing()
{
	this->InitialWindow = this->View->GetWindow();
	this->InitialLevel  = this->View->GetLevel();
}

void
GinkgoImageCommand::Windowing(GinkgoInteractorStyleImage2D* p_isi)
{

  if( !p_isi )
  {
    return;
  }

  double window = this->InitialWindow;
  double level  = this->InitialLevel;
  double EPS    = 0.01;


  double dx = p_isi->GetWindowStep();
  double dy = p_isi->GetLevelStep();

  // Scale by current values
  if (fabs(window) > EPS)
  {
    dx = dx * window;
  }
  else
  {
    dx = dx * (window < 0 ? -EPS : EPS);
  }

  if (fabs(level) > EPS)
  {
    dy = dy * level;
  }
  else
  {
    dy = dy * (level < 0 ? -EPS : EPS);
  }

  // Abs so that direction does not flip
  if (window < 0.0f)
  {
    dx = -1*dx;
  }
  if (level < 0.0f)
  {
    dy = -1*dy;
  }

  // Compute new window level
  double newWindow = dx + window;
  double newLevel  = level - dy;

  // Stay away from zero and really
  if (fabs(newWindow) < EPS)
  {
    newWindow = EPS * (newWindow < 0 ? -1 : 1);
  }

  if (fabs(newLevel) < EPS)
  {
    newLevel = EPS * (newLevel < 0 ? -1 : 1);
  }

  this->View->SyncSetWindow(newWindow);
  this->View->SyncSetLevel(newLevel);
  this->View->Modified();
}

void
GinkgoImageCommand::EndWindowing()
{
}

void
GinkgoImageCommand::StartPicking(vtkInteractorStyleImage* p_isi)
{
  vtkRenderWindowInteractor *rwi = p_isi->GetInteractor();
  vtkAssemblyPath *path=NULL;
  p_isi->FindPokedRenderer(rwi->GetEventPosition()[0],
                           rwi->GetEventPosition()[1]);
  rwi->GetPicker()->Pick(rwi->GetEventPosition()[0],
                         rwi->GetEventPosition()[1], 0.0,
                         p_isi->GetCurrentRenderer());
  vtkAbstractPropPicker *picker;
  if ( (picker=vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker())) )
  {
    path = picker->GetPath();
  }

  if ( path != NULL )
  {
    double* world = picker->GetPickPosition();
	
	double  pos[3] = {0.0, 0.0, 0.0};
    //this->View->GetCurrentPoint(pos);

    //double* imBounds = this->View->GetImage()->GetBounds();

    switch(this->View->GetOrientation())
    {
        case vtkGinkgoImageViewer::SAGITTAL_ID:

          pos [1] = world[0];//*1.0;
          pos [2] = world[1];//*1.0;
          break;


        case vtkGinkgoImageViewer::CORONAL_ID:

          if( this->View->GetConventions() == vtkGinkgoImageViewer::RADIOLOGIC )
          {
            pos [0] = world[0];//*-1.0;
          }
          else
          {
            pos [0] = world[0]*-1.0;
          }
          pos [2] = world[1];
          break;


        case vtkGinkgoImageViewer::AXIAL_ID:

          if( this->View->GetConventions() == vtkGinkgoImageViewer::RADIOLOGIC )
          {
            pos [0] = world[0];
          }
          else
          {
            pos [0] = world[0]*-1.0;
          }
          pos [1] = world[1]*-1.0;
          break;

    }

    // Treat extrem positions
	int range[3][2] = {{0, 0}, {0, 0}, {0, 0}};
	this->View->GetSliceRange(vtkGinkgoImageViewer::X, range[0]);
	this->View->GetSliceRange(vtkGinkgoImageViewer::Y, range[1]);
	this->View->GetSliceRange(vtkGinkgoImageViewer::Z, range[2]);

    for (unsigned int i=0; i<3; i++)
    {
		if (pos[i] < range[i][0]) {
			pos[i] = range[i][0] + 0.0005;
		}
		if (pos[i] > range[i][1]) {
			pos[i] = range[i][1] - 0.0005;
		}
    }

    // Set the position
    //this->View->SyncSetCurrentPoint(pos);
    //this->View->Modified();
  }

}

void
GinkgoImageCommand::EndPicking()
{
}

void
GinkgoImageCommand::ChangeZSlice(GinkgoInteractorStyleImage2D* /*p_isi*/)
{

	/*
  if( !p_isi )
  {
    return;
  }

  int p_nbSlices = p_isi->GetZSliceStep();

  int current_slice = 0;
  int dest_slice    = current_slice + p_nbSlices;
  //int min_slice     = this->View->GetWholeZMin();
  //int max_slice     = this->View->GetWholeZMax();

  double pos[3];
  this->View->GetPositionForSlice ( dest_slice, this->View->GetOrientation(), pos);
  //this->View->SyncSetPosition (pos);

  if ((dest_slice > min_slice) && (dest_slice < max_slice)) {
    this->View->GetPositionForSlice ( dest_slice, this->View->GetOrientation(), pos);
    //this->View->SyncSetSlice(this->View->GetOrientation(), dest_slice);
  }
  if ((dest_slice >= max_slice) && (current_slice != max_slice)) {
    this->View->GetPositionForSlice ( max_slice, this->View->GetOrientation(), pos);
    //this->View->SyncSetSlice(this->View->GetOrientation(), max_slice);
  }
  if ((dest_slice <= min_slice) && (current_slice != min_slice)) {
    this->View->GetPositionForSlice ( min_slice, this->View->GetOrientation(), pos);
    //this->View->SyncSetSlice(this->View->GetOrientation(), min_slice);
  }

  //this->View->Modified();
  */
}


void
GinkgoImageCommand::SetView(vtkGinkgoImageViewer *p_view)
{
  this->View = p_view;
}



void
GinkgoImageCommand::Zoom (vtkInteractorStyleImage* p_isi)
{
  if( !p_isi )
  {
    return;
  }

  vtkRenderWindowInteractor *rwi = p_isi->GetInteractor();

  // vtkAssemblyPath *path=NULL;
  p_isi->FindPokedRenderer(rwi->GetEventPosition()[0],
                           rwi->GetEventPosition()[1]);

  double *center = p_isi->GetCurrentRenderer()->GetCenter();
  int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

  double factor = 10.0 * (double)(dy) / (double)(center[1]);

  //this->View->SyncSetZoom ( pow((double)1.1, factor) );
  this->View->SyncSetZoom ( pow((double)1.1, factor)*this->View->GetZoom() );
  this->View->Modified();
}
