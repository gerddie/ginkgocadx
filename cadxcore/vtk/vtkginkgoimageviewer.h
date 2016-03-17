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
 *  Code based in VTK/VTKInria3D
 */
#pragma once

#define REAL_WORLD_IMAGE_POSITION // Use direction cosines to position image plane. Otherwise vtk position is used (identity cosines) and real position is calculated translating and rotating.

#include <vector>
#include <iostream>

#include <api/api.h>

#include <api/istudycontext.h>
#include <api/math/geometry.h>
#include <api/math/geometry3d.h>

#include <vtkObject.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations

class vtkImageData;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkInteractorStyle;
class vtkActor;
class vtkProp;
class vtkScalarsToColors;
class vtkImageActor;
class vtkLookupTable;
class vtkImageReslice;
class vtkAlgorithmOutput;
class vtkDataArray;
class vtkPlane;

class InternalMembers;
namespace GNC
{
namespace GCS
{
class IGinkgoMatrix4x4;
}
}

namespace GNC
{
namespace GCS
{
class IPipelineProperties;
}
}

//endregion


class EXTAPI vtkGinkgoImageViewer : public vtkObject
{
public:

        //--------------------------------------------------------------------
        //region Estructuras de datos

        typedef enum InteractionStyleIds {
                NO_INTERACTION,
                SELECT_INTERACTION,
                ZOOM_WITH_SELECT_INTERACTION,
                ZOOM_INTERACTION
        } InteractionStyleIds;

        typedef enum ConventionIds {
                RADIOLOGIC,
                NEUROLOGIC
        } ConventionIds;

        typedef enum Axis {
                X = 0,
                Y = 1,
                Z = 2
        } Axis;

        typedef enum PlanIds {
                SAGITTAL_ID,
                CORONAL_ID,
                AXIAL_ID,
                NB_PLAN_IDS  // The number of PlanIds
        } PlanIds;

        typedef enum EventIds {
                ViewImagePositionChangeEvent=(vtkCommand::UserEvent+1),
                ViewImageWindowLevelChangeEvent,
                ViewImageZoomChangeEvent
        } EventIds;

        //endregion

        //--------------------------------------------------------------------
        //region Constructores y soporte VTK

        static vtkGinkgoImageViewer* New();
        vtkTypeMacro(vtkGinkgoImageViewer, vtkObject);

        void InitDefaultInteractor();

        //--------------------------------------------------------------------
        //region Entrada y pipeline

        GNC::GCS::IPipelineProperties* IImagePropertiesProxy;

        void PrepareForDelete(void);

        void Initialize();

        void Uninitialize();

        void SetInteraction();

        bool IsInstalledAndInitialized();

        /** Enable or Disable interaction on the view. The Interaction mode is store
        internaly and set at each time the widget is showed. The interaction
        mode cannot be set before the vtkRenderWindowInteractor is initialized. */
        void SetInteractionOff( void );
        void SetInteractionOn( void );

        bool GetInteraction();

        /**
        Detach the view, i.e. add its own children (if any) to its parent's children (if any).
        */
        void Detach (void);

        void ResetCameraClippingRange(bool resetCameraPosition = true);

        void UpdateOrientation();

        void UpdateDisplayExtent();

        void SetInput(const vtkSmartPointer<vtkImageData>& input, bool forzarSetup = false);

        void SetInputConnection(const vtkSmartPointer<vtkAlgorithmOutput>& input, bool forzeSetup = false);

        void RemoveInputs();

        void SetOverlay(const vtkSmartPointer<vtkImageData>& inputOverlay);

        void IntersectarRayo(double wp[4], double vd[3], double pt[3]);

        /** Proyecta un punto en coordenadas mundo a coordenadas imagen (plano con z=currentSlice) **/
        double* Proyect2D(const double wp[4], double ip[2]) const;

        /** Proyecta un punto en coordenadas imagen (plano con z=currentSlice) a coordenadas mundo **/
        double* UnProyect2D(const double ip[3], double wp[4]) const;

        void SetupPipeline(bool forceReload = false);

        vtkSmartPointer<vtkAlgorithmOutput> GetInputConnection();

        vtkSmartPointer<vtkImageData> GetInput();

        vtkSmartPointer<vtkActor> GetImageActor();

        vtkSmartPointer<vtkPlane> GetPlane();

        const GNC::GCS::Vector3D* GetSliceCoordinates();

        long GetImageTexture();

        /*
        void SetMaskImage(vtkSmartPointer<vtkImageData> mask, vtkSmartPointer<vtkLookupTable> lut);

        void RemoveMaskImage();
        */

