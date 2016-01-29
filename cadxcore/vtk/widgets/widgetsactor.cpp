/*
 *  
 *  $Id: widgetsactor.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *  Code based in VTK
 */
#include <api/autoptr.h>
#include <api/iwidgetsmanager.h>
#include <api/iwidgetsrenderer.h>
#include <api/utils/iginkgomatrix4x4.h>
#include <api/ioverlay.h>

#include "widgetsactor.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorObserver.h>
#include <vtkBox.h>
#include <vtkObjectFactory.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCommand.h>
#include <vtkDataArray.h>
#include <vtkFloatArray.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLRenderer.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkTimerLog.h>
#include <vtkTriangle.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLTexture.h>
#include <vtkImageData.h>
#include <vtkOpenGL.h>
#include <api/iwidgets.h>
#include <vtkgl.h>
#include <vtkDebugLeaks.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <api/controllers/icontroladorcarga.h>


vtkStandardNewMacro(WidgetRepresentation);

//----------------------------------------------------------------------

WidgetRepresentation::WidgetRepresentation()
{
	Enabled = true;
	this->Renderer = NULL;

	this->InteractionState = 0;
	this->StartEventPosition[0] = 0.0;
	this->StartEventPosition[1] = 0.0;
	this->StartEventPosition[2] = 0.0;

	this->PlaceFactor = 0.5;
	this->Placed = 0;
	this->ValidPick = 0;
	this->HandleSize = 0.01;

	this->InitialBounds[0] = this->InitialBounds[2] = this->InitialBounds[4] = 0.0;
	this->InitialBounds[1] = this->InitialBounds[3] = this->InitialBounds[5] = 1.0;

	this->InitialLength = 0.0;

	this->NeedToRender = 0;

	this->CustomActor = WidgetsActor::New();
	this->CustomMapper = WidgetsMapper::New();
	this->CustomActor->SetMapper(CustomMapper);
	this->CustomActor->GetProperty()->SetOpacity(0.5);

}

//----------------------------------------------------------------------

WidgetRepresentation::~WidgetRepresentation()
{
	this->CustomMapper->Delete();
	this->CustomActor->Delete();

}

//----------------------------------------------------------------------

void WidgetRepresentation::SetRenderer(vtkRenderer *ren)
{
	if (ren != this->Renderer) {
		if (this->Renderer != NULL) {
			this->Renderer->RemoveViewProp(static_cast<vtkProp*>(this));
		}
		this->Renderer = ren;
		if (this->Renderer != NULL) {
			this->Renderer->AddViewProp(static_cast<vtkProp*>(this));
		}
		this->Modified();
	}
}

//----------------------------------------------------------------------

void WidgetRepresentation::AdjustBounds(double bounds[6], double newBounds[6],
	double center[3])
{
	center[0] = (bounds[0] + bounds[1]) / 2.0;
	center[1] = (bounds[2] + bounds[3]) / 2.0;
	center[2] = (bounds[4] + bounds[5]) / 2.0;

	newBounds[0] = center[0] + this->PlaceFactor * (bounds[0] - center[0]);
	newBounds[1] = center[0] + this->PlaceFactor * (bounds[1] - center[0]);
	newBounds[2] = center[1] + this->PlaceFactor * (bounds[2] - center[1]);
	newBounds[3] = center[1] + this->PlaceFactor * (bounds[3] - center[1]);
	newBounds[4] = center[2] + this->PlaceFactor * (bounds[4] - center[2]);
	newBounds[5] = center[2] + this->PlaceFactor * (bounds[5] - center[2]);
}

//----------------------------------------------------------------------

double WidgetRepresentation::SizeHandlesInPixels(double factor,
	double pos[3])
{
	//
	int i;
	vtkRenderer *renderer;

	if (!this->ValidPick || !(renderer = this->Renderer) ||
		!renderer->GetActiveCamera()) {
		return (this->HandleSize * factor * this->InitialLength);
	} else {
		double radius, z;
		double lowerLeft[4], upperRight[4];
		double focalPoint[4];

		vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer,
			pos[0], pos[1], pos[2],
			focalPoint);
		z = focalPoint[2];

		double x = focalPoint[0] - this->HandleSize / 2.0;
		double y = focalPoint[1] - this->HandleSize / 2.0;
		vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, x, y, z, lowerLeft);

		x = focalPoint[0] + this->HandleSize / 2.0;
		y = focalPoint[1] + this->HandleSize / 2.0;
		vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, x, y, z, upperRight);

		for (radius = 0.0, i = 0; i < 3; i++) {
			radius += (upperRight[i] - lowerLeft[i]) *
				(upperRight[i] - lowerLeft[i]);
		}
		return (factor * (sqrt(radius) / 2.0));
	}
}

