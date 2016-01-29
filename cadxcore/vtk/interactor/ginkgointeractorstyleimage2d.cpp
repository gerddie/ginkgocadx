/*
 *  
 *  $Id: ginkgointeractorstyleimage2d.cpp $
 *  Ginkgo CADx Project
 *
 *  Code adapted from vtkINRIA3D
=========================================================================

Program:   vtkINRIA3D
Module:    $Id: ginkgointeractorstyleimage2d.cpp $
Language:  C++
Author:    $Author: filus $
Date:      $Date: 2008-03-11 17:32:52 +0100 (mar, 11 mar 2008) $
Version:   $Revision: 752 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
See Copyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include "ginkgointeractorstyleimage2d.h"
#include "../command/ginkgoimagecommand.h"
#include <vtkAbstractPropPicker.h>
#include <vtkAssemblyPath.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <wx/timer.h>

vtkStandardNewMacro(GinkgoInteractorStyleImage2D);

#include <vtkCamera.h>
#include <vtkRenderer.h>

class ISTimerZoom: public wxTimer
{
public:
	ISTimerZoom(GinkgoInteractorStyleImage2D* pIS): IS(pIS)
	{
	}
	virtual void Notify()
	{
		IS->EndZoom();
	}

	GinkgoInteractorStyleImage2D* IS;
};

//----------------------------------------------------------------------------

GinkgoInteractorStyleImage2D::GinkgoInteractorStyleImage2D()
: vtkInteractorStyleImage()
{
	this->View = 0;
	this->ZSliceStep = 0;
	this->LevelStep = 0.0;
	this->WindowStep = 0.0;
	this->PreviewMode = false;
	this->TimerZoom = new ISTimerZoom(this);
}

//----------------------------------------------------------------------------

GinkgoInteractorStyleImage2D::~GinkgoInteractorStyleImage2D()
{
	if (TimerZoom != NULL) {
		TimerZoom->Stop();
		delete TimerZoom;
	}
}

//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnMouseMove()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	switch (this->State) {
		case VTKIS_WINDOW_LEVEL:
			this->FindPokedRenderer(x, y);
			this->WindowLevel();
			this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			break;

		case VTKIS_PICK:
			// No Drag is allowed for picking. Toggle to ZSliceMove state
			// We force StartZSliceMove or StartPan or StartDolly without ending the picking.
			// We don't want to pick at all
			this->StopState();
			this->StartZSliceMove();
			// There we don't break to let the hand to ZSliceMove

		case VTKIS_ZSLICE_MOVE:
			this->FindPokedRenderer(x, y);
			this->ZSliceMove();
			this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			break;

		case VTKIS_DOLLY:
			this->FindPokedRenderer(x, y);
			//this->Dolly();
			//this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			this->InvokeEvent(GinkgoImageCommand::ZoomEvent, this);
			break;

		case VTKIS_PAN:
			if (PreviewMode) {
				this->FindPokedRenderer(x, y);
				this->Pan();
				this->PropagateCameraFocalAndPosition();
				this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			}
			break;
		default:
			break;
	}
}

void GinkgoInteractorStyleImage2D::PropagateCameraFocalAndPosition()
{
	if (!this->GetView()) {
		return;
	}

	double pos[3], focal[3];

	if (!this->GetView()->GetRenderer()) {
		return;
	}

	vtkCamera* camera = this->GetView()->GetRenderer()->GetActiveCamera();
	camera->GetFocalPoint(focal);
	camera->GetPosition(pos);
	
	bool LinkCamera = this->GetView()->GetLinkCameraFocalAndPosition();
	this->GetView()->SetLinkCameraFocalAndPosition(false);
	this->GetView()->SyncSetCameraFocalAndPosition(focal, pos);
	this->GetView()->SetLinkCameraFocalAndPosition(LinkCamera);

	this->GetView()->Render();


}


//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	this->FindPokedRenderer(x, y);
	if (!this->CurrentRenderer) {
		return;
	}

	//switch ( this->View->GetInteractionStyle() )
	switch (this->View->GetLeftButtonInteractionStyle()) {
		/*
		case vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION :
			this->WindowLevelStartPosition[0] = x;
			this->WindowLevelStartPosition[1] = y;
			this->StartWindowLevel();
			break;
		*/
		case vtkGinkgoImageViewer::SELECT_INTERACTION :
			if (this->PreviewMode) {
				this->StartPick();
			}
			break;
		case vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION :
			if (this->PreviewMode) {
				this->StartPick();
				this->StopState();
			}
			//this->StartPan();
			break;
		/*
		case vtkGinkgoImageViewer::FULL_PAGE_INTERACTION :
			this->FullPage();
			break;
		case vtkGinkgoImageViewer::MEASURE_INTERACTION :
			this->StartMeasure();
			break;
		*/
		case vtkGinkgoImageViewer::ZOOM_INTERACTION :
			/*
			if (this->Interactor->GetShiftKey()) {
				this->StartPan();
			} else {
				this->StartDolly(); // continuous zoom
			}
			*/
			break;
	}

}

