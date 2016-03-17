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

//#define _GINKGO_TRACE
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <cmath>
#include "thumbnailscommand.h"
#include <api/globals.h>
#include <api/internationalization/internationalization.h>

#include <wx/filename.h>
#include <wx/file.h>
#include <wx/image.h>

#include <resources/ginkgoresourcesmanager.h>

#include <main/entorno.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladorcarga.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/streaming/streamingloader.h>
#include <main/controllers/controladorextensiones.h>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkAlgorithmOutput.h>
#include <vtkImageShiftScale.h>
#include <vtkImageMapToRGBA.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkExecutive.h>
#include <vtkInformationVector.h>
#include <itk/itkVTKImageToImageFilter.h>
#include <vtkVISUManagement/vtkLookupTableManager.h>
#include <vtkJPEGWriter.h>


#include <itkExceptionObject.h>
#include <itkImage.h>
#include <itkRGBPixel.h>
#include <itkGDCMImageIO.h>
#include <itkImageFileReader.h>
#include <itkVectorResampleImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkImageDuplicator.h>

#include <api/dicom/dcmdictionary.h>
#include <api/dicom/dcmuiddictionary.h>

#define IDC_GENERAR_THUMBNAILS       81
#define IDC_CARGA       91
#define SIZE_THUMBNAILS 76

namespace GADAPI
{
class ThumbnailsCommandParams: public GNC::GCS::IComandoParams
{
public:
        ThumbnailsCommandParams(long file_pk, ThumbnailsNotifier* pNotificador)
        {
                m_file_pk = file_pk;
                m_wxImg = NULL;
                m_pNotificadorThumbnail = pNotificador;
        }

        virtual ~ThumbnailsCommandParams()
        {
                if (m_wxImg != NULL) {
                        delete m_wxImg;
                        m_wxImg = NULL;
                }
        }

