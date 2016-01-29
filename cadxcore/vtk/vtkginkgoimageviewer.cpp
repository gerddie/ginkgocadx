/*
*  
*  $Id: vtkginkgoimageviewer.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*  Code based in VTK/VTKInria3D
*/
//#define _GINKGO_TRACE
#include <limits>
#include <sstream>
#include <stdlib.h>
#include <cmath>

#include <wx/wxprec.h>

#include <api/globals.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/math/geometry.h>
#include <api/math/geometry3d.h>
#include <api/utils/iginkgomatrix4x4.h>

#include <main/controllers/controladorlog.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include "vtkginkgoimageviewer.h"
#include "vtkopenglginkgotexture.h"
#include "interactor/ginkgointeractorstyleimage2d.h"
#include "command/ginkgoimagecommand.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkAlgorithmOutput.h>
#include <vtkImageData.h>
#include <vtkImageReslice.h>
#include <vtkActor.h>
#include <vtkProperty.h>

#include <vtkImageMapToColors.h>
#include <vtkScalarsToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkAlgorithm.h>
#include <vtkInformation.h>
#include <vtkDebugLeaks.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPlaneSource.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>

#include <VTKInria3D/vtkVISUManagement/vtkLookupTableManager.h>

#include <vtkAlgorithmOutput.h>
#include <vtkDataSetAttributes.h>
#include <vtkExecutive.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkImageDataStreamer.h>
#include <vtkOpenGLTexture.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkPointData.h>

//#define DEBUG_PIPELINE

#include <vtkCellArray.h>
#include <vtkDoubleArray.h>

#define VTK_GINKGO_ZOOM_MIN 0.25f
#define VTK_GINKGO_ZOOM_MAX 200

class vtkGinkgoTexture : public vtkOpenGLTexture {
protected:
	vtkGinkgoTexture() : m_IdTextura(-1),  m_vtkRenderWindow(NULL) {}

	~vtkGinkgoTexture() {}
public:

	static vtkGinkgoTexture *New();

	vtkTypeMacro(vtkGinkgoTexture,vtkOpenGLTexture);

	long GetTextureId() const
	{
		return Index;
	}

	//indica si la textura tiene id (se ha intentado cargar)
	bool TextureDefined() const
	{
		return Index != 0;
	}

	operator long () const
	{
		return Index;
	}

protected:
	long m_IdTextura;

	vtkRenderWindow *m_vtkRenderWindow;
};

vtkStandardNewMacro(vtkGinkgoTexture);

#if defined(DEBUG_PIPELINE)
static vtkIdType pts[6][4]={{0,1,2,3}, {4,5,6,7}, {0,1,5,4}, {1,2,6,5}, {2,3,7,6}, {3,0,4,7}};
#endif

class Pipeline {
public:
	vtkSmartPointer<vtkAlgorithmOutput>             InputConnection;
	vtkSmartPointer<vtkRenderer>                    Renderer;
	vtkSmartPointer<vtkRenderWindow>                RenderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor>      RenderWindowInteractor;

	vtkSmartPointer<vtkInteractorStyle>             InteractorStyle;

	vtkSmartPointer<vtkImageData>                   ImageData;
	vtkSmartPointer<vtkImageData>                   OverlayData;
	vtkSmartPointer<vtkActor>                       ImageActor;
	#if defined(DEBUG_PIPELINE)

	GNC::GCS::Vector3D coords[8];

	vtkSmartPointer<vtkPolyData>                    DebugVolumePolyData;
	vtkSmartPointer<vtkPoints>                      DebugVolumePoints;
	vtkSmartPointer<vtkCellArray>                   DebugVolumePolys;
	vtkSmartPointer<vtkDoubleArray>                 DebugVolumeScalars;

	vtkSmartPointer<vtkPolyDataMapper>              DebugVolumeMapper;
	vtkSmartPointer<vtkActor>                       DebugVolumeActor;
	#endif
	vtkSmartPointer<vtkActor>                       OverlayActor;
	vtkSmartPointer<vtkPlaneSource>                 PlaneSource;
	vtkSmartPointer<vtkProperty>                    PlaneProperty;
	vtkSmartPointer<vtkPlane>                       Plane;
	vtkSmartPointer<vtkGinkgoOpenGLTexture>         Textura;
	vtkSmartPointer<vtkPolyDataMapper>              MapperPlano;
	vtkSmartPointer<vtkGinkgoTexture>               TexturaOverlay;
	vtkSmartPointer<vtkPolyDataMapper>              MapperPlanoOverlay;

	vtkSmartPointer<vtkImageMapToWindowLevelColors> WindowLevel;

	vtkSmartPointer<vtkScalarsToColors>             LookupTable;
	int                                             LookupTableId;

	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> ModelMatrix;
	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> ModelMatrixInv;

	Pipeline(): ModelMatrix(GNC::GCS::IGinkgoMatrix4x4::New()),
		ModelMatrixInv(GNC::GCS::IGinkgoMatrix4x4::New())
	{
		ImageActor      = vtkSmartPointer<vtkActor>::New();

		#if defined(DEBUG_PIPELINE)

		DebugVolumePolyData = vtkSmartPointer<vtkPolyData>::New();
		DebugVolumePoints = vtkSmartPointer<vtkPoints>::New();
		DebugVolumePolys = vtkSmartPointer<vtkCellArray>::New();
		DebugVolumeScalars = vtkSmartPointer<vtkDoubleArray>::New();
		DebugVolumeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		DebugVolumeActor = vtkSmartPointer<vtkActor>::New();
		
		for (int i=0; i<8; i++) DebugVolumePoints->InsertPoint(i, 0, 0, 0);
		for (int i=0; i<6; i++) DebugVolumePolys->InsertNextCell(4, pts[i]);
		for (int i=0; i<8; i++) DebugVolumeScalars->InsertTuple1(i,i);

		DebugVolumePolyData->SetPoints(DebugVolumePoints);
		DebugVolumePolyData->SetPolys(DebugVolumePolys);
		DebugVolumePolyData->GetPointData()->SetScalars(DebugVolumeScalars);
		DebugVolumeMapper->SetInput(DebugVolumePolyData);
		DebugVolumeMapper->SetScalarVisibility(0);
		DebugVolumeActor->SetMapper(DebugVolumeMapper);
		DebugVolumeActor->GetProperty()->SetRepresentationToWireframe();

		#endif

		OverlayActor    = vtkSmartPointer<vtkActor>::New();
		PlaneProperty   = vtkSmartPointer<vtkProperty>::New();

		PlaneProperty->SetAmbient(1.0);
		PlaneProperty->SetAmbientColor(1.0,1.0,1.0);
		PlaneProperty->SetOpacity(1);

		PlaneSource = vtkSmartPointer<vtkPlaneSource>::New();
		PlaneSource->SetXResolution(1);
		PlaneSource->SetYResolution(1);

		Textura            = vtkSmartPointer<vtkGinkgoOpenGLTexture>::New();
		TexturaOverlay     = vtkSmartPointer<vtkGinkgoTexture>::New();
		MapperPlano        = vtkSmartPointer<vtkPolyDataMapper>::New();
		MapperPlanoOverlay = vtkSmartPointer<vtkPolyDataMapper>::New();
		Plane              = vtkSmartPointer<vtkPlane>::New();

		double bounds[6];
		bounds[0] = -0.5;
		bounds[1] =  0.5;
		bounds[2] = -0.5;
		bounds[3] =  0.5;
		bounds[4] = -0.5;
		bounds[5] =  0.5;

		double center[3];
		center[0] = (bounds[0] + bounds[1])/2.0;
		center[1] = (bounds[2] + bounds[3])/2.0;
		center[2] = (bounds[4] + bounds[5])/2.0;

		PlaneSource->SetOrigin(center[0],bounds[2],bounds[4]);
		PlaneSource->SetPoint1(center[0],bounds[3],bounds[4]);
		PlaneSource->SetPoint2(center[0],bounds[2],bounds[5]);

		Textura->SetPremultipliedAlpha(1.0);
		Textura->SetInterpolate(1);
		Textura->RepeatOff();
		TexturaOverlay->SetInterpolate(1);
		TexturaOverlay->RepeatOff();
		TexturaOverlay->MapColorScalarsThroughLookupTableOn();
		vtkLookupTable* tblover = vtkLookupTableManager::GetOverlayLooupTable();
		TexturaOverlay->SetLookupTable(tblover);
		TexturaOverlay->GetLookupTable()->SetRange(0, 1);
		tblover->Delete();

		MapperPlano->SetInputData(vtkPolyData::SafeDownCast(PlaneSource->GetOutput()));
		MapperPlano->ScalarVisibilityOff();
		MapperPlanoOverlay->ScalarVisibilityOff();
		MapperPlanoOverlay->SetInputData(vtkPolyData::SafeDownCast(PlaneSource->GetOutput()));

		WindowLevel = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();

		vtkLookupTable* tbl = vtkLookupTableManager::GetLinearLookupTable();
		LookupTableId = vtkLookupTableManager::LUT_LINEAR;
		LookupTable = tbl;

		tbl->Delete();

		ImageActor->SetMapper(MapperPlano);
		ImageActor->SetTexture(Textura);
		ImageActor->GetProperty()->SetLighting(false);
		ImageActor->PickableOff();
		ImageActor->GetProperty()->SetOpacity(1.0);
		OverlayActor->SetMapper(MapperPlanoOverlay);
		OverlayActor->SetTexture(TexturaOverlay);
		OverlayActor->GetProperty()->SetLighting(false);
		OverlayActor->PickableOff();

		ResetMatrices();
	}

	void ResetMatrices()
	{
		ModelMatrix->Identity();
		ModelMatrixInv->Identity();
	}
};

class Interactuacion
{
public:
	int LeftButtonInteractionStyle;
	int RightButtonInteractionStyle;
	int MiddleButtonInteractionStyle;
	int WheelInteractionStyle;

	Interactuacion() {
		LeftButtonInteractionStyle = RightButtonInteractionStyle = MiddleButtonInteractionStyle = WheelInteractionStyle = vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION;
	}
};