//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnMiddleButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	this->FindPokedRenderer(x, y);
	if (!this->CurrentRenderer) {
		return;
	}

	//switch ( this->View->GetInteractionStyle() )
	switch (this->View->GetMiddleButtonInteractionStyle()) {
		/*
		case vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION :
			this->WindowLevelStartPosition[0] = x;
			this->WindowLevelStartPosition[1] = y;
			this->StartWindowLevel();
			break;
		*/
		case vtkGinkgoImageViewer::SELECT_INTERACTION :
		if (this->PreviewMode) {
			this->StartPick();
		} else {
			this->StartPan();
		}
			break;
		case vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION :
			if (this->PreviewMode) {
				this->StartPan();
			}
			break;
		/*
		case vtkGinkgoImageViewer::FULL_PAGE_INTERACTION :
			this->FullPage();
			break;
		case vtkGinkgoImageViewer::MEASURE_INTERACTION :
			this->StartMeasure();
			break;
		*/
		case vtkGinkgoImageViewer::ZOOM_INTERACTION :
			if (!this->PreviewMode) {
				this->StartPan();
			}
			break;
	}
}

//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnMiddleButtonUp()
{
	switch (this->State) {
		case VTKIS_ZSLICE_MOVE:
		if (this->PreviewMode) {
			this->EndZSliceMove();
}
			break;
		case VTKIS_WINDOW_LEVEL:
			if (this->PreviewMode) {			this->EndWindowLevel();
}
			break;
		case VTKIS_PICK:
			if (this->PreviewMode) {			this->EndPick();
}
			break;
		case VTKIS_MEASURE:
			if (this->PreviewMode) {			this->EndMeasure();
}
			break;
		case VTKIS_PAN:
			this->EndPan();
			break;
		case VTKIS_DOLLY:
			if (this->PreviewMode) {			this->EndDolly(); // continuous zoom
}
			break;
	}

}

//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnLeftButtonUp()
{
	switch (this->State) {
		case VTKIS_ZSLICE_MOVE:
			if (this->PreviewMode) {			this->EndZSliceMove();
}
			break;
		case VTKIS_WINDOW_LEVEL:
			if (this->PreviewMode) {			this->EndWindowLevel();
}
			break;
		case VTKIS_PICK:
			if (this->PreviewMode) {			this->EndPick();
}
			break;
		case VTKIS_MEASURE:
			if (this->PreviewMode) {			this->EndMeasure();
}
			break;
		case VTKIS_DOLLY: // continuous zoom
			if (this->PreviewMode) {			this->EndDolly();
}
			break;
		case VTKIS_PAN: // continuous zoom
			if (this->PreviewMode) {			this->EndPan();
}
			break;
	}
}