        GNC::GCS::HistoryController::FileModel m_fileModel;
        long									  m_file_pk;
        wxImage*                        m_wxImg;
        ThumbnailsNotifier*			m_pNotificadorThumbnail;

};

ThumbnailsCommand::ThumbnailsCommand(long file_pk, ThumbnailsNotifier* pNotificador) : IComando(NULL, "GenerarThumbnails")
{
        m_Error = false;
        GTRACE(">> ThumbnailsCommand::ThumbnailsCommand(): " << this);
        m_pThumbParams = new ThumbnailsCommandParams(file_pk, pNotificador);
        m_pParams = m_pThumbParams;
        SetId(IDC_GENERAR_THUMBNAILS);
        EsperaA(IDC_GENERAR_THUMBNAILS);
        EsperaA(IDC_CARGA);
        GTRACE("<< ThumbnailsCommand::ThumbnailsCommand(): " << this);
}

void ThumbnailsCommand::Execute()
{
        NotificarProgreso(0.05f,_Std("Creating Thumbnail..."));
        m_Error = true;
        GTRACE("Arrancando comando generar thumbnails  " << m_pThumbParams->m_ruta)
        //std::string tarea_Std("Generando thumbnails...");
        //pillamos el uid
        m_pThumbParams->m_fileModel = GNC::GCS::HistoryController::Instance()->GetFileModel(m_pThumbParams->m_file_pk);
        if(m_pThumbParams->m_fileModel.sopiuid.empty()) {
                wxBitmap bmp = GinkgoResourcesManager::History::GetIcoUnknownFile();
                m_pThumbParams->m_wxImg = new wxImage();
                (*m_pThumbParams->m_wxImg) = bmp.ConvertToImage();
                GuardarImagen();
                m_Error = false;
                return;
        }
        //if is wado linked...
        if (m_pThumbParams->m_fileModel.location == GNC::GCS::IHistoryController::TL_WadoLinked) {
                wxBitmap bmp = GinkgoResourcesManager::History::GetIcoWadoLinkedPreview();
                m_pThumbParams->m_wxImg = new wxImage();
                (*m_pThumbParams->m_wxImg) = bmp.ConvertToImage();
                m_Error = false;
                return;
        }
        //comprobar si el thumbnail ya estaba generado
        m_pThumbParams->m_wxImg = GetImageFromBD();
        if(m_pThumbParams->m_wxImg != NULL) {
                GTRACE("Saliendo generar thumbnails salida2  " << m_pThumbParams->m_ruta);
                NotificarProgreso(1.0f,_Std("Creating Thumbnail..."));
                m_Error = false;
                return;
        } else {
                //try to make thumbnail with extensions...
                ::GNC::ControladorExtensiones::ListaModulos listaModulos = ::GNC::ControladorExtensiones::Instance()->Modulos();
                for(GNC::ControladorExtensiones::IteradorListaModulos it=listaModulos.begin(); it!=listaModulos.end(); ++it) {
                        GNC::GCS::IModuleController* modulo = (*it).second;
                        GNC::GCS::Ptr<ImgProxy<UCHAR3> >pImgCapture(new ImgProxy<UCHAR3>());
                        if (modulo->MakeThumbnail(m_pThumbParams->m_fileModel, *pImgCapture)) {
                                wxImage* pImage = new wxImage(pImgCapture->anchura, pImgCapture->altura, (unsigned char*)pImgCapture->data, true);
                                if (pImage->IsOk()) {
                                        double scaleX = ((double) SIZE_THUMBNAILS)/((double) pImgCapture->anchura);
                                        double scaleY = ((double) SIZE_THUMBNAILS)/((double) pImgCapture->altura);

                                        if (scaleX < 1.0 || scaleY < 1.0) {
                                                double scale = wxMin(scaleX, scaleY);
                                                int newWidth = (int) (scale * pImgCapture->anchura);
                                                int newHeight = (int) (scale * pImgCapture->altura);
                                                m_pThumbParams->m_wxImg = new wxImage(pImage->Scale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH));
                                        } else {
                                                m_pThumbParams->m_wxImg = new wxImage(*pImage);
                                        }
                                        delete pImage;

                                        GuardarImagen();
                                        m_Error = false;
                                        return;
                                }
                        }
                }

                if (m_pThumbParams->m_fileModel.tsuid== GKUID_MPEG2MainProfileAtMainLevelTransferSyntax || m_pThumbParams->m_fileModel.tsuid == GKUID_MPEG2MainProfileAtHighLevelTransferSyntax) {
                        wxBitmap bmp = GinkgoResourcesManager::History::GetMoviePreview();
                        m_pThumbParams->m_wxImg = new wxImage();
                        (*m_pThumbParams->m_wxImg) = bmp.ConvertToImage();
                        GuardarImagen();
                        m_Error = false;
                        return;
                }
                if (m_pThumbParams->m_fileModel.sopcuid == GKUID_EncapsulatedPDFStorage) {
                        wxBitmap bmp = GinkgoResourcesManager::History::GetPDFPreview();
                        m_pThumbParams->m_wxImg = new wxImage();
                        (*m_pThumbParams->m_wxImg) = bmp.ConvertToImage();
                        m_Error = false;
                        return;
                }

                double size[2]   = {0.0, 0.0};
                int dimensions[3] = {0, 0, 0};
                double spacing[3] = {0.0, 0.0, 0.0};
                double origin[3] = {0.0, 0.0, 0.0};

                size[0] = SIZE_THUMBNAILS;
                size[1] = SIZE_THUMBNAILS;

                {
                        //esto es una nyapa para recuperarnos de errores, antes de leer guardamos en bbdd una imagen negra, si finalmente
                        //el thumbnail se genera bien pues se mete, si no pues se quedara con unknown
                        wxImage img = GinkgoResourcesManager::History::GetIcoUnknownFile().ConvertToImage();
                        m_pThumbParams->m_wxImg = &img;
                        GuardarImagen();
                        m_pThumbParams->m_wxImg = NULL;
                }

                typedef itk::RGBPixel<unsigned char> PixelType;
                typedef itk::Image< PixelType,  2 > ImageType;

                typedef itk::ImageRegionConstIterator< ImageType > ImageIteratorType;

                typedef itk::VTKImageToImageFilter<ImageType> TipoFiltro;

                ImageType::SizeType outputSize;
                ImageType::SpacingType outputSpacing;
                ImageType::PointType outputOrigin;

                vtkSmartPointer<vtkImageShiftScale> normalizeFilter = vtkSmartPointer<vtkImageShiftScale>::New();

                outputSize[0] = size[0];
                outputSize[1] = size[1];

                unsigned long imgsize = outputSize[0] * outputSize[1] * sizeof(unsigned char) * 3;
                unsigned char* data = new unsigned char[imgsize];

                try {

                        GNC::StreamingLoader loader;

                        loader.SetInput(m_pThumbParams->m_fileModel.real_path);

                        loader.GetDimensions(dimensions);
                        loader.GetSpacing(spacing);
                        loader.SetOutputOrigin(origin);

                        vtkSmartPointer<vtkImageReslice> pReslice = vtkSmartPointer<vtkImageReslice>::New();
                        TipoFiltro::Pointer VTK2ITKfiltro = TipoFiltro::New();

                        vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();


                        if ( dimensions[0] == 0 || dimensions[1] == 0 || spacing[0] == 0 || spacing[1] == 0) {
                                if ( !loader.IsSignalFile()) {
                                        m_Error = true;
                                        delete[] data;
                                        data = NULL;
                                        LOG_WARN("GenerarThumbnails", "Error: imagen de tamaño 0, generación ignorada.");
                                        return;
                                }

                                NotificarProgreso(0.8f,_Std("Creating Thumbnail..."));

                                memset(data, 0, imgsize);
                                double x, y;
                                unsigned int ix, iy;
                                const double resx = M_PI * 4.0 / (double) outputSize[0];
                                const double resy = (double) 0.25 * outputSize[1];
                                const double sy = (double) 0.5 * outputSize[1];

                                for (ix = 0; ix < outputSize[0]; ++ix) {

                                        x = ((double) ix);
                                        y = resy * std::sin(x * resx);
                                        y += sy;
                                        iy = std::max<int>(0, std::min<int>(outputSize[0] - 1, (int) std::floor(y + 0.5)) );

                                        unsigned char* pixel = data + (3 * (iy * outputSize[0] + ix));
                                        pixel[0] = 0;
                                        pixel[1] = 255;
                                        pixel[2] = 128;
                                }

                        } else {


                                pReslice->SetInputConnection(loader.GetOutputPort());
                                pReslice->SetOutputDimensionality(2);
                                pReslice->SetResliceAxesDirectionCosines(1, 0, 0,
                                                0, 1, 0,
                                                0, 0, 1);
                                outputSize[0] = size[0];
                                outputSize[1] = size[1];

                                double dInputSize[3];
                                dInputSize[0] = (double)dimensions[0];
                                dInputSize[1] = (double)dimensions[1];
                                dInputSize[2] = 1.0;

                                double dOutputSize[3];
                                dOutputSize[0] = (double)outputSize[0];
                                dOutputSize[1] = (double)outputSize[1];
                                dOutputSize[2] = 1.0;

                                double sX = dOutputSize[0] / dInputSize[0];
                                double sY = dOutputSize[1] / dInputSize[1];

                                double s = std::min<double>(sX, sY);

                                double dOutputSpacing[3];
                                dOutputSpacing[0] = spacing[0] / s;
                                dOutputSpacing[1] = spacing[1] / s;
                                dOutputSpacing[2] = spacing[2];

                                pReslice->SetOutputExtent(0, dOutputSize[0] - 1, 0, dOutputSize[1] - 1,  0, 1);
                                pReslice->SetOutputSpacing(dOutputSpacing);
                                //moves image vertical or horizontal to center thumbnail. x + => left; - => right. y + => up; - => down
                                //formula is tested with color images and images with spacing and origin
                                origin[0] = (((s - sX) * dInputSize[0] / 2.0f) / s) * spacing[0];
                                origin[1] = (((s - sY) * dInputSize[1] / 2.0f) / s)  * spacing[0];
                                pReslice->SetOutputOrigin(origin);

                                pReslice->SetInterpolationModeToCubic();

                                pReslice->Update();

                                vtkImageData *timg = pReslice->GetOutput();

                                if(timg->GetNumberOfScalarComponents() > 1) {
                                        normalizeFilter->SetInputConnection(pReslice->GetOutputPort());
                                        normalizeFilter->SetOutputScalarTypeToUnsignedChar();

                                        if (timg->GetScalarType() != VTK_UNSIGNED_CHAR) {
                                                double range[2];
                                                timg->GetScalarRange(range);

                                                double diff = range[1]-range[0];
                                                if (diff > std::numeric_limits<double>::epsilon()) {
                                                        normalizeFilter->SetShift(-range[0]);
                                                        normalizeFilter->SetScale(255.0/(range[1]-range[0]));
                                                }
                                        }
                                        normalizeFilter->Update();
                                        VTK2ITKfiltro->SetInput(normalizeFilter->GetOutput());

                                } else {
                                        vtkSmartPointer<vtkImageMapToColors> pImageMap = vtkSmartPointer<vtkImageMapToColors>::New();

                                        pImageMap->SetInputConnection(pReslice->GetOutputPort());

                                        vtkSmartPointer<vtkLookupTable> pLookupTable = vtkLookupTableManager::GetLinearLookupTable();

                                        pImageMap->SetLookupTable(pLookupTable);
                                        pImageMap->SetOutputFormatToRGB();

                                        if (timg->GetNumberOfScalarComponents() == 1) {

                                                double window, level;
                                                if (loader.GetDefaultWindowLevel(window,level) && window != 0.0) {
                                                        double v_min = (level) - 0.5 * window;
                                                        double v_max = level + 0.5 * window;
                                                        pLookupTable->SetRange(v_min, v_max);
                                                } else {
                                                        double range[2];
                                                        timg->GetScalarRange(range);

                                                        double diff = range[1]-range[0];

                                                        window = range[1]-range[0];
                                                        level = 0.5*(range[1]+range[0]);

                                                        if (diff > std::numeric_limits<double>::epsilon()) {
                                                                double v_min = (level) - 0.5 * window;
                                                                double v_max = level + 0.5 * window;
                                                                pLookupTable->SetRange(v_min, v_max);
                                                        }
                                                }
                                        }
                                        pImageMap->SetLookupTable(pLookupTable);

                                        pImageMap->Update();
                                        VTK2ITKfiltro->SetInput( pImageMap->GetOutput());
                                }
                                NotificarProgreso(0.6f,_Std("Creating Thumbnail..."));

                                VTK2ITKfiltro->GetImporter()->UpdateLargestPossibleRegion();

                                const ImageType* img = VTK2ITKfiltro->GetOutput();
                                NotificarProgreso(0.8f,_Std("Creating Thumbnail..."));

                                ImageType::RegionType region = img->GetLargestPossibleRegion();

                                ImageIteratorType it (img, region);

                                std::cout << "imgsize = " << imgsize << std::endl;

                                std::cout << "region = ";
                                region.Print(std::cout);
                                std::cout << std::endl;

                                unsigned long off = 0;
                                for (it.GoToBegin(); !it.IsAtEnd() && off < imgsize; ++it) {
                                        const ImageType::PixelType& pixel = it.Value();
                                        data[off++] = pixel.GetRed();
                                        data[off++] = pixel.GetGreen();
                                        data[off++] = pixel.GetBlue();
                                }
                        }

                        m_pThumbParams->m_wxImg = new wxImage(outputSize[0], outputSize[1], data, false);
                        GuardarImagen();
                        NotificarProgreso(1.0f,_Std("Generating thumbnail ..."));

                        m_Error = false;

                } catch (GNC::GCS::ControladorCargaException &ex1) {
                        LOG_ERROR("GenerarThumnails", "Unable to create thumbnail for file [" << m_pThumbParams->m_file_pk << "]: " << ex1.str());
                        if (data != NULL) {
                                delete[] data;
                                data = NULL;
                        }
                        //si se cancela el comando
                        return;
                } catch(itk::ExceptionObject& ex2) {
                        LOG_ERROR("GenerarThumnails", "Unable to create thumbnail for file [" << m_pThumbParams->m_file_pk << "]: " << ex2.GetDescription());
                        //std::string descr = ex2.GetDescription();
                        if (data != NULL) {
                                delete[] data;
                                data = NULL;
                        }
                        return;
                } catch(std::exception &ex3) {
                        LOG_ERROR("GenerarThumnails", "Unable to create thumbnail for file [" << m_pThumbParams->m_file_pk << "]: " << ex3.what());
                        //std::string descr = ex3.what();
                        //si se cancela el comando
                        if (data != NULL) {
                                delete[] data;
                                data = NULL;
                        }
                        return;
                } catch(...) {
                        LOG_ERROR("GenerarThumnails", "Unable to create thumbnail for file [" << m_pThumbParams->m_file_pk << "]: Internal error" );
                        //si se cancela el comando
                        if (data != NULL) {
                                delete[] data;
                                data = NULL;
                        }
                        return;
                }
        }
}