//----------------------------------------------------------------------

double WidgetRepresentation::SizeHandlesRelativeToViewport(double factor,
	double pos[3])
{
	int i;
	vtkRenderer *renderer;

	if (!this->ValidPick || !(renderer = this->Renderer) ||
		!renderer->GetActiveCamera()) {
		return (this->HandleSize * factor * this->InitialLength);
	} else {
		double radius, z;
		double windowLowerLeft[4], windowUpperRight[4];
		double *viewport = renderer->GetViewport();
		int *winSize = renderer->GetRenderWindow()->GetSize();
		double focalPoint[4];

		vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer,
			pos[0], pos[1], pos[2],
			focalPoint);
		z = focalPoint[2];

		double x = winSize[0] * viewport[0];
		double y = winSize[1] * viewport[1];
		vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, x, y, z, windowLowerLeft);

		x = winSize[0] * viewport[2];
		y = winSize[1] * viewport[3];
		vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, x, y, z, windowUpperRight);

		for (radius = 0.0, i = 0; i < 3; i++) {
			radius += (windowUpperRight[i] - windowLowerLeft[i]) *
				(windowUpperRight[i] - windowLowerLeft[i]);
		}

		return (sqrt(radius) * factor * this->HandleSize);
	}
}

//----------------------------------------------------------------------

void WidgetRepresentation::PrintSelf(ostream& os, vtkIndent indent)
{
	//Superclass typedef defined in vtkTypeMacro() found in vtkSetGet.h
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Renderer: " << this->Renderer << "\n";
	os << indent << "Interaction State: " << this->InteractionState << "\n";
	os << indent << "Handle Size: " << this->HandleSize << "\n";
	os << indent << "Need to Render: " << (this->NeedToRender ? "On\n" : "Off\n");
	os << indent << "Place Factor: " << this->PlaceFactor << "\n";
}

double* WidgetRepresentation::GetBounds()
{
	return NULL;
}

void WidgetRepresentation::GetActors(vtkPropCollection *)
{
	;
}

void WidgetRepresentation::GetActors2D(vtkPropCollection *)
{
	;
}

void WidgetRepresentation::GetVolumes(vtkPropCollection *)
{
	;
}

void WidgetRepresentation::ReleaseGraphicsResources(vtkWindow *w)
{
	this->CustomActor->ReleaseGraphicsResources(w);
}

int WidgetRepresentation::RenderOverlay(vtkViewport *v)
{
	return CustomActor->RenderOverlay(v);
	//return 0;
}

int WidgetRepresentation::RenderOpaqueGeometry(vtkViewport *v)
{
	return CustomActor->RenderOpaqueGeometry(v);
	//return 0;
}

int WidgetRepresentation::RenderTranslucentPolygonalGeometry(vtkViewport* v)
{
	return CustomActor->RenderTranslucentPolygonalGeometry(v);
	//return 0;
}

int WidgetRepresentation::RenderVolumetricGeometry(vtkViewport* /*v*/)
{
	return 0;
}

int WidgetRepresentation::HasTranslucentPolygonalGeometry()
{
	return CustomActor->HasTranslucentPolygonalGeometry();
	//return 1;
}

void WidgetRepresentation::SetWidgetsManager(GNC::GCS::IWidgetsManager* pWidgetsManager)
{
	CustomMapper->SetWidgetsManager(pWidgetsManager);
}

void WidgetRepresentation::SetWidgetsRenderer(GNC::GCS::IWidgetsRenderer* pWidgetsRenderer)
{
	CustomMapper->SetWidgetsRenderer(pWidgetsRenderer);
}


vtkStandardNewMacro(WidgetsActor);

WidgetsActor::WidgetsActor()
{
	Modified();
}