        /** Set the RenderWindow */
        void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> arg);


        /** Set the Renderer **/
        void SetRenderer(vtkSmartPointer<vtkRenderer> arg);


        /** Attach an interactor to the internal RenderWindow. **/
        void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor>);

        /** Get the vtkRenderWindow associated */
        vtkSmartPointer<vtkRenderWindow> GetRenderWindow();


        /** Get the vtkRenderer associated */
        vtkSmartPointer<vtkRenderer> GetRenderer();


        /** Get the vtkRenderWindow associated */
        vtkSmartPointer<vtkRenderWindowInteractor> GetRenderWindowInteractor();


        /** Add the actor to the first renderer of the render window if exist.
        Do nothing otherwise.*/
        void AddActor(vtkSmartPointer<vtkProp> actor);


        /** remove the actor from the first renderer of the render window if exist.
        Do nothing otherwise.*/
        void RemoveActor(vtkSmartPointer<vtkProp> actor);

        void Reset();

        void SetTindex(int index);
        int GetTindex();

        /** Call the RenderWindow's Render() method. */
        void Render (void);
        void SyncRender (void);

        void Update();

        void UpdateImage();

        //region Propiedades / Estado, to obtain old spaicing, dimensions... test proxy has to be false
        bool GetDimensions( int dimensions[3], bool testProxy = true );
        bool GetSpacing( double spacing[3], bool testProxy = true );
        bool GetOrigin( double origin[3], bool testProxy = true  );
        bool GetBounds( double bounds[6] );

