/*
 *  Ginkgo CADx Project
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
 *  Code based in VTK
 */
#pragma once
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <api/api.h>
#include <vtkProp.h>
#include <vtkOpenGLActor.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkOpenGLTexture.h>

class vtkRenderer;
class vtkBox;
class WidgetsActor;
class WidgetsMapper;

namespace GNC
{
namespace GCS
{
class IWidgetsManager;
class IWidgetsRenderer;
}
}

class EXTAPI WidgetRepresentation : public vtkProp
{
public:
        // Description:
        // Standard methods for instances of this class.
        void PrintSelf(ostream& os, vtkIndent indent);

        static WidgetRepresentation *New();

        vtkTypeMacro(WidgetRepresentation,vtkProp);

        virtual void SetRenderer(vtkRenderer *ren);
        vtkGetObjectMacro(Renderer,vtkRenderer);
        virtual int GetInteractionState()
        {
                return this->InteractionState;
        }

        vtkSetClampMacro(HandleSize,double,0.001,1000);
        vtkGetMacro(HandleSize,double);

        vtkGetMacro( NeedToRender, int );
        vtkSetClampMacro( NeedToRender, int, 0, 1 );
        vtkBooleanMacro( NeedToRender, int );

        virtual double *GetBounds();
        virtual void GetActors(vtkPropCollection *);
        virtual void GetActors2D(vtkPropCollection *);
        virtual void GetVolumes(vtkPropCollection *);
        virtual void ReleaseGraphicsResources(vtkWindow *);
        virtual int RenderOverlay(vtkViewport *vtkNotUsed(viewport));
        virtual int RenderOpaqueGeometry(vtkViewport *vtkNotUsed(viewport));
        virtual int RenderTranslucentPolygonalGeometry(vtkViewport *vtkNotUsed(viewport));
        virtual int RenderVolumetricGeometry(vtkViewport *vtkNotUsed(viewport));
        virtual int HasTranslucentPolygonalGeometry();

        void SetWidgetsManager(GNC::GCS::IWidgetsManager* pWidgetsManager);
        void SetWidgetsRenderer(GNC::GCS::IWidgetsRenderer* pWidgetRenderer);

protected:
        WidgetRepresentation();
        ~WidgetRepresentation();

        vtkRenderer *Renderer;

        int InteractionState;

        // These are used to track the beginning of interaction with the representation
        // It's dimensioned [3] because some events re processed in 3D.
        double StartEventPosition[3];

        // Instance variable and members supporting suclasses
        double PlaceFactor; // Used to control how widget is placed around bounding box
        int    Placed; // Indicate whether widget has been placed
        void   AdjustBounds(double bounds[6], double newBounds[6], double center[3]);
        double InitialBounds[6]; //initial bounds on place widget (valid after PlaceWidget)
        double InitialLength; //initial length on place widget
        bool   Enabled;

        int    ValidPick; //indicate when valid picks are made

        double HandleSize; //controlling relative size of widget handles
        double SizeHandlesRelativeToViewport(double factor, double pos[3]);
        double SizeHandlesInPixels(double factor,double pos[3]);

        int NeedToRender;

        vtkTimeStamp  BuildTime;

        WidgetsActor*  CustomActor;
        WidgetsMapper* CustomMapper;

private:
        WidgetRepresentation(const WidgetRepresentation&);  //Not implemented
        void operator=(const WidgetRepresentation&);  //Not implemented
};

class EXTAPI WidgetsActor : public vtkOpenGLActor
{
public:
        static WidgetsActor *New();

protected:
        WidgetsActor();
        ~WidgetsActor();

        vtkTypeMacro(WidgetsActor,vtkOpenGLActor);

        void Render(vtkRenderer *ren, vtkMapper *mapper);
        virtual int GetIsOpaque();
        virtual int RenderOpaqueGeometry(vtkViewport *viewport);
        virtual int RenderOverlay(vtkViewport *viewport);
        virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);

        // Description:
        // Does this prop have some translucent polygonal geometry?
        virtual int HasTranslucentPolygonalGeometry();


public:

};


class EXTAPI WidgetsMapper : public vtkPolyDataMapper
{
public:
        static WidgetsMapper *New();

        vtkTypeMacro(WidgetsMapper,vtkPolyDataMapper);

        // Description:
        // Implement superclass render method.
        virtual void RenderPiece(vtkRenderer *ren, vtkActor *a);
        virtual void Render(vtkRenderer *ren, vtkActor *a);

        // Description:
        // Release any graphics resources that are being consumed by this mapper.
        // The parameter window could be used to determine which graphic
        // resources to release.
        void ReleaseGraphicsResources(vtkWindow *);

        // Description:
        // Draw method for OpenGL.
        virtual int Draw(vtkRenderer *ren, vtkActor *a);

        void SetWidgetsManager (GNC::GCS::IWidgetsManager* pManager)
        {
                m_pWManager = pManager;
        }

        virtual void SetWidgetsRenderer(GNC::GCS::IWidgetsRenderer* pRenderer)
        {
                m_pWRenderer = pRenderer;
        }

protected:
        WidgetsMapper();
        ~WidgetsMapper();

        int ListId;
        GNC::GCS::IWidgetsManager*  m_pWManager;
        GNC::GCS::IWidgetsRenderer* m_pWRenderer;

private:
        WidgetsMapper(const WidgetsMapper&);  // Not implemented.
        void operator=(const WidgetsMapper&);  // Not implemented.
};