//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnRightButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	this->FindPokedRenderer(x, y);
	if (!this->CurrentRenderer) {
		return;
	}

	//switch ( this->View->GetInteractionStyle() )
	switch (this->View->GetRightButtonInteractionStyle()) {
		/*
		case vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION :
			this->WindowLevelStartPosition[0] = x;
			this->WindowLevelStartPosition[1] = y;
			this->StartWindowLevel();
			break;
		*/
		case vtkGinkgoImageViewer::SELECT_INTERACTION :
			if (this->PreviewMode) {			this->StartPick();
}
			break;
		case vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION :
			if (this->PreviewMode) {			this->StartPan();
}
			break;
		/*
		case vtkGinkgoImageViewer::FULL_PAGE_INTERACTION :
			this->FullPage();
			break;
		case vtkGinkgoImageViewer::MEASURE_INTERACTION :
			this->StartMeasure();
			break;
		*/
		case vtkGinkgoImageViewer::ZOOM_INTERACTION :
			if (this->PreviewMode) {			
				this->StartPan();
			}
			break;
	}
}

//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnRightButtonUp()
{
	switch (this->State) {
		case VTKIS_ZSLICE_MOVE:
			if (this->PreviewMode) {			this->EndZSliceMove();
}
			break;
		case VTKIS_WINDOW_LEVEL:
			if (this->PreviewMode) {			this->EndWindowLevel();
}
			break;
		case VTKIS_PICK:
			if (this->PreviewMode) {			this->EndPick();
}
			break;
		case VTKIS_MEASURE:
			if (this->PreviewMode) {			this->EndMeasure();
}
			break;
		case VTKIS_DOLLY: // continuous zoom
			if (this->PreviewMode) {			this->EndDolly();
}
			break;
		case VTKIS_PAN:
			
			if (this->PreviewMode) {			
				this->EndPan();
			}
			break;
	}
}

//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnChar()
{
	vtkRenderWindowInteractor *rwi = this->Interactor;

	//double factor = 0.0;
	//int *size = this->View->GetRenderWindow()->GetSize();

	std::string key_sym(rwi->GetKeySym());
	
	vtkAssemblyPath *path = 0;
	vtkAbstractPropPicker *picker = 0;

	switch (rwi->GetKeyCode()) {

		case 'f':
		case 'F':
		{
			this->AnimState = VTKIS_ANIM_ON;
			this->FindPokedRenderer(rwi->GetEventPosition()[0],
						rwi->GetEventPosition()[1]);
			rwi->GetPicker()->Pick(rwi->GetEventPosition()[0],
						rwi->GetEventPosition()[1], 0.0,
						this->CurrentRenderer);
			picker = vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker());
			if (picker) {
				path = picker->GetPath();
			}
			if (path) {
				rwi->FlyToImage(this->CurrentRenderer, picker->GetPickPosition());
			}
			this->AnimState = VTKIS_ANIM_OFF;
			break;
		}

		case 'r':
		case 'R':
			/*
			if (this->View->GetLeftButtonInteractionStyle() == vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION ||
						this->View->GetRightButtonInteractionStyle() == vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION ||
						this->View->GetMiddleButtonInteractionStyle() == vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION ||
						this->View->GetWheelInteractionStyle() == vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION) {
				this->InvokeEvent(vtkCommand::ResetWindowLevelEvent, this);
			}
			*/
			if (this->View->GetLeftButtonInteractionStyle() == vtkGinkgoImageViewer::SELECT_INTERACTION ||
						this->View->GetRightButtonInteractionStyle() == vtkGinkgoImageViewer::SELECT_INTERACTION ||
						this->View->GetMiddleButtonInteractionStyle() == vtkGinkgoImageViewer::SELECT_INTERACTION ||
						this->View->GetWheelInteractionStyle() == vtkGinkgoImageViewer::SELECT_INTERACTION) {
				this->InvokeEvent(GinkgoImageCommand::ResetPositionEvent, this);
			}
			if (this->View->GetLeftButtonInteractionStyle() == vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION ||
						this->View->GetRightButtonInteractionStyle() == vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION ||
						this->View->GetMiddleButtonInteractionStyle() == vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION ||
						this->View->GetWheelInteractionStyle() == vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION) {
				this->InvokeEvent(GinkgoImageCommand::ResetPositionEvent, this);
				this->InvokeEvent(GinkgoImageCommand::ResetZoomEvent, this);
			}
			if (this->View->GetLeftButtonInteractionStyle() == vtkGinkgoImageViewer::ZOOM_INTERACTION ||
						this->View->GetRightButtonInteractionStyle() == vtkGinkgoImageViewer::ZOOM_INTERACTION ||
						this->View->GetMiddleButtonInteractionStyle() == vtkGinkgoImageViewer::ZOOM_INTERACTION ||
						this->View->GetWheelInteractionStyle() == vtkGinkgoImageViewer::ZOOM_INTERACTION) {
				this->InvokeEvent(GinkgoImageCommand::ResetZoomEvent, this);
			}

			break;
	}

}