void ThumbnailsCommand::Update()
{
        if (!m_Error) {
                if (m_pThumbParams == NULL || m_pThumbParams->m_wxImg == NULL || !m_pThumbParams->m_wxImg->IsOk()) {
                        LOG_ERROR("GenerarThumbnails", "No se pudo establecer la previsualizacion: La imagen es invalida o no fue generada correctamente");
                } else {
                        m_pThumbParams->m_pNotificadorThumbnail->SetImage(m_pThumbParams->m_file_pk, m_pThumbParams->m_wxImg);
                }
        } else {
                wxImage img =  GinkgoResourcesManager::History::GetIcoUnknownFile().ConvertToImage();
                m_pThumbParams->m_pNotificadorThumbnail->SetImage(m_pThumbParams->m_file_pk, &img);
        }
}

wxImage* ThumbnailsCommand::GetImage()
{
        return m_pThumbParams->m_wxImg;
}

wxImage* ThumbnailsCommand::GetImageFromBD()
{
        int ancho,alto = 0;
        unsigned char* data = NULL;
        GNC::GCS::HistoryController::Instance()->GetThumbnail(m_pThumbParams->m_file_pk, ancho, alto, data);
        if(data != NULL) {
                wxImage* img = new wxImage(ancho,alto,data,false);
                return img;
        } else {
                return NULL;
        }
}


