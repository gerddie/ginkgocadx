#pragma once
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


#include <api/api.h>
#include <string>
#include <vector>
#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif
#include <vtkSmartPointer.h>
#include <api/autoptr.h>
#include <api/ilock.h>
#include <api/dicom/idicom.h>

class vtkGinkgoImageViewer;
class vtkAlgorithmOutput;
class vtkImageData;

class wxWindow;

namespace GNC
{
namespace GCS
{
class IEntorno;
class IVista;
class IStreamingLoader;
class IImagePropertiesProxy;
class IReferredStudyContext;
class IModuleController;
class Layout;
class HangingProtocol;
}
}

namespace GIL
{
namespace DICOM
{
class DicomDataset;
class TipoPrivateTags;
class IDICOMManager;
}
}

namespace GNC
{
namespace GCS
{
//region Interfaz de gestion de entrada (ficheros, diagnosticos, etc..)
class EXTAPI IStudyContext: public ILockable
{
public:
        typedef struct TStudyFile {
                std::string PathOfFile;
                bool			WidgetsLoaded;
                GNC::GCS::Ptr<GIL::DICOM::TipoMetaInfo> MetaInfo;
                GNC::GCS::Ptr<GIL::DICOM::DicomDataset> TagsImage;
                GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> PrivateTags;

                TStudyFile()
                {
                        PathOfFile = "";
                        WidgetsLoaded = false;
                }

        } TStudyFile;


        typedef std::vector<GNC::GCS::Ptr<TStudyFile> >     TStudyFileVector;

        GNC::GCS::IVista*                                 View;
        GNC::GCS::IModuleController*                      Module;
        vtkSmartPointer<vtkGinkgoImageViewer>             Viewer;
        wxWindow*                                         ParentWindow; // La ventana WX de la que es padre la vista (Notebook/Grid..)
        wxWindow*                                         Window;      // La ventana WX propia de la vista

        GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;

        vtkSmartPointer<vtkAlgorithmOutput>               renderConnection; // Conexion hacia renderer
        GNC::GCS::Ptr<GNC::GCS::Layout>					  hangingLayout;
        GNC::GCS::Ptr<GNC::GCS::HangingProtocol>          hangingProtocol;

        int                                               ActiveFileIndex;
        std::vector<long>											  OpenedSeries;
protected:
        TStudyFileVector											  Files;
public:

        IStudyContext();

        IStudyContext(const IStudyContext& o);
        IStudyContext(const IStudyContext* o)__attribute_deprecated__;

        virtual ~IStudyContext();

        static GNC::GCS::Ptr<GNC::GCS::IReferredStudyContext> NewRef(const GNC::GCS::Ptr<GNC::GCS::IStudyContext>& parent);

        IStudyContext& operator = (const IStudyContext& o);

        /** Obtiene el puerto de conexion de salida del StreamingLoader. Se usa para inyectar pipelines que necesiten las vistas graficas
        	Pipeline: Loader.ruta_fichero => [...] => Loader.loaderOutputConnection => [PIPELINE_ESPECIFICO] => Viewer.renderInputConnection => [...]
        **/
        vtkSmartPointer<vtkAlgorithmOutput> GetLoaderOutputConnection();

        /** Obtiene el puerto de conexion de entrada hacia el renderer. Por defecto es la salida del StreamingLoader
        	Pipeline: Loader.ruta_fichero => [...] => Loader.loaderOutputConnection => [PIPELINE_ESPECIFICO] => Viewer.renderInputConnection => [...]
        **/
        void SetRendererInputConnection(const vtkSmartPointer<vtkAlgorithmOutput>& input);

        void SetViewer(const vtkSmartPointer<vtkGinkgoImageViewer>& viewer);

        virtual void SetActiveIndex(int indice);

        std::list<std::string> GetImagePaths();

        const std::string& GetPathActiveImage();
        const std::string& GetImagePath(const int indice);

        //tags
        GNC::GCS::Ptr<GIL::DICOM::TipoMetaInfo>    GetMetaInfoActiveImage();
        GNC::GCS::Ptr<GIL::DICOM::DicomDataset>   GetTagsActiveImage();
        GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> GetPrivateTagsActiveImage();

        GNC::GCS::Ptr<GIL::DICOM::TipoMetaInfo>    GetMetaInfo(const int indice);
        GNC::GCS::Ptr<GIL::DICOM::DicomDataset>   GetTagsImage(const int indice);
        GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> GetPrivateTags(int indice);

        int GetSliceNumber();
        virtual void RecalibrateActiveImage(double spacing[3], double origin[3]);
        virtual void RecalibrateImage(const int indice, double spacing[3], double origin[3]);

        virtual bool GetSpacingActive(double* spacing);
        virtual bool GetSpacingActive(double& x, double& y, double& z);
        // Returns true if image has spacing
        bool GetSpacing(const int indice, double& x, double& y, double& z);
        virtual void SetSpacing(const int indice, const double x, const double y, const double z);

        void GetOriginActive(double* origin);
        void GetOriginActive(double& x, double& y, double& z);
        void GetOrigin(const int indice, double& x, double& y, double& z);

        void GetDimensionsActiveImage(int* dimensions);
        void GetDimensionsActiveImage(int& x, int& y, int& z);
        void GetDimensionsImage(const int indice, int& x, int& y, int& z);

        int GetTSizeActive();

        int GetPathIndex(const std::string& path);

        std::string GetTagActiveImage(const std::string& tag);
        bool GetTagActiveImage(const std::string& tag, std::string& valor);

        bool IsKeyImageActive();
        bool IsKeyImage(int indice = 0);

        void CreateLoader();

protected:
        void DoInitiallizeContext(const std::list<std::string>& rutas);

        void LoadMetaInfo(const int indice);
        void LoadImageTags(const int indice);
public:
        virtual void CallbackCargarTagsImagen(const int /*indice*/, GIL::DICOM::IDICOMManager* /*pDicomManager*/) = 0;
protected:
        void CreateUnsignedCharMap(vtkSmartPointer<vtkImageData>& pMapa, GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivados, const unsigned char tag, const int indice, unsigned char valorInicial = 0);
        void CreateCharMap(vtkSmartPointer<vtkImageData>& pMapa, GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivados, const unsigned char tag, const int indice, char valorInicial = 0);

        friend class IContextoEstudioReferido;
};

class EXTAPI IReferredStudyContext : public IStudyContext
{
public:
        IReferredStudyContext();

        virtual ~IReferredStudyContext();

        void UnRefViewer();

        virtual void CallbackCargarTagsImagen(const int indice, GIL::DICOM::IDICOMManager* pDicomManager)
        {
                EstudioPadre->CallbackCargarTagsImagen(indice, pDicomManager);
        }

        GNC::GCS::Ptr<GNC::GCS::IStudyContext> EstudioPadre;

};
}
}