//----------------------------------------------------------------------------

void
GinkgoInteractorStyleImage2D::StartZSliceMove()
{
	if ((this->State != VTKIS_NONE) && (this->State != VTKIS_PICK)) {
		return;
	}
	this->StartState(VTKIS_ZSLICE_MOVE);
	this->InvokeEvent(GinkgoImageCommand::StartZSliceMoveEvent, this);
}

void
GinkgoInteractorStyleImage2D::ZSliceMove()
{
	vtkRenderWindowInteractor *rwi = this->Interactor;
	int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
	this->ZSliceStep = dy;
	this->InvokeEvent(GinkgoImageCommand::ZSliceMoveEvent, this);
}

void
GinkgoInteractorStyleImage2D::ZSliceWheelForward()
{
	int dy = (int) this->MouseWheelMotionFactor;
	this->ZSliceStep = dy;
	this->InvokeEvent(GinkgoImageCommand::ZSliceMoveEvent, this);
}

void
GinkgoInteractorStyleImage2D::ZSliceWheelBackward()
{
	int dy = (int) (-1.0 * this->MouseWheelMotionFactor);
	this->ZSliceStep = dy;
	this->InvokeEvent(GinkgoImageCommand::ZSliceMoveEvent, this);
}

void
GinkgoInteractorStyleImage2D::EndZSliceMove()
{
	if (this->State != VTKIS_ZSLICE_MOVE) {
		return;
	}
	this->InvokeEvent(GinkgoImageCommand::EndZSliceMoveEvent, this);
	this->StopState();
}

//----------------------------------------------------------------------------

void
GinkgoInteractorStyleImage2D::FullPage()
{
	this->InvokeEvent(GinkgoImageCommand::FullPageEvent, this);
}

//----------------------------------------------------------------------------

void
GinkgoInteractorStyleImage2D::StartMeasure()
{
	if (this->State != VTKIS_NONE) {
		return;
	}
	this->StartState(VTKIS_MEASURE);
	this->InvokeEvent(GinkgoImageCommand::StartMeasureEvent, this);
}

void
GinkgoInteractorStyleImage2D::Measure()
{
	this->InvokeEvent(GinkgoImageCommand::MeasureEvent, this);
}

void
GinkgoInteractorStyleImage2D::EndMeasure()
{
	if (this->State != VTKIS_MEASURE) {
		return;
	}
	this->InvokeEvent(GinkgoImageCommand::EndMeasureEvent, this);
	this->StopState();
}
//----------------------------------------------------------------------------

void
GinkgoInteractorStyleImage2D::WindowLevel()
{
	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->WindowLevelCurrentPosition[0] = rwi->GetEventPosition()[0];
	this->WindowLevelCurrentPosition[1] = rwi->GetEventPosition()[1];

	int *size = this->View->GetRenderWindow()->GetSize();

	// Compute normalized delta
	double dx = 4.0 *
				(this->GetWindowLevelCurrentPosition()[0] -
				this->GetWindowLevelStartPosition()[0]) / size[0];
	double dy = 4.0 *
				(this->GetWindowLevelStartPosition()[1] -
				this->GetWindowLevelCurrentPosition()[1]) / size[1];

	this->SetWindowStep(dx);
	this->SetLevelStep(dy);

	this->InvokeEvent(vtkCommand::WindowLevelEvent, this);
}