class Comportamiento {
public:
	int  InteractorStyleType;
	bool ShowAnnotations;
	bool ShowDirections;

	::Interactuacion Interactuacion;

	Comportamiento()
	{
		InteractorStyleType = vtkGinkgoImageViewer::ZOOM_INTERACTION;
		ShowAnnotations     = false;
		ShowDirections      = false;
	}

};

class Propiedades {
public:
	unsigned int Orientacion;
	unsigned int Conventions;
	
	GNC::GCS::Vector3D  CameraFocalPoint;
	GNC::GCS::Vector3D  CameraPosition;
	GNC::GCS::Vector3D  CameraViewUp;
	double              ParallelScale;
	bool                Interaction;
	bool                LinkRender;
	bool                LinkCameraFocalAndPosition;
	bool                LinkZoom;

	bool         Modificado;

private:
	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> DirectionCosines;

public:

	Propiedades(): DirectionCosines(GNC::GCS::IGinkgoMatrix4x4::New())
	{
		Orientacion = vtkGinkgoImageViewer::AXIAL_ID;
		Conventions = vtkGinkgoImageViewer::RADIOLOGIC;
		DirectionCosines->Identity();

		Interaction = false;
		LinkRender = false;
		LinkCameraFocalAndPosition = true;
		LinkZoom = true;
		Modificado = true;
		ParallelScale = 1.0;

	}

	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>& GetDirection()
	{
		return DirectionCosines;
	}

	void SetDirection(const GNC::GCS::IGinkgoMatrix4x4* directionCosines)
	{
		DirectionCosines->DeepCopy(directionCosines);
		Modificado = true;
	}

	void SetConventions(unsigned int c)
	{
		if (Conventions != c)
		{
			Conventions = c;
			Modificado = false;
		}
	}

	void ResetModifiedStatus()
	{
		Modificado = false;
	}


};

class Estado {
public:

	bool    Initialized;
	bool    PipelineInstalled;
	bool    FirstRender;


	bool    DefaultWindowLevelSetted;
	double  InitialWindow;
	double  InitialLevel;

	double  RollOffset;
	bool    FlipHorizontal;
	bool    FlipVertical;

	double  InitialParallelScale;

	bool    IsProcessed;

	double  Zoom;

	unsigned int CurrentNumberOfComponents;

	Estado()
	{

		Initialized               = false;
		PipelineInstalled         = false;
		FirstRender               = true;

		DefaultWindowLevelSetted  = false;
		InitialWindow             = InitialLevel = std::numeric_limits<double>::quiet_NaN();

		InitialParallelScale      = 1.0f;

		IsProcessed               = false;

		Zoom                      = 1.0f;
		CurrentNumberOfComponents = 0;
		RollOffset                = 0.0;
		FlipHorizontal            = false;
		FlipVertical              = false;
	}
};

class InternalMembers {
public:

	::Pipeline Pipeline;
	::Comportamiento Comportamiento;
	::Propiedades Propiedades;
	::Estado Estado;
};

vtkStandardNewMacro(vtkGinkgoImageViewer);


vtkGinkgoImageViewer::vtkGinkgoImageViewer() :
		IImagePropertiesProxy(NULL), 
		ImageSizeData(),
		PixelSizeData(),
		VoxelSizeData(),
		CurrentPositionData(),
		CurrentSliceData(),
		CurrentPointValueData(),
		CurrentPointPositionData(),
		CurrentWindowLevelData(),
		members(new InternalMembers()),
		ambitolog("3DView")
{
	InitDefaultInteractor();
}

void vtkGinkgoImageViewer::InitDefaultInteractor()
{
	vtkSmartPointer<GinkgoInteractorStyleImage2D> interactor = vtkSmartPointer<GinkgoInteractorStyleImage2D>::New();
	interactor->SetView(this);

	vtkSmartPointer<GinkgoImageCommand> cbk = vtkSmartPointer<GinkgoImageCommand>::New();
	cbk->SetView(this);
	interactor->AddObserver(vtkCommand::KeyPressEvent, cbk);
	interactor->AddObserver(GinkgoImageCommand::ZoomEvent, cbk);

	SetInteractorStyle(interactor);
}

vtkGinkgoImageViewer::~vtkGinkgoImageViewer()
{
	if (this->GetRenderer()) {
		this->GetRenderer()->RemoveAllViewProps();
	}
	delete members;
}


void vtkGinkgoImageViewer::PrepareForDelete(void)
{
	Uninitialize();
}

/**
This function is called right after setting both Renderer and RenderWindow.
It allows a class to add actors for instance without knowing when the Renderer
and RenderWindow are set. For example, vtkGinkgoImageViewer will add the corner annotations
during the call to the Initialize function.
*/
void vtkGinkgoImageViewer::Initialize(void)
{
	//std::cout << "vtkGinkgoImageViewer::Initialize()" << std::endl;
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	if (e.Initialized) {
		return;
	}

	if (p.Renderer) {
		p.Renderer->SetBackground(0.0f, 0.0f, 0.0f);
	}
	if (p.RenderWindow && p.Renderer) {
		p.RenderWindow->AddRenderer(members->Pipeline.Renderer);
	}

	if (p.RenderWindowInteractor && p.InteractorStyle) {
		p.RenderWindowInteractor->SetInteractorStyle(p.InteractorStyle);
		p.RenderWindowInteractor->SetRenderWindow(p.RenderWindow);
		e.Initialized = true;
		SetInteractionOn();
	}

	if (p.Renderer) {
		p.Renderer->GetActiveCamera()->ParallelProjectionOn();
		p.Renderer->AddActor(p.ImageActor);
		#if defined(DEBUG_PIPELINE)
		p.Renderer->AddActor(p.DebugVolumeActor);
		#endif
		p.Renderer->AddActor(p.OverlayActor);
		p.ImageActor->VisibilityOff();
		#if defined(DEBUG_PIPELINE)
		p.DebugVolumeActor->VisibilityOff();
		#endif
		p.OverlayActor->VisibilityOff();
	}
}

void vtkGinkgoImageViewer::Uninitialize()
{
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	if (!e.Initialized) {
		return;
	}

	if (p.Renderer) {
		p.Renderer->RemoveAllViewProps();
	}

	if (p.RenderWindow && p.Renderer) {
		p.RenderWindow->RemoveRenderer(p.Renderer);
	}

	if (p.RenderWindowInteractor) {
		p.RenderWindowInteractor->SetInteractorStyle(NULL);
		p.RenderWindowInteractor->SetRenderWindow(NULL);
	}

	e.Initialized = false;
	SetInteractionOff();
}

void vtkGinkgoImageViewer::SetInteraction()
{
	Pipeline& p = members->Pipeline;
	if ( p.RenderWindowInteractor ) {
		if ( !members->Propiedades.Interaction ) {
			p.RenderWindowInteractor->SetInteractorStyle(NULL);
		} else {
			if ( GetInteractorStyle() ) {
				p.RenderWindowInteractor->SetInteractorStyle(p.InteractorStyle);
			}
		}
	}
}

bool vtkGinkgoImageViewer::IsInstalledAndInitialized()
{
	Estado&   e = members->Estado;
	Pipeline& p = members->Pipeline;

	if (p.InputConnection != NULL || p.ImageData != NULL) {
		return e.PipelineInstalled && e.Initialized;
	} else {
		return false;
	}
}

void vtkGinkgoImageViewer::SetInteractionOff()
{
	Propiedades& p = members->Propiedades;
	p.Interaction = false;
	this->SetInteraction();
}

void vtkGinkgoImageViewer::SetInteractionOn()
{
	Propiedades& p = members->Propiedades;
	p.Interaction = true;
	this->SetInteraction();
}

bool vtkGinkgoImageViewer::GetInteraction()
{
	Propiedades& p = members->Propiedades;
	return p.Interaction;
}

/**
Detach the view, i.e. add its own children (if any) to its parent's children (if any).
*/
void vtkGinkgoImageViewer::Detach (void)
{
	vtkSmartPointer<vtkGinkgoImageViewer> parent = GetParent();
	if ( parent ) {
		parent->AddChildren( Children );
		parent->RemoveChild(this);
		Parent = NULL;
	}

	RemoveAllChildren();
}

void vtkGinkgoImageViewer::SetInput(const vtkSmartPointer<vtkImageData>& input, bool forzarSetup)
{
	//std::cout << "vtkGinkgoImageViewer::SetInput()" << std::endl;
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;
	p.ImageData = input;
	p.InputConnection = NULL;
	if (e.PipelineInstalled || forzarSetup) {
		SetupPipeline(forzarSetup);
	}
}

void vtkGinkgoImageViewer::SetInputConnection(const vtkSmartPointer<vtkAlgorithmOutput>& input, bool forzarSetup)
{
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	p.InputConnection = input;
	p.ImageData = NULL;

	if (e.PipelineInstalled || forzarSetup) {
		p.InputConnection->GetProducer()->UpdateInformation();
		SetupPipeline(forzarSetup);
	}
}

void vtkGinkgoImageViewer::RemoveInputs()
{
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	e.PipelineInstalled = false;	
	p.InputConnection = NULL;
	p.ImageData = NULL;
	p.ImageActor->VisibilityOff();
	#if defined(DEBUG_PIPELINE)
	p.DebugVolumeActor->VisibilityOff();
	#endif
	p.OverlayActor->VisibilityOff();
	IImagePropertiesProxy = NULL;
}

void vtkGinkgoImageViewer::SetOverlay(const vtkSmartPointer<vtkImageData>& inputOverlay)
{
	//std::cout << "vtkGinkgoImageViewer::SetInputConnection()" << std::endl;
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	p.OverlayData = inputOverlay;
	if (e.PipelineInstalled) {
		if (GetNumberOfComponents() == 1)  { // NC = 1. Antes era 0, 2, 3 o 4
			//overlay???
			if (p.OverlayData != NULL)
			{
				p.TexturaOverlay->SetInputData(p.OverlayData);
				p.TexturaOverlay->Modified();
				p.OverlayActor->Modified();
				p.OverlayActor->VisibilityOn();
			} else {
				p.OverlayActor->VisibilityOff();
			}
		} else {
			p.OverlayActor->VisibilityOff();
		}
	}
}

