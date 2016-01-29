/*
 *
 *  $Id: istudycontext.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <wx/event.h>
#include <wx/mstream.h>
#include <wx/filename.h>
#include <wx/xml/xml.h>
#include <wx/file.h>

#include <api/globals.h>
#include <api/ientorno.h>
#include <api/ivista.h>
#include <api/istudycontext.h>
#include <api/dicom/dcmdictionary.h>
#include <api/toolsystem/itoolsregistry.h>

#include <api/dicom/idicomizador.h>
#include <api/dicom/idicommanager.h>

#include <api/controllers/ipacscontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/historycontroller.h>
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/hangingprotocolcontroller.h>

#include <main/controllers/controladorcarga.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>
#include <export/tagsprivadoscomunes.h>



#undef __DEPRECATED
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkExecutive.h>
#include <vtkImageData.h>
#include <vtkDataObject.h>
#include <vtkAlgorithmOutput.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkCharArray.h>

#define _GINKGO_TRACE

GNC::GCS::IStudyContext::IStudyContext()
{
	View               = NULL;
	Viewer              = NULL;
	Module              = NULL;
	Window             = NULL;
	ParentWindow        = NULL;

	Loader              = GNC::GCS::ControladorCarga::Instance()->NewLoader();

	renderConnection    = Loader->GetOutputPort();

	ActiveFileIndex = -1;
}

GNC::GCS::IStudyContext::IStudyContext(const GNC::GCS::IStudyContext& o)
{
	*this = o;
}

GNC::GCS::IStudyContext::IStudyContext(const GNC::GCS::IStudyContext* o)
{
	if (o != NULL) {
		*this = *o;
	}
	else {
		View = NULL;
		Module = NULL;
		Window = NULL;
		ParentWindow = NULL;
		Viewer       = NULL;
		Loader = GNC::GCS::ControladorCarga::Instance()->NewLoader();
	}
}

GNC::GCS::IStudyContext::~IStudyContext()
{
}

GNC::GCS::Ptr<GNC::GCS::IReferredStudyContext> GNC::GCS::IStudyContext::NewRef(const GNC::GCS::Ptr<GNC::GCS::IStudyContext>& parent)
{
	GNC::GCS::Ptr<IReferredStudyContext> reference = new IReferredStudyContext();

	reference->View                 = parent->View;
	reference->Module                = parent->Module;
	reference->Window               = parent->Window;
	reference->ParentWindow          = parent->ParentWindow;

	reference->ActiveFileIndex   = parent->ActiveFileIndex;

	reference->Files              = parent->Files;

	reference->OpenedSeries        = parent->OpenedSeries;

	reference->EstudioPadre          = parent;
	
	reference->hangingLayout = parent->hangingLayout;

	return reference;
}

GNC::GCS::IStudyContext& GNC::GCS::IStudyContext::operator = (const GNC::GCS::IStudyContext& o)
{
	View                 = o.View;
	Module                = o.Module;
	Window               = o.Window;
	ParentWindow          = o.ParentWindow;

	Loader                = o.Loader;

	renderConnection      = Loader->GetOutputPort();

	ActiveFileIndex   = o.ActiveFileIndex;

	Files              = o.Files;

	OpenedSeries.clear();
	for (std::vector<long>::const_iterator it = o.OpenedSeries.begin(); it != o.OpenedSeries.end(); ++it) {
		OpenedSeries.push_back((*it));
	}
	hangingLayout = o.hangingLayout;

	return *this;
}

vtkSmartPointer<vtkAlgorithmOutput> GNC::GCS::IStudyContext::GetLoaderOutputConnection()
{
	return vtkSmartPointer<vtkAlgorithmOutput>(Loader->GetOutputPort());
}

void GNC::GCS::IStudyContext::SetRendererInputConnection(const vtkSmartPointer<vtkAlgorithmOutput>& input)
{
	renderConnection = input;
}

void GNC::GCS::IStudyContext::SetViewer(const vtkSmartPointer<vtkGinkgoImageViewer>& viewer)
{
	Viewer = viewer;
	if (Viewer != NULL) {
		Viewer->IImagePropertiesProxy = Loader;
	}
}

void GNC::GCS::IStudyContext::SetActiveIndex(int indice)
{
	int newIdx = indice;

	if (indice >= 0 && !Files.empty()) { // Propagate last to each other, so there is no more slices to layout them
		size_t numFiles = Files.size();
		if (indice >= numFiles) {
			newIdx = numFiles - 1;
		}
		TStudyFile& fichero = *Files[newIdx];

		ActiveFileIndex = newIdx;
	
		Loader->SetInput(fichero.PathOfFile);
		Loader->UpdateInformation();
		double spacing[3] = {1.0f, 1.0f, 1.0f};
		GetSpacingActive(spacing[0], spacing[1], spacing[2]);
		Loader->SetOutputSpacing(spacing);
		double origin[3] = {0.0f, 0.0f, 0.0f};
		GetOriginActive(origin[0], origin[1], origin[2]);
		Loader->SetOutputOrigin(origin);
		if(Viewer != NULL) {
			// FIXME: Comprobar si el pipeline esta conectado.
			Viewer->SetupPipeline();
		} else {
			//std::cerr << "No se ha inicializado el contexto correctamente (Viewer NULL) tal vez sea un estudio referido" << std::endl;
		}
	}
	else {
		LOG_ERROR("StudyContext", "Layout distribution error");
	}
}

void GNC::GCS::IStudyContext::DoInitiallizeContext(const std::list<std::string>& rutas)
{
	ActiveFileIndex = 0;
	Viewer=NULL;

	Files.clear();

	//se inicializan los vectores...

	for (std::list<std::string>::const_iterator it = rutas.begin(); it != rutas.end(); ++it)
	{
		Files.push_back(new TStudyFile());
		Files.back()->PathOfFile = (*it);
	}
}

std::list<std::string> GNC::GCS::IStudyContext::GetImagePaths()
{
	std::list<std::string> resultado;
	for (TStudyFileVector::iterator it = Files.begin(); it != Files.end(); ++it)
	{
		resultado.push_back((*it)->PathOfFile);
	}

	return resultado;
}

const std::string& GNC::GCS::IStudyContext::GetPathActiveImage()
{
	return GetImagePath(ActiveFileIndex);
}

const std::string& GNC::GCS::IStudyContext::GetImagePath(const int indice)
{
	if (indice >= 0 && (unsigned int)indice < Files.size()) {
		return Files[indice]->PathOfFile;
	}
	else {
		throw GNC::GCS::VistaException("GetRutaDeImagen(): Indice fuera de rango");
	}
}


GNC::GCS::Ptr<GIL::DICOM::TipoMetaInfo> GNC::GCS::IStudyContext::GetMetaInfoActiveImage()
{
	return GetMetaInfo(ActiveFileIndex);
}

GNC::GCS::Ptr<GIL::DICOM::DicomDataset> GNC::GCS::IStudyContext::GetTagsActiveImage()
{
	return GetTagsImage(ActiveFileIndex);
}

GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> GNC::GCS::IStudyContext::GetPrivateTagsActiveImage()
{
	return GetPrivateTags(ActiveFileIndex);
}

GNC::GCS::Ptr<GIL::DICOM::TipoMetaInfo> GNC::GCS::IStudyContext::GetMetaInfo(const int indice)
{
	if(indice < 0 || indice >= (int)Files.size())
		return NULL;
	
	TStudyFile& fichero = *Files[indice];

	if(! fichero.MetaInfo.IsValid() ) {
		LoadMetaInfo(indice);
	}
	return fichero.MetaInfo;
}

GNC::GCS::Ptr<GIL::DICOM::DicomDataset> GNC::GCS::IStudyContext::GetTagsImage(const int indice)
{
	if(indice < 0 || indice >= (int)Files.size())
		return NULL;

	TStudyFile& fichero = *Files[indice];
	
	if(!fichero.TagsImage.IsValid()) {
		LoadImageTags(indice);
	}
	return fichero.TagsImage;
}


GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> GNC::GCS::IStudyContext::GetPrivateTags(int indice)
{
	if(indice < 0 || indice >= (int)Files.size())
		return NULL;

	TStudyFile& fichero = *Files[indice];

	if(!fichero.PrivateTags.IsValid()) {
		LoadImageTags(indice);
	}
	return fichero.PrivateTags;
}

int GNC::GCS::IStudyContext::GetSliceNumber()
{
	return Files.size();
}

bool GNC::GCS::IStudyContext::GetSpacingActive(double* spacing)
{
	return GNC::GCS::IStudyContext::GetSpacing(ActiveFileIndex, spacing[0], spacing[1] , spacing[2]);
}

bool GNC::GCS::IStudyContext::GetSpacingActive(double& x, double& y, double& z)
{
	return GNC::GCS::IStudyContext::GetSpacing(ActiveFileIndex, x, y ,z);
}

void GNC::GCS::IStudyContext::RecalibrateActiveImage(double spacing[3], double origin[3])
{
	RecalibrateImage(ActiveFileIndex, spacing, origin);
}

void GNC::GCS::IStudyContext::RecalibrateImage(const int indice, double spacing[3], double origin[3])
{
	if (indice == ActiveFileIndex) {
		Loader->SetOutputOrigin(origin);
		Loader->SetOutputSpacing(spacing);

		Viewer->SetupPipeline();
	}

	SetSpacing(indice, spacing[0], spacing[1], spacing[2]);
}

bool GNC::GCS::IStudyContext::GetSpacing(const int indice, double& x, double& y, double& z)
{
	bool hasSpacing = false;
	std::string spacing;
	x=0.0f;
	y=0.0f;
	z=0.0f;
	GNC::GCS::Ptr<GIL::DICOM::DicomDataset> tagsImage = GetTagsImage(indice);
	if (tagsImage.IsValid()) {
		if(tagsImage->getTag("0028|0030",spacing)) {
			char c;
			std::istringstream issl(spacing);
			issl >> x;
			if(!issl.eof()){
				issl>>c;//la barra
				issl>>y;
			}
			if(!issl.eof()){
				issl>>c;//la barra
				issl>>z;
			}
			hasSpacing = true;
		}
		else if(tagsImage->getTag("0018|1164",spacing)) {
			char c;
			std::istringstream issl(spacing);
			issl >> x;
			if(!issl.eof()){
				issl>>c;//la barra
				issl>>y;
			}
			if(!issl.eof()){
				issl>>c;//la barra
				issl>>z;
			}
			hasSpacing = true;
		}
	}
	if (x < std::numeric_limits<double>::epsilon()) {
		x = 1.0f;
		hasSpacing = false;
	}
	if (y < std::numeric_limits<double>::epsilon()) {
		y = 1.0f;
		hasSpacing = false;
	}
	if (z < std::numeric_limits<double>::epsilon()) {
		z = 1.0f;
	}
	return hasSpacing;
}

void GNC::GCS::IStudyContext::SetSpacing(const int indice, const double x, const double y, const double z)
{
	std::ostringstream os;
	os << x << "\\" << y << "\\" <<z;
	std::string nuevoSpacing(os.str());

	GNC::GCS::Ptr<GIL::DICOM::DicomDataset> tagsImage = GetTagsImage(indice);
	if (tagsImage.IsValid()) {
		tagsImage->tags[std::string("0028|0030")] = nuevoSpacing;
	}
}

void GNC::GCS::IStudyContext::GetOriginActive(double* origin)
{
	GetOrigin(ActiveFileIndex, origin[0], origin[1], origin[2]);
}

void GNC::GCS::IStudyContext::GetOriginActive(double& x, double& y, double& z)
{
	GetOrigin(ActiveFileIndex, x, y ,z);
}

void GNC::GCS::IStudyContext::GetOrigin(const int indice, double& x, double& y, double& z)
{
	std::string origin;
	x = y = z =0.0f;
	GNC::GCS::Ptr<GIL::DICOM::DicomDataset> tagsImagen = GetTagsImage(indice);
	if (tagsImagen.IsValid()) {
		if(tagsImagen->getTag("0020|0032",origin)) {
			char c;
			std::istringstream issl(origin);
			issl >> x;
			if(!issl.eof()){
				issl>>c;//la barra
				issl>>y;
			}
			if(!issl.eof()){
				issl>>c;//la barra
				issl>>z;
			}
		}
	}
}

void GNC::GCS::IStudyContext::GetDimensionsActiveImage(int* dims)
{
	GetDimensionsImage(ActiveFileIndex, dims[0], dims[1], dims[2]);
}

void GNC::GCS::IStudyContext::GetDimensionsActiveImage(int& x, int& y, int& z)
{
	GetDimensionsImage(ActiveFileIndex, x, y, z);
}

void GNC::GCS::IStudyContext::GetDimensionsImage(const int indice, int& x, int& y, int& z)
{
	//z = Files.size();
	x = y = z = 0;
	GNC::GCS::Ptr<GIL::DICOM::DicomDataset> tagsImagenOriginal = GetTagsImage(indice);
	if (tagsImagenOriginal.IsValid()) {
		std::string tag;
		if (tagsImagenOriginal->getTag("0028|0010",tag) )
		{
			std::istringstream issl(tag);
			issl >> y;
		}
		if (tagsImagenOriginal->getTag("0028|0011",tag))
		{
			std::istringstream issl(tag);
			issl >> x;
		}

		if (tagsImagenOriginal->getTag("0028|0008",tag))
		{
			std::istringstream issl(tag);
			issl >> z;
		}
		else {
			z = 1;
		}
	}
}

int GNC::GCS::IStudyContext::GetTSizeActive()
{
	int dimensions[3] = {0,0,0};
	Loader->GetOutputDimensions(dimensions);
	return dimensions[2];
}

std::string GNC::GCS::IStudyContext::GetTagActiveImage(const std::string& tag)
{
	std::string tagVal;
	GetTagActiveImage(tag, tagVal);
	return tagVal;
}

bool GNC::GCS::IStudyContext::GetTagActiveImage(const std::string& tag, std::string& valor)
{
	GNC::GCS::Ptr<GIL::DICOM::DicomDataset> pointer = GetTagsActiveImage();
	if (pointer.IsValid()) {
		return pointer->getTag(tag,valor);
	} else {
		return false;
	}
}

int GNC::GCS::IStudyContext::GetPathIndex(const std::string& path)
{

	int i = 0;
	
	for (TStudyFileVector::iterator it = Files.begin(); it != Files.end(); ++it, i++) 
	{		
		if ((*it)->PathOfFile == path)
		{
			return i;
		}
	}
	return -1;
}

void GNC::GCS::IStudyContext::CreateLoader()
{
	Loader              = GNC::GCS::ControladorCarga::Instance()->NewLoader();

	renderConnection    = Loader->GetOutputPort();
}


bool GNC::GCS::IStudyContext::IsKeyImageActive()
{
	return IsKeyImage(ActiveFileIndex);
}

bool GNC::GCS::IStudyContext::IsKeyImage(int indice)
{
	GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivados = GetPrivateTags(indice);
	if (!pTagsPrivados.IsValid())
		return false;

	std::string isKey;
	if (pTagsPrivados->GetTag(TAG_KEY_IMAGE, isKey)) {
		return isKey.compare("YES") == 0;
	} else {
		return false;
	}
}
void GNC::GCS::IStudyContext::LoadMetaInfo(const int indice)
{
	GNC::GCS::ILocker lock(this);
	//tags imagenes

	TStudyFile* fichero = Files[indice];

	if (!fichero->MetaInfo.IsValid()) {
		fichero->MetaInfo = new GIL::DICOM::TipoMetaInfo();
		
		GIL::DICOM::IDICOMManager*	pDICOMManager;
		pDICOMManager= GNC::Entorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
		pDICOMManager->CargarMetaInfo(fichero->PathOfFile, *(fichero->MetaInfo));
		GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
	}
}

void GNC::GCS::IStudyContext::LoadImageTags(const int indice)
{
	GNC::GCS::ILocker lock(this);

	TStudyFile* fichero = Files[indice];


	//tags imagenes
	if (!fichero->TagsImage.IsValid()) {
		fichero->TagsImage = new GIL::DICOM::DicomDataset();
		fichero->PrivateTags = new GIL::DICOM::TipoPrivateTags(UID_TAGS_PRIVADOS_COMUNES);

		GIL::DICOM::IDICOMManager*	pDICOMManager;
		pDICOMManager= GNC::Entorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
		pDICOMManager->CargarFichero(fichero->PathOfFile, *(fichero->TagsImage));
		pDICOMManager->CargarTagsPrivados(*(fichero->PrivateTags));
		CallbackCargarTagsImagen(indice, pDICOMManager);
		GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
	}
}

//inicializa el mapa de valoracion con los datos del tag privado, si no existe el tag privado lo crea y lo inserta
void GNC::GCS::IStudyContext::CreateUnsignedCharMap(vtkSmartPointer<vtkImageData>& pMapa, GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivados, const unsigned char tag, const int indice, unsigned char valorInicial)
{
	GNC::GCS::ILocker lock(this);
	//se crea la imagen
	if (pMapa == NULL) {
		pMapa = vtkSmartPointer<vtkImageData>::New();

		double spacing[3] = {1.0f,1.0f,1.0f};
		double origin[3] = {0.0f,0.0f,0.0f};
		int dimensions[3] = {0,0,0};
		GetSpacing(indice,spacing[0],spacing[1],spacing[2]);
		GetOrigin(indice,origin[0],origin[1],origin[2]);
		GetDimensionsImage(indice,dimensions[0],dimensions[1], dimensions[2]);

		//se inicializan valores de dimensions, scalartype...
		pMapa->SetDimensions(dimensions[0],dimensions[1],1);

                pMapa->AllocateScalars(VTK_UNSIGNED_CHAR, 1); 
		pMapa->SetSpacing(spacing[0],spacing[1],spacing[2]);
		pMapa->SetOrigin(origin[0],origin[1],origin[2]);

		GIL::DICOM::TagPrivadoUndefined* pTagPrivado = pTagsPrivados->GetTagUndefined(tag);
		if(pTagPrivado == NULL) {
			const unsigned int size = dimensions[0] * dimensions[1];
			pTagPrivado = pTagsPrivados->NewTagUndefined(tag,size);
			//se hace un fill 0
			const unsigned char* fin = size + pTagPrivado->GetValor();
			for( unsigned char* pData = pTagPrivado->GetValor(); pData < fin; ++pData)
			{
				(*pData) = valorInicial;
			}
		}

                unsigned int size = pMapa->GetDimensions()[0] * pMapa->GetDimensions()[1];
                unsigned char *in_data =  pTagPrivado->GetValor();
                unsigned char *out_ptr =  reinterpret_cast<unsigned char*>(pMapa->GetScalarPointer()); 
                std::copy(in_data, in_data + size, out_ptr); 
	}
}

void GNC::GCS::IStudyContext::CreateCharMap(vtkSmartPointer<vtkImageData>& pMapa, GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivados, const unsigned char tag, const int indice, char valorInicial)
{
	GNC::GCS::ILocker lock(this);
	//se crea la imagen
	if (pMapa == NULL) {
		pMapa = vtkSmartPointer<vtkImageData>::New();

		double spacing[3] = {1.0f,1.0f,1.0f};
		double origin[3] = {0.0f,0.0f,0.0f};
		int dimensions[3] = {0,0,0};
		GetSpacing(indice,spacing[0],spacing[1],spacing[2]);
		GetOrigin(indice,origin[0],origin[1],origin[2]);
		GetDimensionsImage(indice,dimensions[0],dimensions[1], dimensions[2]);

		//se inicializan valores de dimensions, scalartype...
		pMapa->SetDimensions(dimensions[0],dimensions[1],1);
                pMapa->AllocateScalars(VTK_CHAR, 1); 
		pMapa->SetSpacing(spacing[0],spacing[1],spacing[2]);
		pMapa->SetOrigin(origin[0],origin[1],origin[2]);

		GIL::DICOM::TagPrivadoUndefined* pTagPrivado = pTagsPrivados->GetTagUndefined(tag);
		if(pTagPrivado == NULL) {
			const unsigned int size = dimensions[0] * dimensions[1];
			pTagPrivado = pTagsPrivados->NewTagUndefined(tag,size);
			//se hace un fill 0
			const char* fin = size + (char*)pTagPrivado->GetValor();
			for( char* pData = (char*)pTagPrivado->GetValor(); pData < fin; ++pData)
			{
				(*pData) = valorInicial;
			}
		}

                unsigned int size = pMapa->GetDimensions()[0] * pMapa->GetDimensions()[1];
                unsigned char *in_data =  pTagPrivado->GetValor();
                unsigned char *out_ptr =  reinterpret_cast<unsigned char*>(pMapa->GetScalarPointer()); 
                std::copy(in_data, in_data + size, out_ptr); 
	}
}

GNC::GCS::IReferredStudyContext::IReferredStudyContext()
{
}

GNC::GCS::IReferredStudyContext::~IReferredStudyContext()
{
}

void GNC::GCS::IReferredStudyContext::UnRefViewer()
{
	Viewer = NULL;
}