unsigned char ClampToByte(int a)
{
        if (a > 255)
                return 255;
        else if (a < 0)
                return 0;
        else
                return a;
}

unsigned char ClampUpToByte(int a)
{
        if (a > 255)
                return 255;
        else
                return a;
}


void ThumbnailsCommand::GuardarImagen()
{
        if (!m_pThumbParams->m_wxImg->IsOk())
                return;

        unsigned char *pixelData = m_pThumbParams->m_wxImg->GetData();

        // We apply a vertical gradient
        const int w = m_pThumbParams->m_wxImg->GetWidth();
        const int h = m_pThumbParams->m_wxImg->GetHeight();
        const int med_h = h/2;

        // Aplicamos desde el origen hasta la mitad en altura (255, 255, 255, 100) -> (255, 255, 255, 0)
        for (int y = 0, pos = 0, alpha = 70; (y < med_h) && (alpha > 0); ++y, alpha-=2) {
                for (int x = 0; x < w; ++x, pos+=3) {
                        pixelData[pos+0] = ClampUpToByte((int) pixelData[pos+0] + alpha);
                        pixelData[pos+1] = ClampUpToByte((int) pixelData[pos+1] + alpha);
                        pixelData[pos+2] = ClampUpToByte((int) pixelData[pos+2] + alpha);
                }

        } // for y

        GNC::GCS::HistoryController::Instance()->SetThumbnail(m_pThumbParams->m_file_pk,
                        m_pThumbParams->m_wxImg->GetWidth(),
                        m_pThumbParams->m_wxImg->GetHeight(),
                        m_pThumbParams->m_wxImg->GetData());

}

}