void
GinkgoInteractorStyleImage2D::WindowLevelWheelForward()
{
	int *size = this->View->GetRenderWindow()->GetSize();

	double dy = 4.0 * (double) (this->MouseWheelMotionFactor) / size[1];

	this->SetWindowStep(0.0);
	this->SetLevelStep(dy);

	this->InvokeEvent(vtkCommand::WindowLevelEvent, this);
}

void
GinkgoInteractorStyleImage2D::WindowLevelWheelBackward()
{
	int *size = this->View->GetRenderWindow()->GetSize();

	double dy = 4.0 * (double) (-1.0 * this->MouseWheelMotionFactor) / size[1];

	this->SetWindowStep(0.0);
	this->SetLevelStep(dy);

	this->InvokeEvent(vtkCommand::WindowLevelEvent, this);
}
//----------------------------------------------------------------------------

void GinkgoInteractorStyleImage2D::OnMouseWheelForward()
{

	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	double factor = 0.0;

	this->FindPokedRenderer(x, y);

	if (this->CurrentRenderer == NULL) {
		return;
	}

	switch (this->View->GetWheelInteractionStyle()) {
		/*
		case vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION :
			this->StartWindowLevel();
			this->WindowLevelWheelForward();
			this->EndWindowLevel();
			break;		
		*/
		case vtkGinkgoImageViewer::SELECT_INTERACTION :
			this->StartZSliceMove();
			this->ZSliceWheelForward();
			this->EndZSliceMove();
			break;
		case vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION :
			factor = 10.0 * 0.2 * this->MouseWheelMotionFactor;
			DoZoom(factor);
			break;
		/*
		case vtkGinkgoImageViewer::FULL_PAGE_INTERACTION :

			break;
		*/
		case vtkGinkgoImageViewer::ZOOM_INTERACTION :
			factor = 10.0 * 0.2 * this->MouseWheelMotionFactor;
			DoZoom(factor);
			break;

		default:
			break;
	}

}

void GinkgoInteractorStyleImage2D::OnMouseWheelBackward()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	double factor = 0.0;

	this->FindPokedRenderer(x, y);

	if (this->CurrentRenderer == NULL) {
		return;
	}


	switch (this->View->GetWheelInteractionStyle()) {
		/*
		case vtkGinkgoImageViewer::WINDOW_LEVEL_INTERACTION :
			this->StartWindowLevel();
			this->WindowLevelWheelBackward();
			this->EndWindowLevel();
			break;
		*/
		case vtkGinkgoImageViewer::SELECT_INTERACTION :
			this->StartZSliceMove();
			this->ZSliceWheelBackward();
			this->EndZSliceMove();
			break;
		case vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION :
			factor = 10.0 * -0.2 * this->MouseWheelMotionFactor;
			DoZoom(factor);
			break;
			/*
		case vtkGinkgoImageViewer::FULL_PAGE_INTERACTION :

			break;
		*/
		case vtkGinkgoImageViewer::ZOOM_INTERACTION :
			factor = 10.0 * -0.2 * this->MouseWheelMotionFactor;
			DoZoom(factor);
			break;

		default:
			break;
	}

}

void GinkgoInteractorStyleImage2D::DoZoom(double factor, bool launchTimer)
{
	if (TimerZoom->IsRunning()) {
		TimerZoom->Stop();
	}

	if (this->State != VTKIS_ZOOM) {
		this->StartZoom();
	}
	this->View->SyncSetZoom(pow((double) 1.1, factor) * this->View->GetZoom());
	GetInteractor()->Render();
	
	if (launchTimer) {
		TimerZoom->Start(600, true);
	}
}