//	bool GetMatrizModelo(double matriz[16]);
        const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>& GetModelMatrix();
        const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>& GetModelMatrixInv();

        int GetNumberOfComponents();
        ///intentar usar estas funciones sólo cuando la imagen este cargada
        vtkDataArray* GetScalars();
        void* GetScalarPointer();
        int GetScalarType();
        vtkSmartPointer<vtkImageData> GetDataObject();
        //endregion

        //--------------------------------------------------------------------
        //region Comportamiento

        /**
        Switch between radiological (left is right and right is left) and
        neurological (left is left and right is right) conventions.
        */
        void SetConventionsToRadiological(void);

        /**
        Switch between radiological (left is right and right is left) and
        neurological (left is left and right is right) conventions.
        */
        void SetConventionsToNeurological(void);

        void SetOrientation(unsigned int p_orientation);

        int GetOrientation();

        /** Copy camera status from other **/
        void CopyCameraStatus(vtkGinkgoImageViewer* other);

        /** Copy camera from other **/
        void CopyCamera(const vtkGinkgoImageViewer* other);

        /** Returns the current camera rotation in radians (-PI/2, PI/2] **/
        double GetCameraRotation() const;

        /** Returns if camera location has changed to Flip image vertically **/
        bool   GetCameraFlipVertical() const;

        /** Returns if camera location has changed to Flip image horizonally **/
        bool   GetCameraFlipHorizontal() const;


        /** Specify the interactor style */
        void SetInteractorStyle(vtkSmartPointer<vtkInteractorStyle> style);


        /** Get the interactor style */
        vtkSmartPointer<vtkInteractorStyle> GetInteractorStyle();

        int GetInteractionStyle();
        void SetInteractionStyle(int type);


        /** Set the background color. Format is RGB, 0 <= R,G,B <=1
        Example: SetBackgroundColor(0.9,0.9,0.9) for grey-white. */
        void SetBackgroundColor(double r, double g, double b);


        /** Show/Hide the annotations. Call UpdateAnnotations after this function. */
        void SetShowAnnotations (bool show);
        bool GetShowAnnotations();

        //--------------------------------------------------------------------
        //region Propiedades

        void SetInterpolationMode(int enable);
        int GetInterpolationMode();

        bool GetWholeExtent(int extent[6]) const;
        bool SetUpdateExtent(int extent[6]);
        bool GetSliceRange(vtkGinkgoImageViewer::Axis axis, int range[2]) const;

        bool GetLinkCameraFocalAndPosition();
        void SetLinkCameraFocalAndPosition(bool link);

        bool GetLinkZoom();
        void SetLinkZoom(bool link);

        void ClearRotationAndFlip(bool update = false);
        void RotateCamera( bool right);
        void RotateCamera( double angle);
        void Flip(bool vertical);


        //region Control: WindowLevel

        void SetWindowLevelFrom(vtkSmartPointer<vtkGinkgoImageViewer> p_view);
        void SetDefaultWindowLevel(float initialWindow, float initialLevel);
        void SetAutoDefaultWindowLevel();

        double GetWindow();
        double GetLevel();

        void SetWindow(double window);
        void SetLevel(double level);
        void SyncSetWindow (double w);
        void SyncSetLevel (double w);

        void ResetToDefaultWindowLevel();

        void SyncSetCameraFocalAndPosition(double* focal, double* pos);
        void GetRelativePositionOfCamera(double focal[3], double pos[3]);
        void SetRelativePositionOfCamera(const double focal[3], const double pos[3]);

        void SetLookupTable(vtkSmartPointer<vtkScalarsToColors> lut, int idLookupTable);
        vtkSmartPointer<vtkScalarsToColors> GetLookupTable();
        int GetIdLookupTable();

        /** Set Image Brightness (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        void SetBrightness(float brightness);

        /** Set Image Contrast (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        void SetContrast(float contrast);

        /** Get Image Brightness (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        float GetBrightness();

        /** Get Image Contrast (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        float GetContrast();

        void ResetBrightnessAndContrast();

        //region Control: Zoom
        void SetInitialParallelScale(double scale);
        /**
        Resets Zoom
        \param maintainZoom Maintain current zoom scale
        \param zoomMode Set the Zoom mode: ZM_CenteredBestFit = 0, ZM_CenteredOriginalSize = 1
        **/
        void ResetZoom(bool mantenerZoom, int zoomMode);

        /** Set the actual zoom factor of the view. */
        void SetZoom(double factor);
        double GetZoom();
        /* Returns the height of one display pixel in world size (mm) */
        double GetZoomFactor();
        void SyncSetZoom(double factor);

        int GetLeftButtonInteractionStyle();
        int GetRightButtonInteractionStyle();
        int GetMiddleButtonInteractionStyle();
        int GetWheelInteractionStyle();


        //--------------------------------------------------------------------
        //region Conversion de coordenadas

        void CoordenadasImagenACoordenadasMundo(const double ip[3], double wp[4]);

        void CoordenadasImagenACoordenadasPixel(const double ip[3], int pp[2]);

        void CoordenadasImagenACoordenadasPixel(GNC::GCS::Vector& vector);

        void CoordenadasMundoACoordenadasImagen(const double wp[3], double ip[3]);

        /** Apply cosines correction if needed **/
        void CoordenadasMundoACoordenadasMundoReal(const double wp[3], double rwp[3]);

        //--------------------------------------------------------------------
        //region Anotaciones
        void ActualizarAnotaciones();

        //region Propiedades geometricas

        void IntersectarRectangulo(double wp0[3], double wp1[3], double ip0[3], double ip1[3]);

        /** Update the annotations. */
        void UpdateAnnotations( void );

        void PrintSelf(ostream& os, vtkIndent indent);

        unsigned int GetConventions();

        /** Reset the camera */
        void ResetCamera();

        vtkSmartPointer<vtkGinkgoImageViewer> GetParent (void) const;

        /**
        Add a child to the list of children. Check if the child is already
        in the list firt.
        */
        void AddChild (vtkSmartPointer<vtkGinkgoImageViewer> p_view);

        //BTX
        void AddChildren (std::list<vtkSmartPointer<vtkGinkgoImageViewer> > p_viewlist);
        //ETX

        /**
        Remove a child form the list of children.
        */
        void RemoveChild (vtkSmartPointer<vtkGinkgoImageViewer> view);

        void RemoveAllChildren (void);

        /**
        This function is called right after setting both Renderer and RenderWindow.
        It allows a class to add actors for instance without knowing when the Renderer
        and RenderWindow are set. For example, vtkGinkgoImageViewer will add the corner annotations
        during the call to the Initialize function.
        */

        //BTX
        std::list < vtkSmartPointer<vtkGinkgoImageViewer> > GetChildren(void) const
        {
                return this->Children;
        }
        //ETX

        void DrawOn();
        void DrawOff();

        void SetIsProcessed(bool processed);
        bool GetIsProcessed();

        bool IsLocked (void)
        {
                return this->GetIsProcessed();
        }

        /**
        Part of the function propagation mechanism, when the function Lock() is
        called, the view does not transmit the function to its children (and does
        not do anything in fact).
        */
        void Lock (void);

        /**
        A call to UnLock() permits to transmit function calls to the view's children.
        */
        void UnLock (void);

        /**
        Returns true if the view has this child in its list.
        */
        bool HasChild (vtkSmartPointer<vtkGinkgoImageViewer>) const;

        /** Set the render link ON or OFF */
        void SetLinkRender(bool linkRender);
        bool GetLinkRender();

        /**force disable shaders...*/
        void InternalEnableShaders(bool enabled);

protected:

        //BTX
        vtkSmartPointer<vtkGinkgoImageViewer>                 Parent;
        std::list< vtkSmartPointer<vtkGinkgoImageViewer> > Children;
        //ETX

        /**
        Set the parent for this view. Internal use only.
        */
        void SetParent (vtkSmartPointer<vtkGinkgoImageViewer> view);

protected:
        vtkGinkgoImageViewer();
        ~vtkGinkgoImageViewer();

        void SetupAnnotations();

        void UpdateCamera();
public:
        void ActualizarImagen();


public:
        std::string ImageSizeData;
        std::string PixelSizeData;
        std::string VoxelSizeData;
        std::string CurrentPositionData;
        std::string CurrentSliceData;
        std::string CurrentPointValueData;
        std::string CurrentPointPositionData;
        std::string CurrentWindowLevelData;

        InternalMembers*  members;

        std::string ambitolog;

        GNC::GCS::Vector3D SliceCoordinates[4];

        GNC::GCS::Vector3D CurrentOrigin;

};