//====================================================================================================


void vtkGinkgoImageViewer::UpdateDisplayExtent()
{
	Pipeline&    p  = members->Pipeline;
	Estado&      e  = members->Estado;
	Propiedades& pr = members->Propiedades;


	if (!e.Initialized || !e.PipelineInstalled) {
		return;
	}

	GNC::GCS::Vector3D origin;
	GNC::GCS::Vector3D spacing;


	//if we have an input connection we take properties from proxy
	if (p.InputConnection != NULL) {
		if (IImagePropertiesProxy != NULL) {
			IImagePropertiesProxy->UpdateInformation();
			//std::cout << GetTickCount() << " Update direction" << std::endl;
			pr.SetDirection(IImagePropertiesProxy->GetOutputDirection());
			IImagePropertiesProxy->GetOutputOrigin(origin);
			IImagePropertiesProxy->GetOutputSpacing(spacing);
		} else {
			LOG_ERROR("ginkgoImageViewer", "you are tying to load an image from a input connection without properties proxy");
		}
	}
	else {
                // vtk 6 deleted this function, and since we have no InputConnection
                // we must pray that the information doesn't need updating. 
		// p.ImageData->UpdateInformation();
		p.ImageData->GetOrigin(origin);
		p.ImageData->GetSpacing(spacing);
	}

#if 0
	std::cout << "Origin  = " << origin << std::endl;
	std::cout << "Spacing = " << spacing << std::endl;
#endif

	CurrentOrigin = origin;

	p.ResetMatrices();
	
	//build model matrix with directioncosines + spacing + origin

	#if defined(REAL_WORLD_IMAGE_POSITION) 
	p.ModelMatrix->DeepCopy(pr.GetDirection());
	#endif

	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> spacingMatrix(GNC::GCS::IGinkgoMatrix4x4::New());
	spacingMatrix->Identity();
	spacingMatrix->SetElement(0,0, spacing.x);
	spacingMatrix->SetElement(1,1, spacing.y);
	spacingMatrix->SetElement(2,2, spacing.z);

	p.ModelMatrix = p.ModelMatrix->Multiply4x4(spacingMatrix);
	p.ModelMatrix->SetElement(0, 3, origin.x);
	p.ModelMatrix->SetElement(1, 3, origin.y);
	p.ModelMatrix->SetElement(2, 3, origin.z);

	p.ModelMatrixInv = p.ModelMatrix->Invert();

#if 0 && defined(_DEBUG)
	std::cout << "Matriz Modelo:" << std::endl;
	p.ModelMatrix->Print(std::cout);
#endif

#if 0
	std::cout << "Matriz Modelo Invertida:" << std::endl;
	p.ModelMatrixInv->Print(std::cout);
#endif
}

void vtkGinkgoImageViewer::ResetCameraClippingRange(bool resetCameraPosition)
{
	Pipeline& p  = members->Pipeline;
	//Propiedades& pr = members->Propiedades;

	//update planeSource...
	double zpos = 0.0;
	int dimensions[3] = {0, 0, 0};
	if (GetDimensions(dimensions)) {
		//if we have an input connection we take properties from proxy
		if (p.InputConnection != NULL) {
			if (IImagePropertiesProxy != NULL) {
				zpos = IImagePropertiesProxy->GetCurrentSliceIndex();
			}
		}

		double po[4] = {-0.5,                                                    -0.5,      zpos,    1.0};
		double p1[4] = {((double)dimensions[0]) - 0.5,                           -0.5,      zpos,    1.0};
		double p2[4] = {((double)dimensions[0]) - 0.5,  ((double)dimensions[1]) - 0.5,      zpos,    1.0};
		double p3[4] = {-0.5,                           ((double)dimensions[1]) - 0.5,      zpos,    1.0};

		double r[4] = {0.0,0.0,0.0,0.0};
		if ( GNC::GCS::IControladorLog::Instance()->IsEnabledFor(GNC::GCS::IControladorLog::DebugLog) ) {
			LOG_DEBUG(ambitolog, "----- Image Plane Coordinates -----");
			double ptest[4][4]  = { {0.0, 0.0, zpos, 1.0}, {0.0, (double)(dimensions[1]-1), zpos, 1.0}, {(double)(dimensions[0]-1), (double)(dimensions[1]-1), zpos, 1.0}, {(double)(dimensions[0]-1), 0.0, zpos, 1.0} };
			for (int i = 0; i < 4; i++) {
				p.ModelMatrix->MultiplyPoint(ptest[i], r);
				GNC::GCS::Vector3D tptest = r;

				CoordenadasMundoACoordenadasMundoReal(tptest.v, tptest.v);

				LOG_DEBUG(ambitolog, "P(" << ptest[i][0] << "," << ptest[i][1] << "," << ptest[i][2] << ") * M  = " << tptest);
			}
		}

		//points of plane source
		p.ModelMatrix->MultiplyPoint(po, SliceCoordinates[0].v);
		p.ModelMatrix->MultiplyPoint(p1, SliceCoordinates[1].v);		
		p.ModelMatrix->MultiplyPoint(p2, SliceCoordinates[2].v);
		p.ModelMatrix->MultiplyPoint(p3, SliceCoordinates[3].v);

		p.PlaneSource->SetOrigin(SliceCoordinates[0].v);
		p.PlaneSource->SetPoint1(SliceCoordinates[1].v);
		p.PlaneSource->SetPoint2(SliceCoordinates[3].v);
		//z direction cosines is the normal of plane source
		p.PlaneSource->Update();

		p.Plane->SetOrigin( p.PlaneSource->GetOrigin() );
		p.Plane->SetNormal( p.PlaneSource->GetNormal() );

		p.MapperPlano->Modified();
		p.MapperPlanoOverlay->Modified();
		p.ImageActor->Modified();

		if (resetCameraPosition) { // Depending on interaction, could not be desirable to re-position the camera each time (ex: 3D interaction)
			Estado& e = members->Estado;
			GNC::GCS::Vector3D plane_center;
			GNC::GCS::Vector3D plane_normal;
			GNC::GCS::Vector3D plane_origin;
			GNC::GCS::Vector3D plane_p2;

			GNC::GCS::Vector3D camera_position;
			GNC::GCS::Vector3D camera_focal_point;
			GNC::GCS::Vector3D camera_view_up;

			p.PlaneSource->GetCenter(plane_center.v);
			p.PlaneSource->GetNormal(plane_normal.v);
			p.PlaneSource->GetOrigin(plane_origin.v);
			p.PlaneSource->GetPoint2(plane_p2.v);

			camera_focal_point = plane_center;			
			camera_view_up =  (plane_origin - plane_p2).Normalizado();
			camera_position = plane_center - (plane_normal * 1000.0);

			double rotation = e.RollOffset;
			if (IImagePropertiesProxy != NULL) {
				IImagePropertiesProxy->SetCustomOrientation(camera_focal_point, camera_position, camera_view_up, rotation);
			}

			bool hflip = e.FlipHorizontal;

			if (e.FlipVertical) {
				camera_view_up =  (plane_p2 - plane_origin).Normalizado();
				hflip = !hflip;
			}
			if (hflip) {
				camera_position = plane_center + (plane_normal * 1000.0);
			}			

			vtkCamera *cam = p.Renderer ? p.Renderer->GetActiveCamera() : NULL;

			if (cam != NULL) {
				cam->SetFocalPoint(camera_focal_point.v);
				cam->SetPosition(camera_position.v);				
				cam->SetViewUp(camera_view_up.v);
				cam->Roll(rotation);
				cam->OrthogonalizeViewUp();

				//necessary for views synchronization in standard view				
				Propiedades& pr = members->Propiedades;
				pr.CameraFocalPoint = camera_focal_point;
				pr.CameraPosition = camera_position;
				pr.CameraViewUp = camera_view_up;
				p.Renderer->ResetCameraClippingRange();
			}			
		}
		else {
			p.Renderer->ResetCameraClippingRange();
		}
		
	}
}

void vtkGinkgoImageViewer::UpdateOrientation()
{
	Propiedades& pr = members->Propiedades;

	ResetCameraClippingRange(true);

	pr.ResetModifiedStatus();
}


void vtkGinkgoImageViewer::IntersectarRayo(double wp[4], double vd[3], double pt[3])
{
	typedef GNC::GCS::Vector3D TVector;
	if (members != NULL) {
		Pipeline& p = members->Pipeline;
		TVector punto_recta = wp;
		TVector vdir_recta = vd;
		TVector centro_plano;
		TVector vnorm_plano;
		p.Plane->GetOrigin(centro_plano.v);
		p.Plane->GetNormal(vnorm_plano.v);
		TVector interseccion = TVector::InterseccionEntreRectaYPlano(punto_recta, vdir_recta, centro_plano, vnorm_plano);
		pt[0] = interseccion.x;
		pt[1] = interseccion.y;
		pt[2] = interseccion.z;
	}
}

double* vtkGinkgoImageViewer::Proyect2D(const double wp[4], double ip[3]) const
{
	Pipeline& p = members->Pipeline;
	double pt[4] = {0.0, 0.0, 0.0, 1.0};
	p.ModelMatrixInv->MultiplyPoint(wp, pt);
	ip[0] = pt[0];
	ip[1] = pt[1];
	ip[2] = pt[2];
	return ip;
}

double* vtkGinkgoImageViewer::UnProyect2D(const double ip[3], double wp[4]) const
{
	Pipeline& p = members->Pipeline;
	double pt[4] = {0.0, 0.0, 0.0, 1.0};
	pt[0] = ip[0];
	pt[1] = ip[1];
	pt[2] = ip[2];

	if (IImagePropertiesProxy != NULL) {
		pt[2] = (double)IImagePropertiesProxy->GetCurrentSliceIndex();
	}

	p.ModelMatrix->MultiplyPoint(pt, wp);
	return wp;
}