WidgetsActor::~WidgetsActor()
{
	if ( this->Property != NULL)
    {
		this->Property->UnRegister(this);
		this->Property = NULL;
    }

	if ( this->BackfaceProperty != NULL)
    {
		this->BackfaceProperty->UnRegister(this);
		this->BackfaceProperty = NULL;
    }

	if (this->Mapper)
    {
		this->Mapper->UnRegister(this);
		this->Mapper = NULL;
    }
	this->SetTexture(NULL);
};

void WidgetsActor::Render(vtkRenderer* ren, vtkMapper* mapper)
{
	//double opacity;
	int param;
	glGetIntegerv(GL_RENDER_MODE, &param);
	if (param == GL_SELECT) {
		return;
	}
	//glGetIntegerv(GL_RENDER_MODE, param);
	//glDepthMask( GL_FALSE );

	// build transformation
	if (!this->IsIdentity) {
		double *mat = this->GetMatrix()->Element[0];
		double mat2[16];
		mat2[0] = mat[0];
		mat2[1] = mat[4];
		mat2[2] = mat[8];
		mat2[3] = mat[12];
		mat2[4] = mat[1];
		mat2[5] = mat[5];
		mat2[6] = mat[9];
		mat2[7] = mat[13];
		mat2[8] = mat[2];
		mat2[9] = mat[6];
		mat2[10] = mat[10];
		mat2[11] = mat[14];
		mat2[12] = mat[3];
		mat2[13] = mat[7];
		mat2[14] = mat[11];
		mat2[15] = mat[15];

		// insert model transformation
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixd(mat2);
	}

	// send a render to the mapper; update pipeline
	mapper->Render(ren, this);

	// pop transformation matrix
	if (!this->IsIdentity) {
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	//glDepthMask(GL_TRUE);
}


int WidgetsActor::GetIsOpaque()
{
  return 0;
}

//----------------------------------------------------------------------------
// This causes the actor to be rendered. It in turn will render the actor's
// property, texture map and then mapper. If a property hasn't been
// assigned, then the actor will create one automatically. Note that a
// side effect of this method is that the visualization network is updated.
int WidgetsActor::RenderOpaqueGeometry(vtkViewport *vp)
{
  int          renderedSomething = 0;
  vtkRenderer* ren = static_cast<vtkRenderer*>(vp);

  if ( !this->Mapper )
  {
	  return 0;
  }

  // is this actor opaque ?
  if (this->GetIsOpaque())
  {

    // render the backface property
    if (this->BackfaceProperty)
	 {
		 this->BackfaceProperty->BackfaceRender(this, ren);
	 }

    this->Render(ren,this->Mapper);
    renderedSomething = 1;
  }

  return renderedSomething;
}

int WidgetsActor::RenderOverlay(vtkViewport *vp)
{

	if ( !this->Mapper )
	{
		return 0;
	}
	vtkRenderer* ren = static_cast<vtkRenderer*>(vp);
	this->Render(ren,this->Mapper);
	return 1;

	//return 0;
}

//-----------------------------------------------------------------------------
int WidgetsActor::RenderTranslucentPolygonalGeometry(vtkViewport* /*vp*/)
{
  int          renderedSomething = 0;
 // vtkRenderer* ren = static_cast<vtkRenderer*>(vp);

  if ( !this->Mapper )
  {
	  return 0;
  }

	/*
  // is this actor opaque ?
  if (!this->GetIsOpaque())
  {
	  // render the backface property
	  if (this->BackfaceProperty)
	  {
		  this->BackfaceProperty->BackfaceRender(this, ren);
	  }

	  this->Render(ren,this->Mapper);

	  renderedSomething = 1;
    }
	 */
  return renderedSomething;
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int WidgetsActor::HasTranslucentPolygonalGeometry()
{
  if ( !this->Mapper )
  {
	  return 0;
  }

  return !this->GetIsOpaque();
}


vtkStandardNewMacro(WidgetsMapper);

WidgetsMapper::WidgetsMapper()
{
	this->ListId = 0;
	this->m_pWManager = NULL;
	this->m_pWRenderer = NULL;
	this->LastWindow = NULL;
}


WidgetsMapper::~WidgetsMapper()
{
	if (this->LastWindow) {
		this->ReleaseGraphicsResources(this->LastWindow);
	}
}


void WidgetsMapper::ReleaseGraphicsResources(vtkWindow* /*win*/)
{
	if (m_pWRenderer == NULL) {
		return;
	}
	this->LastWindow = NULL;
}

void WidgetsMapper::RenderPiece(vtkRenderer *ren, vtkActor *act)
{
	//
	// make sure that we've been properly initialized
	//
	if (ren->GetRenderWindow()->CheckAbortStatus()) {
		return;
	}

	// make sure our window is current
	//ren->GetRenderWindow()->MakeCurrent();

	//
	// if something has changed regenerate colors and display lists
	// if required
	//
	int noAbort = 1;

	// TODO: Reescribir toda esta lógica
	if (this->GetMTime() > this->BuildTime ||
		act->GetProperty()->GetMTime() > this->BuildTime ||
		ren->GetRenderWindow() != this->LastWindow) {
		if (!this->ImmediateModeRendering &&
			!this->GetGlobalImmediateModeRendering()) {
			//this->ReleaseGraphicsResources(ren->GetRenderWindow());
			this->LastWindow = ren->GetRenderWindow();

			//bool redimensionado = false;
			// get a unique display list id
			//this->ListId = glGenLists(1);
			//glNewList(this->ListId, GL_COMPILE);

			noAbort = this->Draw(ren, act);
			//glEndList();

			// Time the actual drawing
			this->Timer->StartTimer();
			//glCallList(this->ListId);
			this->Timer->StopTimer();
		} else {
			this->ReleaseGraphicsResources(ren->GetRenderWindow());
			this->LastWindow = ren->GetRenderWindow();
		}
		if (noAbort) {
			this->BuildTime.Modified();
		}
	}// if nothing changed but we are using display lists, draw it
	else {
		if (!this->ImmediateModeRendering &&
			!this->GetGlobalImmediateModeRendering()) {
			// Time the actual drawing
			this->Timer->StartTimer();
			//glCallList(this->ListId);
			this->Timer->StopTimer();
		}
	}

	// if we are in immediate mode rendering we always
	// want to draw the primitives here
	if (this->ImmediateModeRendering ||
		this->GetGlobalImmediateModeRendering()) {
		// Time the actual drawing
		this->Timer->StartTimer();
		this->Draw(ren, act);
		this->Timer->StopTimer();
	}

	this->TimeToDraw = this->Timer->GetElapsedTime();

	// If the timer is not accurate enough, set it to a small
	// time so that it is not zero
	if (this->TimeToDraw == 0.0) {
		this->TimeToDraw = 0.0001;
	}
}

void WidgetsMapper::Render(vtkRenderer *ren, vtkActor *act)
{
	this->RenderPiece(ren, act);
}

// Draw method for OpenGL.

void ImprimirMatriz(const double matriz[16])
{
	for(int i = 0; i<4; ++i)
	{
		for(int j = 0; j<4; ++j) {
			std::cout << matriz[j*4 + i] << "\t";
		}
		std::cout<< std::endl;
	}
	std::cout<< std::endl;
	std::cout<< std::endl;
}

int WidgetsMapper::Draw(vtkRenderer *aren, vtkActor* /*act*/)
{

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glDisable (GL_ALPHA_TEST);
	glDisable(GL_SCISSOR_TEST);
	//glAlphaFunc (GL_GREATER, 0);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_MODELVIEW);

	vtkGinkgoImageViewer* pViewer = m_pWRenderer->m_pImageViewer;

	double MatrizModelo[16];

	pViewer->GetModelMatrix()->ToDoubleArrayByColumns(MatrizModelo);
	double ip[3] = {0.0, 0.0, (double)pViewer->GetTindex()};
	double wp[4] = {0.0, 0.0, 0.0, 1.0};

	pViewer->UnProyect2D(ip, wp);
	MatrizModelo[12] = wp[0];
	MatrizModelo[13] = wp[1];
	MatrizModelo[14] = wp[2];

	glMultMatrixd(MatrizModelo);
	
	if (m_pWManager != NULL && m_pWRenderer != NULL) {
		m_pWManager->RenderPrivate(m_pWRenderer->m_c);
	}

	glPopAttrib();
	this->UpdateProgress(1.0);
	return aren->GetRenderWindow()->CheckAbortStatus();
}