void vtkGinkgoImageViewer::SetupPipeline(bool forceReload)
{
	//GNC::GCS::Timer t;
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	//t.start();

	double oldSpacing[3] = {-1.0,-1.0,-1.0};
	double oldOrigin[3] = {0.0, 0.0, 0.0};
	int oldDimensions[3] = {-1,-1,-1};

	//se consultan las dimensiones y spacing para saber si al final hay que resetear el zoom
	bool mantenerVista = !forceReload;

	if (e.PipelineInstalled) {
		GetSpacing(oldSpacing, false);
		GetOrigin(oldOrigin, false);
		GetDimensions(oldDimensions, false);
	}
	else {
		mantenerVista = false;
	}

	unsigned int nc = 0;

	if (p.ImageData != NULL) {
		//p.ImageData->UpdateInformation();
		nc = p.ImageData->GetNumberOfScalarComponents();
	}
	else if (p.InputConnection != NULL) {
		if (IImagePropertiesProxy != NULL) {
			IImagePropertiesProxy->UpdateInformation();
		}
		p.InputConnection->GetProducer()->UpdateInformation();
		nc = GetNumberOfComponents();
	}
	else {
		//std::cout << "NULL input" << std::endl;
	}

	#if defined(DEBUG_PIPELINE)

	if (IImagePropertiesProxy != NULL) {
		GNC::GCS::Vector3D origin;
		GNC::GCS::Vector3D spacing;
		GNC::GCS::Vector3D dimensions;
		
		IImagePropertiesProxy->GetOrigin(origin);
		IImagePropertiesProxy->GetSpacing(spacing);
		IImagePropertiesProxy->GetFPDimensions(dimensions);
		
		const GNC::GCS::Vector3D pt[8] = {
			GNC::GCS::Vector3D(0.0, 0.0, 0.0), GNC::GCS::Vector3D(1.0, 0.0, 0.0), GNC::GCS::Vector3D(1.0, 1.0, 0.0), GNC::GCS::Vector3D(0.0, 1.0, 0.0),
			GNC::GCS::Vector3D(0.0, 0.0, 1.0), GNC::GCS::Vector3D(1.0, 0.0, 1.0), GNC::GCS::Vector3D(1.0, 1.0, 1.0), GNC::GCS::Vector3D(0.0, 1.0, 1.0)
		};

		GNC::GCS::Vector3D pp;		
		for (int i = 0; i < 8; ++i) {
			IImagePropertiesProxy->GetDirection()->MultiplyPoint( (dimensions * spacing * pt[i]).v, pp.v);
			pp = (origin + pp);
			p.DebugVolumePolyData->GetPoints()->SetPoint(i, pp.v);
		}
		
	}
	#endif

	if (!e.PipelineInstalled) {

		if (nc == 1)  {
			p.Textura->MapColorScalarsThroughLookupTableOn();
			if (p.ImageData != NULL) {
				GNC::GCS::ILocker lock(p.Textura);
				p.Textura->SetInputData(p.ImageData);
			}
			else {
				GNC::GCS::ILocker lock(p.Textura);
				p.Textura->SetInputConnection(p.InputConnection);
			}
			p.Textura->SetLookupTable(p.LookupTable);
			p.Textura->Modified();

			//overlay???
			if (p.OverlayData != NULL)
			{
				p.TexturaOverlay->SetInputData(p.OverlayData);
				p.TexturaOverlay->Modified();
				p.OverlayActor->Modified();
				p.OverlayActor->VisibilityOn();
			} else {
				p.OverlayActor->VisibilityOff();
			}
		}//nc == 1
		else {
			p.Textura->MapColorScalarsThroughLookupTableOff();
			if (p.ImageData != NULL) {
				GNC::GCS::ILocker lock(p.Textura);
				p.Textura->SetInputData(p.ImageData);
			}
			else {
				GNC::GCS::ILocker lock(p.Textura);
				p.Textura->SetInputConnection(p.InputConnection);
			}

			p.Textura->Modified();
			p.OverlayActor->VisibilityOff();
		}
		p.ImageActor->Modified();
		p.ImageActor->VisibilityOn();
		#if defined(DEBUG_PIPELINE)
		if (IImagePropertiesProxy != NULL) {
			p.DebugVolumeActor->Modified();
			p.DebugVolumeActor->VisibilityOn();
		}
		#endif
		e.PipelineInstalled = true;
		e.CurrentNumberOfComponents = nc;
	}
	else {
		if (nc != e.CurrentNumberOfComponents) {
			if (nc == 1)  { // NC = 1. Antes era 0, 2, 3 o 4
				p.Textura->MapColorScalarsThroughLookupTableOn();
				//p.StreamConnector->RemoveAllInputs();
				if (p.ImageData != NULL) {
					GNC::GCS::ILocker lock(p.Textura);
					p.Textura->SetInputData(p.ImageData);
				}
				else {
					GNC::GCS::ILocker lock(p.Textura);
					p.Textura->SetInputConnection(p.InputConnection);
				}
				p.Textura->SetLookupTable(p.LookupTable);
				p.Textura->Modified();
				//overlay???
				if (p.OverlayData != NULL)
				{
					p.TexturaOverlay->SetInputData(p.OverlayData);
					p.TexturaOverlay->Modified();
					p.OverlayActor->Modified();
					p.OverlayActor->VisibilityOn();
				} else {
					p.OverlayActor->VisibilityOff();
				}
			}
			else { // NC = 0, 2, 3 o 4. antes era 1
				p.Textura->MapColorScalarsThroughLookupTableOff();
				p.WindowLevel->RemoveAllInputs();
				p.Textura->SetLookupTable(NULL);

				if (p.ImageData != NULL) {
					GNC::GCS::ILocker lock(p.Textura);
					p.Textura->SetInputData(p.ImageData);
				}
				else {
					GNC::GCS::ILocker lock(p.Textura);
					p.Textura->SetInputConnection(p.InputConnection);
				}
				p.Textura->Modified();

				p.OverlayActor->VisibilityOff();
			}
			e.CurrentNumberOfComponents = nc;
		} else {
			if (nc == 1) {
				p.Textura->MapColorScalarsThroughLookupTableOn();
				if (p.OverlayData != NULL)
				{
					p.TexturaOverlay->SetInputData(p.OverlayData);
					p.TexturaOverlay->Modified();
					p.OverlayActor->Modified();
					p.OverlayActor->VisibilityOn();
				}
				else {
					p.OverlayActor->VisibilityOff();
				}
			} else {
				p.Textura->MapColorScalarsThroughLookupTableOff();
				p.OverlayActor->VisibilityOff();
			}
			if (p.ImageData != NULL) {
				GNC::GCS::ILocker lock(p.Textura);
				p.Textura->SetInputData(p.ImageData);
			}
			else {
				GNC::GCS::ILocker lock(p.Textura);
				p.Textura->SetInputConnection(p.InputConnection);
			}
			p.Textura->Modified();
		}
	}

	//t.stop();
	//std::cout << "vtkGinkgoImageViewer::SetupPipeline(): " << t << std::endl;

	double curSpacing[3] = {0.0,0.0,0.0};
	double curOrigin[3] = {0.0, 0.0, 0.0};
	int curDimensions[3] = {0,0,0};

	GetSpacing(curSpacing);
	GetOrigin(curOrigin);
	GetDimensions(curDimensions);

	for (int i = 0; mantenerVista && i < 3; i++) {
		if ( (curSpacing[i] != oldSpacing[i]) || (curOrigin[i] != oldOrigin[i]) || (curDimensions[i] != oldDimensions[i])) {
			mantenerVista = false;
		}
	}
	if (this->GetTindex() != 0) {
		mantenerVista = false;
	}

	SetTindex(0);

	//save camera position to restore if we have to mantain zoom or zoom and pan
	double focal[3], pos[3];
	GetRelativePositionOfCamera(focal, pos);
	
	UpdateDisplayExtent();
	ResetCameraClippingRange(true);
	//se comprueba si han cambiado las dimensiones o el spacing
	bool mantenerZoom = true;
	{

		for(int i = 0; i < 3 && mantenerZoom; ++i) {
			mantenerZoom = mantenerZoom && oldSpacing[i] == curSpacing[i] && oldDimensions[i] == curDimensions[i];
		}
	}
	ResetZoom(mantenerZoom, 0);
	if (mantenerVista || mantenerZoom) {
		SetRelativePositionOfCamera(focal, pos);
	}
	
}

//====================================================================================================

vtkSmartPointer<vtkAlgorithmOutput> vtkGinkgoImageViewer::GetInputConnection()
{
	Pipeline& p = members->Pipeline;
	return p.InputConnection;
}


vtkSmartPointer<vtkImageData> vtkGinkgoImageViewer::GetInput()
{
	Pipeline& p = members->Pipeline;
	if (p.InputConnection != NULL) {
		return vtkImageData::SafeDownCast(p.Textura->GetInput());
	}
	else if (p.ImageData != NULL) {
		return p.ImageData;
	}
	else {
		return NULL;
	}
}


vtkSmartPointer<vtkActor> vtkGinkgoImageViewer::GetImageActor()
{
	Pipeline& p = members->Pipeline;
	if (p.ImageActor != NULL) {
		return p.ImageActor;
	}
	return NULL;
}

vtkSmartPointer<vtkPlane> vtkGinkgoImageViewer::GetPlane()
{
	Pipeline& p = members->Pipeline;
	if (p.ImageActor != NULL) {
		return p.Plane;
	}
	return NULL;
}

const GNC::GCS::Vector3D* vtkGinkgoImageViewer::GetSliceCoordinates()
{
	return SliceCoordinates;
}

long vtkGinkgoImageViewer::GetImageTexture()
{
	Pipeline& p = members->Pipeline;
	if (p.InputConnection != NULL) {
		return p.Textura->GetTextureId();
	}
	else if (p.ImageData != NULL) {
		return p.Textura->GetTextureId();
	}
	else {
		return -1;
	}
}

/** Set the RenderWindow */
void vtkGinkgoImageViewer::SetRenderWindow(vtkSmartPointer<vtkRenderWindow> rw)
{
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	if (p.RenderWindow == rw) {
		return;
	}

	if (e.Initialized) {
		Uninitialize();
	}

	p.RenderWindow = rw;
	p.RenderWindowInteractor = rw->GetInteractor();

	if (p.RenderWindow && p.Renderer) {
		Initialize();
	}
}


/** Set the Renderer **/
void vtkGinkgoImageViewer::SetRenderer(vtkSmartPointer<vtkRenderer> ren)
{
	Pipeline& p = members->Pipeline;
	Estado&   e = members->Estado;

	if (p.Renderer == ren) {
		return;
	}

	if (e.Initialized) {
		Uninitialize();
	}

	p.Renderer = ren;

	if (p.RenderWindow && p.Renderer) {
		Initialize();
	}
}


/** Attach an interactor to the internal RenderWindow. **/
void vtkGinkgoImageViewer::SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> rwi)
{
	Pipeline& p = members->Pipeline;
	p.RenderWindowInteractor = rwi;
}


/** Get the vtkRenderWindow associated */
vtkSmartPointer<vtkRenderWindow> vtkGinkgoImageViewer::GetRenderWindow()
{
	Pipeline& p = members->Pipeline;
	return p.RenderWindow;
}

/** Get the vtkRenderer associated */
vtkSmartPointer<vtkRenderer> vtkGinkgoImageViewer::GetRenderer()
{
	Pipeline& p = members->Pipeline;
	return p.Renderer;
}

/** Get the vtkRenderWindow associated */
vtkSmartPointer<vtkRenderWindowInteractor> vtkGinkgoImageViewer::GetRenderWindowInteractor()
{
	Pipeline& p = members->Pipeline;
	return p.RenderWindowInteractor;
}

/** Add the actor to the first renderer of the render window if exist.
Do nothing otherwise.*/
void vtkGinkgoImageViewer::AddActor(vtkSmartPointer<vtkProp> actor)
{
	Pipeline& p = members->Pipeline;
	if (p.Renderer)
	{
		p.Renderer->AddActor(actor);
	}
	else
	{
		LOG_ERROR("Viewer", "No se pudo insertar el actor en el pipeline del viewer porque no hay un renderer asociado");
	}

}

/** remove the actor from the first renderer of the render window if exist.
Do nothing otherwise.*/
void vtkGinkgoImageViewer::RemoveActor(vtkSmartPointer<vtkProp> actor)
{
	Pipeline& p = members->Pipeline;
	if (p.Renderer)
	{
		p.Renderer->RemoveActor(actor);
	}
	else
	{
		LOG_ERROR("Viewer", "No se pudo insertar el actor en el pipeline del viewer porque no hay un renderer asociado");
	}
}

void vtkGinkgoImageViewer::Reset()
{
}

void vtkGinkgoImageViewer::SetTindex(int index)
{
	Pipeline& p = members->Pipeline;

	if (p.Textura)
	{
		p.Textura->SetTindex(index);
	}
}

int vtkGinkgoImageViewer::GetTindex()
{
	Pipeline& p = members->Pipeline;
	if (p.Textura)
	{
		return p.Textura->GetTindex();
	}
	return -1;
}

/** Call the RenderWindow's Render() method. */
void vtkGinkgoImageViewer::Render (void)
{
	Pipeline& p = members->Pipeline;
	p.RenderWindow->Render();

}

void vtkGinkgoImageViewer::SyncRender (void)
{
}

void vtkGinkgoImageViewer::Update()
{
}

void vtkGinkgoImageViewer::UpdateImage()
{
}

//region Propiedades / Estado
bool vtkGinkgoImageViewer::GetDimensions( int dimensions[3], bool testProxy )
{
	Pipeline& p  = members->Pipeline;

	int extent[6] = {0, 0, 0, 0, 0, 0};
	dimensions[0] = dimensions[1] = dimensions[2] = 0;
	bool ok = false;

	if (testProxy && IImagePropertiesProxy != NULL) {
		IImagePropertiesProxy->GetOutputDimensions(dimensions);
		return true;
	}

	if (p.InputConnection != NULL) {
		if (p.InputConnection->GetProducer() &&  p.InputConnection->GetProducer()->GetExecutive()) {
			vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
			if (iv && iv->GetNumberOfInformationObjects() > 0) {
				vtkInformation* io = iv->GetInformationObject(0);
				vtkInformationIntegerVectorKey* k = vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT();
				if (io->Has(k)) {
					io->Get(k, extent);
					dimensions[0] = extent[1] + 1;
					dimensions[1] = extent[3] + 1;
					dimensions[2] = extent[5] + 1;
				}
				ok = true;
			}
		}
	}
	else if (p.ImageData != NULL) {
		p.ImageData->GetDimensions(dimensions);
		ok = true;
	}

	return ok;

}

bool vtkGinkgoImageViewer::GetSpacing( double spacing[3] , bool testProxy)
{
	Pipeline& p  = members->Pipeline;

	spacing[0] = spacing[1] = spacing[2] = 1.0f;

	if (testProxy && IImagePropertiesProxy != NULL) {
		IImagePropertiesProxy->GetOutputSpacing(spacing);
		return true;
	}

	bool ok = false;

	if (p.InputConnection != NULL) {

		if (p.InputConnection->GetProducer() &&  p.InputConnection->GetProducer()->GetExecutive()) {
			vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
			if (iv && iv->GetNumberOfInformationObjects() > 0) {
				vtkInformation* io = iv->GetInformationObject(0);
				io->Get(vtkDataObject::SPACING(), spacing);
				ok = true;
			}
		}
	}
	else if (p.ImageData != NULL) {
		p.ImageData->GetSpacing(spacing);
	}

	for (int i = 0; i < 2; ++i) {
		if (spacing[i] <= 0.0f) {
			spacing[i] = 1.0f;
		}
	}

	return ok;
}

bool vtkGinkgoImageViewer::GetOrigin( double origin[3] , bool testProxy)
{
	Pipeline& p  = members->Pipeline;

	origin[0] = origin[1] = origin[2] = 0.0f;

	if (testProxy && IImagePropertiesProxy != NULL) {
		IImagePropertiesProxy->GetOutputOrigin(origin);
		return true;
	}

	bool ok = false;

	if (p.InputConnection != NULL) {
		if (p.InputConnection->GetProducer() &&  p.InputConnection->GetProducer()->GetExecutive()) {
			vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
			if (iv && iv->GetNumberOfInformationObjects() > 0) {
				vtkInformation* io = iv->GetInformationObject(0);
				io->Get(vtkDataObject::ORIGIN(), origin);
				ok = true;
			}
		}
	}
	else if (p.ImageData != NULL) {
		p.ImageData->GetOrigin(origin);
	}
	return ok;
}

bool vtkGinkgoImageViewer::GetBounds( double bounds[6]  )
{
	Estado&   e  = members->Estado;
	Pipeline& p  = members->Pipeline;
	if (e.PipelineInstalled) {

		#if defined(DEBUG_PIPELINE)
		double b1[6];
		double b2[6];
		p.DebugVolumeActor->GetBounds(b1);
		p.ImageActor->GetBounds(b2);
		for (int i = 0, j = 1; i < 6; i+=2, j+=2) {
			bounds[i] = std::min(b1[i], b2[i]);
			bounds[j] = std::max(b1[j], b2[j]);
		}
		return true;
		#else
		double b[3];
		GNC::GCS::Vector3D p1, p2, pt;
		p.PlaneSource->GetOrigin(b);
		bounds[0] = b[0];
		bounds[2] = b[1];
		bounds[4] = b[2];

		p.PlaneSource->GetOrigin(pt.v);
		p.PlaneSource->GetPoint1(p1.v);
		p.PlaneSource->GetPoint2(p2.v);

		p1 = p1 - pt;
		p2 = p2 - pt;

		pt = pt + (p1 + p2);

		bounds[1] = pt.x;
		bounds[3] = pt.y;
		bounds[5] = pt.z;
		return true;
		#endif
		
	}
	else {
		for (int i = 0; i < 6; i++) {
			bounds[i] = 0;
		}
	}
	return false;
}


const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>& vtkGinkgoImageViewer::GetModelMatrix()
{
	Pipeline& p  = members->Pipeline;
	return p.ModelMatrix;
}

const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>& vtkGinkgoImageViewer::GetModelMatrixInv()
{
	Pipeline& p  = members->Pipeline;
	return p.ModelMatrixInv;
}

int vtkGinkgoImageViewer::GetNumberOfComponents()
{
	Pipeline& p  = members->Pipeline;
	int nc = 0;

	if (p.InputConnection != NULL) {
                p.InputConnection->GetProducer()->Update(); 
		vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
		if (iv->GetNumberOfInformationObjects() > 0) {
			vtkInformation* io = iv->GetInformationObject(0);
			vtkInformation* scalarInfo = vtkDataObject::GetActiveFieldInformation(io, vtkImageData::FIELD_ASSOCIATION_POINTS, vtkDataSetAttributes::SCALARS);
			if (scalarInfo && scalarInfo->Has(vtkImageData::FIELD_NUMBER_OF_COMPONENTS()))
			{
				nc = scalarInfo->Get( vtkImageData::FIELD_NUMBER_OF_COMPONENTS() );
			}
		}
	} else if (p.ImageData != NULL) {
		nc = p.ImageData->GetNumberOfScalarComponents();
	}

	return nc;
}

vtkDataArray* vtkGinkgoImageViewer::GetScalars()
{
        vtkSmartPointer<vtkImageData> pImgData = GetDataObject();
        
	if (pImgData == NULL)
		return NULL;

	//pImgData->Update();
	return pImgData->GetPointData()->GetScalars();
}


void* vtkGinkgoImageViewer::GetScalarPointer()
{
	vtkSmartPointer<vtkImageData> pImgData = GetDataObject();
        
	if (pImgData == NULL)
		return NULL;

        return pImgData->GetScalarPointer();
}

int vtkGinkgoImageViewer::GetScalarType()
{
	vtkSmartPointer<vtkImageData> pImgData = GetDataObject();

	if (pImgData == NULL)
		return -1;

	return pImgData->GetScalarType();
}

vtkSmartPointer<vtkImageData> vtkGinkgoImageViewer::GetDataObject()
{
	Pipeline& p  = members->Pipeline;

	vtkSmartPointer<vtkImageData> pImgData = NULL;
	if (p.InputConnection != NULL ) {
		if (p.InputConnection->GetProducer() &&  p.InputConnection->GetProducer()->GetExecutive()) {
                        p.InputConnection->GetProducer()->Update(); 
			//if (p.InputConnection->GetProducer()->GetExecutive()->Update())
			//{
			vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
			if (iv && iv->GetNumberOfInformationObjects() > 0) {
				vtkInformation* io = iv->GetInformationObject(0);
				pImgData = vtkImageData::SafeDownCast(io->Get(vtkDataObject::DATA_OBJECT()));

			}
			//} 
			//else {
			//	pImgData = NULL;
			//}
		}
	} else if (p.ImageData != NULL) {
		pImgData = p.ImageData;
		//pImgData->Update();
	}

	if (pImgData == NULL)
		return NULL;

	return pImgData;
}

//endregion


/**
Switch between radiological (left is right and right is left) and
neurological (left is left and right is right) conventions.
*/

void vtkGinkgoImageViewer::SetConventionsToRadiological(void)
{
	Propiedades& pr = members->Propiedades;
	pr.Conventions = vtkGinkgoImageViewer::RADIOLOGIC;
	UpdateCamera();
}

/**
Switch between radiological (left is right and right is left) and
neurological (left is left and right is right) conventions.
*/
void vtkGinkgoImageViewer::SetConventionsToNeurological(void)
{
	Propiedades& pr = members->Propiedades;
	pr.Conventions = vtkGinkgoImageViewer::NEUROLOGIC;
	UpdateCamera();
}

int vtkGinkgoImageViewer::GetOrientation()
{
	return members->Propiedades.Orientacion;
}


double vtkGinkgoImageViewer::GetCameraRotation() const
{
	const Estado& e= members->Estado;
	return M_PI * (e.RollOffset / 180.0);
}

bool vtkGinkgoImageViewer::GetCameraFlipVertical() const
{
	const Estado& e= members->Estado;
	return e.FlipVertical;
}

bool vtkGinkgoImageViewer::GetCameraFlipHorizontal() const
{
	const Estado& e= members->Estado;
	return e.FlipHorizontal;
}

void vtkGinkgoImageViewer::SetOrientation(unsigned int orientation)
{
	if (orientation != members->Propiedades.Orientacion) {
		members->Propiedades.Orientacion = orientation;
		UpdateCamera();
	}
}

/** Specify the interactor style */
void vtkGinkgoImageViewer::SetInteractorStyle(vtkSmartPointer<vtkInteractorStyle> style)
{
	Pipeline& p = members->Pipeline;
	p.InteractorStyle = style;
}

/** Get the interactor style */
vtkSmartPointer<vtkInteractorStyle> vtkGinkgoImageViewer::GetInteractorStyle()
{
	Pipeline& p = members->Pipeline;
	return p.InteractorStyle;
}

int vtkGinkgoImageViewer::GetInteractionStyle()
{
	Comportamiento& c = members->Comportamiento;
	return c.InteractorStyleType;
}

void vtkGinkgoImageViewer::SetInteractionStyle(int type)
{
	//Pipeline& p = members->Pipeline;

	switch (type) {
		case ZOOM_WITH_SELECT_INTERACTION:
			//p.InteractorStyle = NULL;
			break;
		default:
			break;
	}

}

/** Set the background color. Format is RGB, 0 <= R,G,B <=1
Example: SetBackgroundColor(0.9,0.9,0.9) for grey-white. */
void vtkGinkgoImageViewer::SetBackgroundColor(double /*r*/, double /*g*/, double /*b*/)
{
}

/** Show/Hide the annotations. Call UpdateAnnotations after this function. */
void vtkGinkgoImageViewer::SetShowAnnotations (bool show)
{
	members->Comportamiento.ShowAnnotations = show;
}

bool vtkGinkgoImageViewer::GetShowAnnotations()
{
	return members->Comportamiento.ShowAnnotations;
}

//--------------------------------------------------------------------
//region Propiedades
void vtkGinkgoImageViewer::SetInterpolationMode(int enable)
{
	Pipeline& p  = members->Pipeline;
	p.Textura->SetInterpolate(enable);
	p.TexturaOverlay->SetInterpolate(enable);
}

int vtkGinkgoImageViewer::GetInterpolationMode()
{
	Pipeline& p  = members->Pipeline;
	return p.Textura->GetInterpolate();
}

bool vtkGinkgoImageViewer::GetWholeExtent(int extent[6]) const
{

	Pipeline& p  = members->Pipeline;
	memset(extent, 0, 6 * sizeof(int));

	bool ok = false;

	if (p.InputConnection != NULL) {
		if (p.InputConnection->GetProducer() &&  p.InputConnection->GetProducer()->GetExecutive()) {
			vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
			if (iv && iv->GetNumberOfInformationObjects() > 0) {
				vtkInformation* io = iv->GetInformationObject(0);
				io->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
				ok = true;
			}
		}
	}
	else if (p.ImageData != NULL)
	{
                // essentially pray
		p.ImageData->GetExtent(extent);
	}

	return ok;
}

bool vtkGinkgoImageViewer::SetUpdateExtent(int extent[6])
{

	Pipeline& p  = members->Pipeline;

	bool ok = false;

	if (p.InputConnection != NULL) {
		if (p.InputConnection->GetProducer() &&  p.InputConnection->GetProducer()->GetExecutive()) {
                        p.InputConnection->GetProducer()->SetUpdateExtent(extent); 
			vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
			if (iv && iv->GetNumberOfInformationObjects() > 0) {
				vtkInformation* io = iv->GetInformationObject(0);
				io->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent, 6);
				ok = true;
			}
		}
	}

	return ok;
}

bool vtkGinkgoImageViewer::GetSliceRange(vtkGinkgoImageViewer::Axis axis, int range[2]) const
{
	int extent[6] = {0, 0, 0, 0, 0, 0};
	bool ok = false;
	int axis_offset = (axis >> 1);
	ok = GetWholeExtent(extent);
	range[0] = extent[axis_offset];
	range[1] = extent[axis_offset + 1];
	return ok;
}

bool vtkGinkgoImageViewer::GetLinkCameraFocalAndPosition()
{
	return true;
}

void vtkGinkgoImageViewer::SetLinkCameraFocalAndPosition(bool link)
{
	Propiedades& p = members->Propiedades;
	p.LinkCameraFocalAndPosition = link;
}

bool vtkGinkgoImageViewer::GetLinkZoom()
{
	Propiedades& p = members->Propiedades;
	return p.LinkZoom;
}

void vtkGinkgoImageViewer::SetLinkZoom(bool link)
{
	Propiedades& p = members->Propiedades;
	p.LinkZoom = link;
}

void vtkGinkgoImageViewer::CopyCameraStatus(vtkGinkgoImageViewer* w)
{
	if (w == this || w == NULL) {
		return;
	}
	else {
		Estado& e        = members->Estado;
		Estado& eo       = w->members->Estado;
		e.RollOffset     = eo.RollOffset;
		e.FlipHorizontal = eo.FlipHorizontal;
		e.FlipVertical   = eo.FlipVertical;

		double focal[3], pos[3];
		w->GetRelativePositionOfCamera(focal, pos);
		this->SetRelativePositionOfCamera(focal,pos);
	}
}

void vtkGinkgoImageViewer::CopyCamera(const vtkGinkgoImageViewer* w)
{
	if (w == this || w == NULL) {
		return;
	}
	else {
		Pipeline& p            = members->Pipeline;
		const Pipeline& po     = w->members->Pipeline;

		Propiedades& pr        = members->Propiedades;
		const Propiedades& pro = w->members->Propiedades;

		pr.CameraPosition       = pro.CameraPosition;
		pr.CameraFocalPoint     = pro.CameraFocalPoint;
		pr.CameraViewUp         = pro.CameraViewUp;
		pr.ParallelScale        = pro.ParallelScale;

		vtkCamera *c        = p.Renderer->GetActiveCamera();
		vtkCamera *co = po.Renderer->GetActiveCamera();

		double tmp[3];
		co->GetPosition(tmp);
		c->SetPosition(tmp);

		co->GetFocalPoint(tmp);
		c->SetFocalPoint(tmp);

		co->GetViewUp(tmp);
		c->SetViewUp(tmp);

		c->SetParallelScale(co->GetParallelScale());
	}
}

void vtkGinkgoImageViewer::ClearRotationAndFlip(bool update)
{
	Estado& e = members->Estado;
	e.RollOffset = 0;
	e.FlipVertical = false;
	e.FlipHorizontal = false;

	if (update) {
		Propiedades& pr = members->Propiedades;
		ResetCameraClippingRange(true);
		pr.ResetModifiedStatus();
	}
}

void vtkGinkgoImageViewer::RotateCamera( bool right)
{
	Estado& e = members->Estado;

	right? e.RollOffset -= 90 : e.RollOffset += 90;

	if (e.RollOffset > 180.0) {
		e.RollOffset -= 360.0;
	}
	else if (e.RollOffset <= -180.0) {
		e.RollOffset += 360.0;
	}

	UpdateOrientation();
}

void vtkGinkgoImageViewer::RotateCamera( double angle)
{
	Estado& e = members->Estado;

	e.RollOffset += angle;

	if (e.RollOffset > 180.0) {
		e.RollOffset -= 360.0;
	}
	else if (e.RollOffset <= -180.0) {
		e.RollOffset += 360.0;
	}

	//update has to be call UpdateOrientation();
}

void vtkGinkgoImageViewer::Flip(bool vertical)
{
	Estado& e = members->Estado;
	//if its rolled between 45 and 135 or between -45 and -135 vertical means horizontal and horizontal vertical
	if ( (e.RollOffset > 45 && e.RollOffset <= 135)
		|| (e.RollOffset < -45 && e.RollOffset >= -135) ) 
	{
		if (vertical) {
			e.FlipHorizontal = !e.FlipHorizontal;
		}
		else {
			e.FlipVertical = !e.FlipVertical;
		}
	}
	else 
	{
		if (vertical) {
		e.FlipVertical = !e.FlipVertical;
		}
		else {
			e.FlipHorizontal = !e.FlipHorizontal;
		}
	}
	UpdateOrientation();
}

void vtkGinkgoImageViewer::SetWindowLevelFrom(vtkSmartPointer<vtkGinkgoImageViewer> /*p_view*/)
{
}

void vtkGinkgoImageViewer::SetDefaultWindowLevel(float initialWindow, float initialLevel)
{
	Estado& e = members->Estado;
	e.InitialWindow = initialWindow;
	e.InitialLevel = initialLevel;
	e.DefaultWindowLevelSetted = true;
	ResetToDefaultWindowLevel();
}

void vtkGinkgoImageViewer::SetAutoDefaultWindowLevel()
{
	Pipeline& p  = members->Pipeline;

	vtkImageData* pImgData = NULL;
	if (p.InputConnection != NULL) {
		if (p.InputConnection->GetProducer() &&  p.InputConnection->GetProducer()->GetExecutive()) {
                        p.InputConnection->GetProducer()->Update(); 
			vtkInformationVector* iv = p.InputConnection->GetProducer()->GetExecutive()->GetOutputInformation();
			if (iv && iv->GetNumberOfInformationObjects() > 0) {
				vtkInformation* io = iv->GetInformationObject(0);
				pImgData = vtkImageData::SafeDownCast(io->Get(vtkDataObject::DATA_OBJECT()));
			}
		}
	} else if (p.ImageData != NULL) {
		pImgData = p.ImageData;
	}

	if (pImgData == NULL) {
		return;
	}

	if( pImgData->GetNumberOfScalarComponents()==3 || pImgData->GetNumberOfScalarComponents()==4 )
	{
		return;
	}

	double range[2];
	// TODO PETA
        // vtk6 doesn't have these 
	//pImgData->UpdateInformation();
	//pImgData->UpdateData();
	pImgData->GetScalarRange(range);
	//std::cout << range[0] << ", " << range[1] << std::endl;

	double window = range[1]-range[0];
	double level = 0.5*(range[1]+range[0]);

	//no tenemos shift/scale
	/*window = (window-this->GetShift())/this->GetScale();
	level = (level-this->GetShift())/this->GetScale();*/

	Estado& e = members->Estado;
	e.InitialWindow = window;
	e.InitialLevel = level;
	e.DefaultWindowLevelSetted = true;

	//se establece
	SyncSetWindow(e.InitialWindow);
	SyncSetLevel(e.InitialLevel);
	ActualizarAnotaciones();
}

double vtkGinkgoImageViewer::GetWindow()
{
	Pipeline& p = members->Pipeline;
	return p.WindowLevel->GetWindow();
}

double vtkGinkgoImageViewer::GetLevel()
{
	Pipeline& p = members->Pipeline;
	return p.WindowLevel->GetLevel();
}

void vtkGinkgoImageViewer::SetWindow(double window)
{
	Pipeline& p = members->Pipeline;
	p.WindowLevel->SetWindow(window);

	//rango del window/level
	if (members->Pipeline.LookupTable) {
		double v_min = this->GetLevel() - 0.5 * this->GetWindow();
		double v_max = this->GetLevel() + 0.5 * this->GetWindow();

		members->Pipeline.LookupTable->SetRange(v_min,v_max);
	}
}

void vtkGinkgoImageViewer::SetLevel(double level)
{
	Pipeline& p = members->Pipeline;
	p.WindowLevel->SetLevel(level);

	//rango del window/level
	if (members->Pipeline.LookupTable) {
		double v_min = this->GetLevel() - 0.5 * this->GetWindow();
		double v_max = this->GetLevel() + 0.5 * this->GetWindow();

		members->Pipeline.LookupTable->SetRange(v_min,v_max);
	}
}

void vtkGinkgoImageViewer::SyncSetWindow (double w)
{
	SetWindow(w);
}

void vtkGinkgoImageViewer::SyncSetLevel (double w)
{
	SetLevel(w);
}

void vtkGinkgoImageViewer::ResetToDefaultWindowLevel()
{
	Estado& e = members->Estado;
	if (e.DefaultWindowLevelSetted) {
		SyncSetWindow(e.InitialWindow);
		SyncSetLevel(e.InitialLevel);
		//Modified();
		ActualizarAnotaciones();
	}
	else {
		this->SetAutoDefaultWindowLevel();
	}
}



void vtkGinkgoImageViewer::SyncSetCameraFocalAndPosition(double* /*focal*/, double* /*pos*/)
{
	//propagate event
	this->InvokeEvent (vtkGinkgoImageViewer::ViewImagePositionChangeEvent, NULL);
}

void vtkGinkgoImageViewer::GetRelativePositionOfCamera(double focal[3], double pos[3])
{	
	//relative position of camera
	Propiedades& pr = members->Propiedades;
	Pipeline& p = members->Pipeline;

	vtkCamera *camera = p.Renderer->GetActiveCamera();
	double focal_c[3], pos_c[3];
	camera->GetPosition(pos_c);
	camera->GetFocalPoint(focal_c);

	for (int i = 0; i < 3; i++) {
		focal[i] = pr.CameraFocalPoint[i] - focal_c[i];
		pos[i] = pr.CameraPosition[i] - pos_c[i];
	}
}

void vtkGinkgoImageViewer::SetRelativePositionOfCamera(const double focal[3], const double pos[3])
{
	Propiedades& pr = members->Propiedades;
	Pipeline& p = members->Pipeline;

	double focal_c[3], pos_c[3];
	for (int i = 0; i < 3; i++) {
		focal_c[i] = pr.CameraFocalPoint[i] - focal[i];
		pos_c[i] = pr.CameraPosition[i] - pos[i];
	}

	vtkCamera *camera = p.Renderer->GetActiveCamera();
	camera->SetPosition(pos_c);
	camera->SetFocalPoint(focal_c);
}

void vtkGinkgoImageViewer::SetLookupTable(vtkSmartPointer<vtkScalarsToColors> lut, int idLookupTable)
{
	if (!lut) {
		return;
	}
	double v_min = this->GetLevel() - 0.5 * this->GetWindow();
	double v_max = this->GetLevel() + 0.5 * this->GetWindow();

	lut->SetRange(v_min,v_max);


	vtkLookupTable* realLut = vtkLookupTable::SafeDownCast(lut);

	if (!realLut) {
		std::cerr << "Error: Cannot cast vtkScalarsToColors to vtkLookupTable." << std::endl;
		return;
	}

	/**
	Due to the same problem as above (shift/scale), one must copy the lut
	so that it does not change values of the shared object.
	*/
	vtkLookupTable* newLut = vtkLookupTable::New();
	newLut->DeepCopy(realLut);
	newLut->SetRange(v_min, v_max);
	members->Pipeline.Textura->SetLookupTable(newLut);
	members->Pipeline.LookupTable = newLut;
	members->Pipeline.LookupTableId = (vtkLookupTableManager::LookupTableIds) idLookupTable;
	newLut->Delete();
}

vtkSmartPointer<vtkScalarsToColors> vtkGinkgoImageViewer::GetLookupTable()
{
	return members->Pipeline.LookupTable;
}

int vtkGinkgoImageViewer::GetIdLookupTable()
{
	return members->Pipeline.LookupTableId;
}

/** Set Image Brightness (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
void vtkGinkgoImageViewer::SetBrightness(float brightness)
{
	members->Pipeline.Textura->SetBrightness(brightness);
}

/** Set Image Contrast (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
void vtkGinkgoImageViewer::SetContrast(float contrast)
{
	members->Pipeline.Textura->SetContrast(contrast);
}

/** Get Image Brightness (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
float vtkGinkgoImageViewer::GetBrightness()
{
	return members->Pipeline.Textura->GetBrightness();
}

/** Get Image Contrast (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
float vtkGinkgoImageViewer::GetContrast()
{
	return members->Pipeline.Textura->GetContrast();
}

void vtkGinkgoImageViewer::ResetBrightnessAndContrast()
{
	members->Pipeline.Textura->ResetBrightnessAndContrast();
}

//region Control: Zoom
void vtkGinkgoImageViewer::ResetZoom(bool mantenerZoom, int zoomMode)
{
	Pipeline& p = members->Pipeline;
	Estado& e = members->Estado;
	Propiedades& pr = members->Propiedades;

	double bounds[6];
	p.Renderer->ComputeVisiblePropBounds(bounds);
	p.Renderer->ResetCamera(members->Pipeline.ImageActor->GetBounds());
	double w1 = bounds[1] - bounds[0];
	double w2 = bounds[3] - bounds[2];
	double w3 = bounds[5] - bounds[4];
	double radius = 0.5 * (std::max(w1, std::max(w2, w3)));

	vtkCamera *camera = p.Renderer->GetActiveCamera();

	camera->SetParallelScale(radius);

	double focal_c[3], pos_c[3];
	camera->GetPosition(pos_c);
	camera->GetFocalPoint(focal_c);
	//save initial focal point and position
	pr.CameraFocalPoint[0] = focal_c[0];
	pr.CameraFocalPoint[1] = focal_c[1];
	pr.CameraFocalPoint[2] = focal_c[2];

	pr.CameraPosition[0] = pos_c[0];
	pr.CameraPosition[1] = pos_c[1];
	pr.CameraPosition[2] = pos_c[2];

	e.InitialParallelScale = radius;

	if (mantenerZoom) {
		SetZoom(e.Zoom);
	} else {
		e.Zoom = 1.0;
		if (zoomMode == 1) {

			double wPunto[2][4]    = {{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};
			double wPPunto[2][4]    = {{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};
			double cdir[3]       = {0.0, 0.0, 0.0};         // Vector director de la camara
			double iPunto[2][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}}; //origen del plano

			camera->GetDirectionOfProjection(cdir);			

			//>>World
			vtkInteractorObserver::ComputeDisplayToWorld(p.Renderer, p.RenderWindow->GetSize()[0] >> 1 , p.RenderWindow->GetSize()[1] >> 1, 0, wPunto[0]);
			vtkInteractorObserver::ComputeDisplayToWorld(p.Renderer, (p.RenderWindow->GetSize()[0] >> 1) + 1 , (p.RenderWindow->GetSize()[1] >> 1) + 1, 0, wPunto[1]);
			//>>plano imagen
			IntersectarRayo(wPunto[0], cdir, wPPunto[0]);
			IntersectarRayo(wPunto[1], cdir, wPPunto[1]);
			wPPunto[0][3] = 1.0;
			wPPunto[1][3] = 1.0;
			//>>Imagen
			Proyect2D(wPPunto[0], iPunto[0]);
			Proyect2D(wPPunto[1], iPunto[1]);

			double relX = std::abs( (iPunto[1][0] - iPunto[0][0]) );
			double relY = std::abs( (iPunto[1][1] - iPunto[0][1]) );

			SetZoom( (relX + relY) / 2.0);
		}
	}
}

void vtkGinkgoImageViewer::SetInitialParallelScale(double scale)
{
	Estado& e = members->Estado;
	e.InitialParallelScale = scale;
	SetZoom(e.Zoom);
}


/** Set the actual zoom factor of the view. */
void vtkGinkgoImageViewer::SetZoom(double factor)
{
	Pipeline& p = members->Pipeline;
	Estado& e = members->Estado;

	//se satura
#ifndef _GINKGO_DEBUG
	factor = std::max(factor,(double)VTK_GINKGO_ZOOM_MIN);
	factor = std::min(factor,(double)VTK_GINKGO_ZOOM_MAX);
#endif

	if (p.Renderer == NULL) {
		return;
	}


	if( this->IsLocked() )
	{
		return;
	}

	this->Lock();


	for( std::list< vtkSmartPointer<vtkGinkgoImageViewer> >::iterator it = this->Children.begin(); it != this->Children.end(); ++it)
	{

		vtkGinkgoImageViewer* view = vtkGinkgoImageViewer::SafeDownCast ( *it );

		if( view && view->GetLinkZoom() )
		{
			view->SyncSetZoom (factor);
			if( !view->GetRenderWindow()->GetNeverRendered() )
			{
				view->Render();
			}
		}
	}
	this->UnLock();


	vtkCamera *camera = p.Renderer->GetActiveCamera();

	e.Zoom = factor;

	camera->SetParallelScale(e.InitialParallelScale / e.Zoom);

	//se invoca el evento
	this->InvokeEvent (vtkGinkgoImageViewer::ViewImageZoomChangeEvent, NULL);
	/*
	if (p.RenderWindowInteractor->GetLightFollowCamera()) {
		p.Renderer->UpdateLightsGeometryToFollowCamera();
	}
	*/
}

double vtkGinkgoImageViewer::GetZoom()
{
	Estado& e = members->Estado;
	return e.Zoom;
}

double vtkGinkgoImageViewer::GetZoomFactor()
{
	Pipeline& p = members->Pipeline;
	//Estado& e = members->Estado;

	return p.Renderer->GetActiveCamera()->GetParallelScale() / (double) p.RenderWindow->GetSize()[1];
}

void vtkGinkgoImageViewer::SyncSetZoom(double factor)
{
	SetZoom(factor);
}

int vtkGinkgoImageViewer::GetLeftButtonInteractionStyle()
{
	Comportamiento& c = members->Comportamiento;
	Interactuacion& i = c.Interactuacion;
	return i.LeftButtonInteractionStyle;
}

int vtkGinkgoImageViewer::GetRightButtonInteractionStyle()
{
	Comportamiento& c = members->Comportamiento;
	Interactuacion& i = c.Interactuacion;
	return i.RightButtonInteractionStyle;
}

int vtkGinkgoImageViewer::GetMiddleButtonInteractionStyle()
{
	Comportamiento& c = members->Comportamiento;
	Interactuacion& i = c.Interactuacion;
	return i.MiddleButtonInteractionStyle;
}

int vtkGinkgoImageViewer::GetWheelInteractionStyle()
{
	Comportamiento& c = members->Comportamiento;
	Interactuacion& i = c.Interactuacion;
	return i.WheelInteractionStyle;
}

//--------------------------------------------------------------------
//region Conversion de coordenadas

void vtkGinkgoImageViewer::CoordenadasImagenACoordenadasMundo(const double ip[3], double wp[3])
{
	double pointImagen[4] = {0.0, 0.0, 0.0, 1.0};
	pointImagen[0] = ip[0];
	pointImagen[1] = ip[1];
	pointImagen[2] = ip[2];

	Pipeline& p = members->Pipeline;
	double r[4];
	p.ModelMatrix->MultiplyPoint(pointImagen,r);

#if 0
	double pt[4] = {0.0, 0.0, 0.0, 0.0};

	for (int i = 0; i < 4; i++) {
		pt[i] = r[i];
	}
#endif

	//transformacion homogeneas => no homogeneas
	if (r[3] == 1.0) {
		wp[0] = r[0];
		wp[1] = r[1];
		wp[2] = r[2];
	}
	else if (r[3]) {
		wp[0] = r[0]/r[3];
		wp[1] = r[1]/r[3];
		wp[2] = r[2]/r[3];
	}
}


void vtkGinkgoImageViewer::CoordenadasMundoACoordenadasImagen(const double wp[3], double ip[3])
{
	vtkSmartPointer<vtkImageData> input = GetInput();
	if (!input)
	{
		return;
	}

	const double* spacing = input->GetSpacing();
	const double* origin = input->GetOrigin();

	ip[0] = (wp[0] - origin[0]) / spacing[0];
	ip[1] = ( wp[1] - origin[1]) / spacing[1]; 
	ip[2] = (wp[2] - origin[2]) / spacing[2];
}

void vtkGinkgoImageViewer::CoordenadasImagenACoordenadasPixel(const double ip[3], int pp[3])
{
	//Transforma a coordenadas pixel para usar con vtk (un 0.5 del desplazamiento y otro para hacer un round)
	pp[0] = floor(ip[0] + 0.5);
	pp[1] = floor(ip[1] + 0.5);
	pp[2] = floor(ip[2] + 0.5);
}

void vtkGinkgoImageViewer::CoordenadasImagenACoordenadasPixel(GNC::GCS::Vector& vector)
{
	vector += 0.5;
}

void vtkGinkgoImageViewer::CoordenadasMundoACoordenadasMundoReal(const double wp[3], double rwp[3])
{
	#if defined(REAL_WORLD_IMAGE_POSITION)
		if (wp != rwp) {
			rwp[0] = wp[0];
			rwp[1] = wp[1];
			rwp[2] = wp[2];
		}
	#else
		Propiedades& pr = members->Propiedades;
		GNC::GCS::Vector3D v = wp;
		v -= CurrentOrigin;
		pr.GetDirection()->MultiplyPoint(v.v, v.v);
		v += CurrentOrigin;
		rwp[0] = v.x;
		rwp[1] = v.y;
		rwp[2] = v.z;
	#endif
}

//--------------------------------------------------------------------
//region Anotaciones
void vtkGinkgoImageViewer::ActualizarAnotaciones()
{
	/*
	SetupAnnotations();
	*/
}


//region Propiedades geometricas

void vtkGinkgoImageViewer::IntersectarRectangulo(double /*wp0*/[3], double /*wp1*/[3], double /*ip0*/[3], double /*ip1*/[3])
{
	assert(false);

}

/** Update the annotations. */
void vtkGinkgoImageViewer::UpdateAnnotations( void )
{
	assert(false);
}

void vtkGinkgoImageViewer::PrintSelf(std::ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}

unsigned int vtkGinkgoImageViewer::GetConventions()
{
	Propiedades& p = members->Propiedades;
	return p.Conventions;
}

/** Reset the camera */
void vtkGinkgoImageViewer::ResetCamera()
{
	Pipeline& p = members->Pipeline;

	if (!p.Renderer) {
		return;
	}
	p.Renderer->Modified();
	p.Renderer->ResetCamera();
}

vtkSmartPointer<vtkGinkgoImageViewer> vtkGinkgoImageViewer::GetParent (void) const
{
	return NULL;
}

/**
Add a child to the list of children. Check if the child is already
in the list firt.
*/
void vtkGinkgoImageViewer::AddChild (vtkSmartPointer<vtkGinkgoImageViewer> p_view)
{
}

void vtkGinkgoImageViewer::AddChildren (std::list<vtkSmartPointer<vtkGinkgoImageViewer> > p_viewlist)
{
}

/**
Remove a child form the list of children.
*/
void vtkGinkgoImageViewer::RemoveChild (vtkSmartPointer<vtkGinkgoImageViewer> view)
{
}

void vtkGinkgoImageViewer::RemoveAllChildren (void)
{
}

void vtkGinkgoImageViewer::DrawOn()
{
}

void vtkGinkgoImageViewer::DrawOff()
{
}

void vtkGinkgoImageViewer::SetIsProcessed(bool processed)
{
	Estado& e = members->Estado;
	e.IsProcessed = processed;
}

bool vtkGinkgoImageViewer::GetIsProcessed()
{
	Estado& e = members->Estado;
	return e.IsProcessed;
}

/**
Part of the function propagation mechanism, when the function Lock() is
called, the view does not transmit the function to its children (and does
not do anything in fact).
*/
void vtkGinkgoImageViewer::Lock (void)
{
}

/**
A call to UnLock() permits to transmit function calls to the view's children.
*/
void vtkGinkgoImageViewer::UnLock (void)
{
}

/**
Returns true if the view has this child in its list.
*/
bool vtkGinkgoImageViewer::HasChild (vtkSmartPointer<vtkGinkgoImageViewer>) const
{
	return false;
}

void vtkGinkgoImageViewer::SetLinkRender(bool linkRender)
{
	Propiedades& p = members->Propiedades;
	p.LinkRender = linkRender;
}

bool vtkGinkgoImageViewer::GetLinkRender()
{
	Propiedades& p = members->Propiedades;
	return p.LinkRender;
}

void vtkGinkgoImageViewer::InternalEnableShaders(bool enabled)
{
	Pipeline& p = members->Pipeline;
	p.Textura->ForceEnableShaders(enabled);
}

//------------------------------------------------------------------------------------------------------

void vtkGinkgoImageViewer::SetupAnnotations()
{	
	assert(false);
}

void vtkGinkgoImageViewer::UpdateCamera()
{
	assert(false);
}

void vtkGinkgoImageViewer::ActualizarImagen()
{
	assert(false);
}
