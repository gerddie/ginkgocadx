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


#include <wx/filename.h>
#include <wx/file.h>
#include <wx/thread.h>

#include <api/globals.h>

#include <api/icommand.h>
#include <api/dicom/idicommanager.h>
#include <main/controllers/commandcontroller.h>
#include <api/imodelointegracion.h>
#include <api/internationalization/internationalization.h>
#include <api/utils/iginkgomatrix4x4.h>

#include <commands/comandocarga.h>

#include <main/entorno.h>

#include <main/controllers/controladorlog.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/pacscontroller.h>

#include "streamingloader.h"

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtkImageData.h>

#include <itkImage.h>

#include <itkCommand.h>
#include <itkMetaDataObject.h>

#include <itkImageFileReader.h>
#include <itkGDCMSeriesFileNames.h>
#include <itk/itkImageToVTKImageFilter.h>
#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>
#include <vtkAlgorithmOutput.h>
#include <vtkImageChangeInformation.h>

//#define DEBUG_PRINT_DIR_COSINES
#define USE_STREAMING true


GNC::StreamingLoader::StreamingLoader(): DirectionCosines(GNC::GCS::IGinkgoMatrix4x4::New())
{

	//gdcm::ImageHelper::SetForceRescaleInterceptSlope(true);
	//gdcm::ImageHelper::SetForcePixelSpacing(true);

	m_SignalFile = false;
	m_IO = ImageIO::New();
	m_IO->SetLoadPrivateTags(false);
	//Esto da leaks no activar
	//m_IO->SetLoadSequences(false);
	//
	m_IO->SetUseStreamedReading(USE_STREAMING);
	m_IO->SetMaxSizeLoadEntry(1024); // No permitir cargar campos de mas de 1K
	m_pOutput = vtkSmartPointer<vtkImageChangeInformation>::New();

	m_CurrentNumberOfComponents = 0;
	m_CurrentComponentType      = ImageIO::UNKNOWNCOMPONENTTYPE;
	m_CurrentPixelType          = ImageIO::UNKNOWNPIXELTYPE;
	m_CurrentDimensions[0] = 0;
	m_CurrentDimensions[1] = 0;
	m_CurrentDimensions[2] = 0;
	DirectionCosines->Identity();
	for (int i = 0; i < 3; i++) {
		m_Spacing[i] = 0.0;
		m_Origin[i] = 0.0;
	}
	
}

GNC::StreamingLoader::~StreamingLoader()
{
}

void GNC::StreamingLoader::SetInput(const std::string& fichero)
{

	GNC::GCS::ILocker(this);

	m_SignalFile = false;

	if (m_CurrentFile == fichero) {
		return;
	}

	try {
		m_CurrentFile = fichero;
		m_IO->SetFileName(fichero);
		m_IO->SetUseStreamedReading(true);


		//correct a bug because this dictionary doesn't clear between consecutive loads
		itk::MetaDataDictionary newDict;
		m_IO->SetMetaDataDictionary(newDict);
		//
		m_IO->ReadImageInformation();
		

		//std::string tagkey;
		//std::string GantryTilt;

		//itk::MetaDataDictionary & dict = m_IO->GetMetaDataDictionary();
		//tagkey = "0018|1120";
		//if( itk::ExposeMetaData<std::string>(dict, tagkey, GantryTilt ) )
		//{
		//	//std::cout << "Gantry Tilt = " << m_GantryTilt << std::endl;
		//}
		//// Check if the gantry tilt is within an acceptable range
		//double gantryTiltThreshold = 0.01;

		//std::stringstream sstr;
		//sstr.str( GantryTilt );
		//sstr >> m_GantryTilt;

		//if( fabs(m_GantryTilt) > gantryTiltThreshold )
		//{
		//	//std::cout << "Unacceptable gantry tilt" << std::endl;
		//}

		{
			#if defined(DEBUG_PRINT_DIR_COSINES)
			std::cout << "Dir. Cosines: " << std::endl;
			#endif
			for (int di = 0; di < 3; di++)
			{

				const std::vector<double> dir = m_IO->GetDirection(di);
				int l = 0;
				for (std::vector<double>::const_iterator it = dir.begin(); it != dir.end() && l < 3; ++it, l++) {
					#if defined(DEBUG_PRINT_DIR_COSINES)
					if (!l == 0) {
						std::cout << "," << *it;
					}
					else {
						std::cout << *it;
					}
					#endif
					DirectionCosines->SetElement(l, di, (*it));
				}
				#if defined(DEBUG_PRINT_DIR_COSINES)
				std::cout << std::endl;
				#endif

			}
//			char age[20];
			for (int i=0; i < 3; i++) {
				m_Spacing[i] = m_IO->GetSpacing(i);
				m_Origin[i]  = m_IO->GetOrigin(i);
			}
			//check if image data is coherent...
			itk::MetaDataDictionary& dict = m_IO->GetMetaDataDictionary();
			if (dict.HasKey("0028|0107") && dict.HasKey( "0028|0103") && dict.HasKey("0028|0102")) {
				//get highBit
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
                // GW: not clear what this is supposed to check
				if(itk::MetaDataObject<std::string>* highBit = dynamic_cast<itk::MetaDataObject <std::string> *>(dict["0028|0102"].GetPointer())) 
				{
					if(itk::MetaDataObject<std::string>* bitsStored = dynamic_cast<itk::MetaDataObject <std::string> *>(dict["0028|0101"].GetPointer())) 
					{
						//get largest value
						if(itk::MetaDataObject<std::string>* largestValue = dynamic_cast<itk::MetaDataObject <std::string> *>(dict["0028|0107"].GetPointer())) 
						{
							//get pixel representation
							if(itk::MetaDataObject<std::string>* pixelRepresentation = dynamic_cast<itk::MetaDataObject <std::string> *>(dict["0028|0103"].GetPointer())) 
							{
                                // GW: not clear what this is supposed to check
								long long minimum = 0;
								double rescaleIntercept = 0.0;
								double rescaleSlope = 1.0;
								long long paddingValue = 0;

								if(itk::MetaDataObject<std::string>* minimiumValue = dynamic_cast<itk::MetaDataObject<std::string>*>(dict["0028|0106"].GetPointer()))  {
									minimum = (long long) atol(minimiumValue->GetMetaDataObjectValue().c_str());
								}
								if(itk::MetaDataObject<std::string>* rescaleInterceptValue = dynamic_cast<itk::MetaDataObject <std::string>*>(dict["0028|1052"].GetPointer()))  {
									rescaleIntercept = (double) atof( rescaleInterceptValue->GetMetaDataObjectValue().c_str() );
								}
								if(itk::MetaDataObject <std::string> * rescaleSlopeValue = dynamic_cast<itk::MetaDataObject <std::string>*>(dict["0028|1053"].GetPointer()))  {
									rescaleSlope = (double) atof(rescaleSlopeValue->GetMetaDataObjectValue().c_str());
								}
								if(itk::MetaDataObject <std::string> * pixelPaddingValue = dynamic_cast<itk::MetaDataObject <std::string>*>(dict["0028|0120"].GetPointer()))  {
									paddingValue = (double) atol(pixelPaddingValue->GetMetaDataObjectValue().c_str());
								}
								if(itk::MetaDataObject <std::string> * pixelPaddingRangeLimitValue = dynamic_cast<itk::MetaDataObject <std::string>*>(dict["0028|0121"].GetPointer()))  {
									std::string val = pixelPaddingRangeLimitValue->GetMetaDataObjectValue();
									LOG_TRACE("ControladorCarga/CargaMultidimensional", "Pixel Padding Range Limit: " << val);
								}

								if (pixelRepresentation->GetMetaDataObjectValue() == "1") {//if its signed and largest value > (2^highbit)-1
									long long exp = (long long) atol(bitsStored->GetMetaDataObjectValue().c_str());
                                    if (exp < 1) {
                                        throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: Number of stored bits not positive") , "ControladorCarga/CargaMultidimensional");
                                    }
									long long limit = (1L << (exp-1));
									long long val = (long long) atol(largestValue->GetMetaDataObjectValue().c_str());
									long long maxVal = val;//(long long) (  (val * rescaleSlope) + rescaleIntercept );
									if (maxVal > limit ) {
										throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: Largest image pixel value data is not coherent with combination of Pixel Representation and High bit") , "ControladorCarga/CargaMultidimensional");
									}					 
								} else {
									long long exp = (long long) atol(bitsStored->GetMetaDataObjectValue().c_str());
									long long limit = (1L << (exp));
									long long val = (long long) atol(largestValue->GetMetaDataObjectValue().c_str());
									long long maxVal = val; //(long long) (  (val * rescaleSlope) + rescaleIntercept );
									if (maxVal > limit  ) {
										throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: Largest image pixel value data is not coherent with combination of Pixel Representation and High bit") , "ControladorCarga/CargaMultidimensional");
									}
								}
							}
						}
					}
				}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
			}//check if image data is coherent
		}

		if (m_CurrentNumberOfComponents != m_IO->GetNumberOfComponents() || m_CurrentComponentType != m_IO->GetComponentType() || m_CurrentPixelType != m_IO->GetPixelType() || m_CurrentDimensions[0] != m_IO->GetDimensions(0) || m_CurrentDimensions[1] != m_IO->GetDimensions(1) || m_CurrentDimensions[2] != m_IO->GetDimensions(2) )
		{
			RecomponerPipeline();
		}
		else {
			switch(m_IO->GetNumberOfComponents()) {
				case 1:
					{
						switch(m_IO->GetComponentType()) {
							case ImageIO::UCHAR:
								{
									typedef unsigned char TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::CHAR:
								{
									typedef char TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::USHORT:
								{
									typedef unsigned short TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::SHORT:
								{
									typedef unsigned short TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::UINT:
								{
									typedef unsigned int TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::INT:
								{
									typedef int TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::ULONG:
								{
									typedef unsigned long TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::LONG:
								{
									typedef long TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::FLOAT:
								{
									typedef float TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);
								}
								break;
							case ImageIO::DOUBLE:
								{
									typedef double TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::UNKNOWNCOMPONENTTYPE:
							default:
								m_SignalFile = true;
								LOG_ERROR("StreamingLoader", "Tipo de componente (pixeltype) no soportado en imagenes de 1 componente")
								break;
						}
					}
					break;
				case 3:
					{
						switch(m_IO->GetComponentType()) {
							case ImageIO::UCHAR:
								{
									typedef unsigned char TypedComponentType;
									typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::CHAR:
								{
									typedef char TypedComponentType;
									typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
									typedef itk::Image<TypedPixelType, 3 > TypedImageType;
									typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
									TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
									pTypedReader->SetFileName(fichero);

								}
								break;
							case ImageIO::USHORT:
							{
								typedef unsigned short TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::SHORT:
							{
								typedef short TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::UINT:
							{
								typedef unsigned int TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::INT:
							{
								typedef int TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::LONG:
							{
								typedef long TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::ULONG:
							{
								typedef unsigned long TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::FLOAT:
							{
								typedef float TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::DOUBLE:
							{
								typedef double TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								TypedReaderType* pTypedReader = static_cast<TypedReaderType*>(m_pReader.GetPointer());
								pTypedReader->SetFileName(fichero);
								
							}
								break;
							case ImageIO::UNKNOWNCOMPONENTTYPE:
							default:
								m_SignalFile = true;
								LOG_ERROR("StreamingLoader", "Tipo de componente (pixeltype) no soportado en imagenes de 3 componentes")
								break;
						}
					}
					break;
				default:
					m_SignalFile = true;
					LOG_ERROR("StreamingLoader", "Numero de componentes no soportado")
					break;
			}

			m_pReader->Modified();
			m_pPipelineJoiner->Modified();
			m_pOutput->Modified();
		}
	}
	catch (GNC::GCS::ControladorCargaException&)
	{
		m_SignalFile = true;
		if (m_pReader) {
			m_pReader->ResetPipeline();
		}
		throw;
	}
	catch (itk::ExceptionObject& ex)
	{
		if (m_pReader) {
			m_pReader->ResetPipeline();
		}

		m_SignalFile = true;
		
		// Try to read as signal
		GIL::DICOM::IDICOMManager* m = GIL::DICOM::PACSController::Instance()->CrearInstanciaDeDICOMManager();
		m->CargarFichero(m_CurrentFile);
		std::string modality;
		m->GetTag(0x0008, 0x0060, modality);
		//TODO: Mantener esta lectura del dataset.
		GIL::DICOM::PACSController::Instance()->LiberarInstanciaDeDICOMManager(m);

		if (modality == "ECG" || modality == "EC" || modality == "HD")
		{
			DirectionCosines->Identity();
			RecomponerPipeline();
			return;
		}
		else {
			throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: ") + ex.GetDescription(), "ControladorCarga/CargaMultidimensional");
		}
	}
	catch (...)
	{
		if (m_pReader) {
			m_pReader->ResetPipeline();
		}
		throw GNC::GCS::ControladorCargaException( _Std("Internal error reading the study, unknown format "), "ControladorCarga/CargaMultidimensional");
	}
}



vtkSmartPointer<vtkAlgorithmOutput> GNC::StreamingLoader::GetOutputPort()
{
	vtkSmartPointer<vtkAlgorithmOutput> out (m_pOutput->GetOutputPort());
	return out;
}

void GNC::StreamingLoader::GetOutputCopy(vtkSmartPointer<vtkImageData>& output)
{
	GNC::GCS::ILocker((GNC::GCS::IStreamingLoader*)this);
	output = vtkSmartPointer<vtkImageData>::New();
	m_pOutput->Update();
	output->DeepCopy(m_pOutput->GetOutput());

}

//------------------------------------------------------------------------------------------------
//region IPipelineProperties realization

void GNC::StreamingLoader::Update()
{
	m_pOutput->Update();
}

void GNC::StreamingLoader::UpdateInformation()
{
	m_pOutput->UpdateInformation();
}

const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::StreamingLoader::GetDirection()
{
	return DirectionCosines;
}

void GNC::StreamingLoader::GetOrigin(double origin[3])
{
	for (int i = 0; i < 3; i++) {
		origin[i] = m_Origin[i];
	}
}

void GNC::StreamingLoader::GetSpacing(double spacing[3])
{
	for (int i = 0; i < 3; i++) {
		spacing[i] = m_Spacing[i];
	}
}

void GNC::StreamingLoader::GetDimensions(int dimensions[3])
{
	for (int i = 0; i < 3; i++) {
		dimensions[i] = m_CurrentDimensions[i];
	}
}

const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::StreamingLoader::GetOutputDirection()
{
	return GetDirection();
}

void GNC::StreamingLoader::GetOutputOrigin(double origin[3])
{
	m_pOutput->GetOutputOrigin(origin);
}

void GNC::StreamingLoader::GetOutputSpacing(double spacing[3])
{
	m_pOutput->GetOutputSpacing(spacing);
}

void GNC::StreamingLoader::GetOutputDimensions(int dims[3])
{
	for (int i = 0; i<3; ++i)
	{
		dims[i] = m_CurrentDimensions[i];
	}
}

//endregion

void GNC::StreamingLoader::SetOutputSpacing(double spacing[3])
{
	if (spacing[0] <= 0 || spacing[1] <= 0) {
		spacing[0] = spacing[1] = 1.0;
	}
	m_pOutput->SetOutputSpacing(spacing);
}

void GNC::StreamingLoader::SetOutputOrigin(double origin[3])
{
	m_pOutput->SetOutputOrigin(origin);
}

bool GNC::StreamingLoader::GetDefaultWindowLevel(double &window, double &level)
{
	std::string windowTag, levelTag;
	bool ok = m_IO->GetValueFromTag("0028|1050", levelTag) && m_IO->GetValueFromTag("0028|1051", windowTag);
	if (ok) {
		{
			std::istringstream istr(levelTag);
			istr >> level;	
		}
		{
			std::istringstream istr(windowTag);
			istr >> window;	
		}		
	}
	return ok;	
}

std::string GNC::StreamingLoader::GetPatientPosition()
{
	std::string patientPositionTag;
	bool ok = m_IO->GetValueFromTag("0018|5100", patientPositionTag);
	if (ok) {
		return patientPositionTag;
	}
	return "";	
}

void GNC::StreamingLoader::RecomponerPipeline()
{
	if (m_SignalFile) {
		m_CurrentNumberOfComponents = 0;
		m_CurrentComponentType      = ImageIO::UNKNOWNCOMPONENTTYPE;
		m_CurrentPixelType          = ImageIO::SCALAR;
		m_CurrentDimensions[0]      = 800;
		m_CurrentDimensions[1]      = 800;
		m_CurrentDimensions[2]      = 1;
		m_pReader = NULL;
		m_pPipelineJoiner = NULL;
		m_pOutput->RemoveAllInputs();

		typedef unsigned short TypedPixelType;
		typedef itk::Image<TypedPixelType, 3 > TypedImageType;
		typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;
		
		TypedImageType::Pointer img = TypedImageType::New();
		TypedJoinerType::Pointer joiner = TypedJoinerType::New();
	
		m_pPipelineJoiner = joiner;
		joiner->SetInput(img);
		
		TypedImageType::SizeType size;
		TypedImageType::IndexType index;
		size[0] = 800;
		size[1] = 800;
		size[2] = 1;
		index[0] = index[1] = index[2] = 0;
		TypedImageType::RegionType region;
		region.SetSize(size);
		region.SetIndex(index);
		img->SetRegions(region);
		img->Allocate();
		img->FillBuffer(0);

		m_Origin[0] = m_Origin[1] = m_Origin[2] = 0.0;
		m_Spacing[0] = m_Spacing[1] = m_Spacing[2] = 1.0;

		m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
		m_pOutput->SetOutputOrigin(m_Origin);
		m_pOutput->SetOutputSpacing(m_Spacing);
	}
	else {
		m_CurrentNumberOfComponents = m_IO->GetNumberOfComponents();
		m_CurrentComponentType      = m_IO->GetComponentType();
		m_CurrentPixelType          = m_IO->GetPixelType();
		m_CurrentDimensions[0]      = m_IO->GetDimensions(0);
		m_CurrentDimensions[1]      = m_IO->GetDimensions(1);
		m_CurrentDimensions[2]      = m_IO->GetDimensions(2);
		m_pReader         = NULL;
		m_pPipelineJoiner = NULL;

		//m_pOutput->AbortExecuteOn();
		m_pOutput->RemoveAllInputs();

		ImageIO::Pointer pIO = ImageIO::New();
		pIO->SetLoadPrivateTags(false);
		//Esto da leaks no activar
		//pIO->SetLoadSequences(false);
		pIO->SetUseStreamedReading(USE_STREAMING);
		pIO->SetMaxSizeLoadEntry(1024); // No permitir cargar campos de mas de 1K
		switch(m_IO->GetNumberOfComponents()) {
			case 1:
				{
					switch(m_IO->GetComponentType()) {
						case ImageIO::UCHAR:
							{
								typedef unsigned char TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::CHAR:
							{
								typedef char TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::USHORT:
							{
								typedef unsigned short TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();


								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::SHORT:
							{
								typedef short TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();

							}
							break;
						case ImageIO::UINT:
							{
								typedef unsigned int TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::INT:
							{
								typedef int TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::ULONG:
							{
								typedef unsigned long TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::LONG:
							{
								typedef long TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::FLOAT:
							{
								typedef float TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::DOUBLE:
							{
								typedef double TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::UNKNOWNCOMPONENTTYPE:
						default:
							throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: unsupported pixel format"), "ControladorCarga/CargaMultidimensiona(1 componente)");
					}
				}
				break;
			case 3:
				{
					switch(m_IO->GetComponentType()) {
						case ImageIO::UCHAR:
							{
								typedef unsigned char TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::CHAR:
							{
								//std::cout << "RGB c" << std::endl;
								typedef unsigned char TypedComponentType;
								typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
								typedef itk::Image<TypedPixelType, 3 > TypedImageType;
								typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
								typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;

								TypedReaderType::Pointer reader = TypedReaderType::New();
								TypedJoinerType::Pointer joiner = TypedJoinerType::New();

								m_pReader = reader;
								m_pPipelineJoiner = joiner;

								joiner->SetInput(reader->GetOutput());

								reader->SetImageIO(pIO);
								reader->SetFileName(m_CurrentFile);
								reader->SetUseStreaming(USE_STREAMING);
								m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
								//reader->GetOutput()->ReleaseDataFlagOn();
							}
							break;
						case ImageIO::USHORT:
						{
							//std::cout << "RGB c" << std::endl;
							typedef unsigned short TypedComponentType;
							typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
							typedef itk::Image<TypedPixelType, 3 > TypedImageType;
							typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
							typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;
							
							TypedReaderType::Pointer reader = TypedReaderType::New();
							TypedJoinerType::Pointer joiner = TypedJoinerType::New();
							
							m_pReader = reader;
							m_pPipelineJoiner = joiner;
							
							joiner->SetInput(reader->GetOutput());
							
							reader->SetImageIO(pIO);
							reader->SetFileName(m_CurrentFile);
							reader->SetUseStreaming(USE_STREAMING);
							m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
							//reader->GetOutput()->ReleaseDataFlagOn();
						}
							break;
						case ImageIO::SHORT:
						{
							//std::cout << "RGB c" << std::endl;
							typedef short TypedComponentType;
							typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
							typedef itk::Image<TypedPixelType, 3 > TypedImageType;
							typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
							typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;
							
							TypedReaderType::Pointer reader = TypedReaderType::New();
							TypedJoinerType::Pointer joiner = TypedJoinerType::New();
							
							m_pReader = reader;
							m_pPipelineJoiner = joiner;
							
							joiner->SetInput(reader->GetOutput());
							
							reader->SetImageIO(pIO);
							reader->SetFileName(m_CurrentFile);
							reader->SetUseStreaming(USE_STREAMING);
							m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
							//reader->GetOutput()->ReleaseDataFlagOn();
						}
							break;
						case ImageIO::UINT:
						{
							//std::cout << "RGB c" << std::endl;
							typedef unsigned int TypedComponentType;
							typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
							typedef itk::Image<TypedPixelType, 3 > TypedImageType;
							typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
							typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;
							
							TypedReaderType::Pointer reader = TypedReaderType::New();
							TypedJoinerType::Pointer joiner = TypedJoinerType::New();
							
							m_pReader = reader;
							m_pPipelineJoiner = joiner;
							
							joiner->SetInput(reader->GetOutput());
							
							reader->SetImageIO(pIO);
							reader->SetFileName(m_CurrentFile);
							reader->SetUseStreaming(USE_STREAMING);
							m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
							//reader->GetOutput()->ReleaseDataFlagOn();
						}
							break;
						case ImageIO::INT:
						{
							//std::cout << "RGB c" << std::endl;
							typedef int TypedComponentType;
							typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
							typedef itk::Image<TypedPixelType, 3 > TypedImageType;
							typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
							typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;
							
							TypedReaderType::Pointer reader = TypedReaderType::New();
							TypedJoinerType::Pointer joiner = TypedJoinerType::New();
							
							m_pReader = reader;
							m_pPipelineJoiner = joiner;
							
							joiner->SetInput(reader->GetOutput());
							
							reader->SetImageIO(pIO);
							reader->SetFileName(m_CurrentFile);
							reader->SetUseStreaming(USE_STREAMING);
							m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
							//reader->GetOutput()->ReleaseDataFlagOn();
						}
							break;
						case ImageIO::FLOAT:
						{
							//std::cout << "RGB c" << std::endl;
							typedef float TypedComponentType;
							typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
							typedef itk::Image<TypedPixelType, 3 > TypedImageType;
							typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
							typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;
							
							TypedReaderType::Pointer reader = TypedReaderType::New();
							TypedJoinerType::Pointer joiner = TypedJoinerType::New();
							
							m_pReader = reader;
							m_pPipelineJoiner = joiner;
							
							joiner->SetInput(reader->GetOutput());
							
							reader->SetImageIO(pIO);
							reader->SetFileName(m_CurrentFile);
							reader->SetUseStreaming(USE_STREAMING);
							m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
							//reader->GetOutput()->ReleaseDataFlagOn();
						}
							break;
						case ImageIO::DOUBLE:
						{
							//std::cout << "RGB c" << std::endl;
							typedef double TypedComponentType;
							typedef itk::RGBPixel<TypedComponentType> TypedPixelType;
							typedef itk::Image<TypedPixelType, 3 > TypedImageType;
							typedef itk::ImageFileReader<TypedImageType> TypedReaderType;
							typedef itk::ImageToVTKImageFilter<TypedImageType> TypedJoinerType;
							
							TypedReaderType::Pointer reader = TypedReaderType::New();
							TypedJoinerType::Pointer joiner = TypedJoinerType::New();
							
							m_pReader = reader;
							m_pPipelineJoiner = joiner;
							
							joiner->SetInput(reader->GetOutput());
							
							reader->SetImageIO(pIO);
							reader->SetFileName(m_CurrentFile);
							reader->SetUseStreaming(USE_STREAMING);
							m_pOutput->SetInputConnection(joiner->GetImporter()->GetOutputPort());
							//reader->GetOutput()->ReleaseDataFlagOn();
						}
							break;
						case ImageIO::UNKNOWNCOMPONENTTYPE:
						default:
							throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: unsupported pixel format"), "ControladorCarga/CargaMultidimensional(3 componentes)");

					}
				}
				break;
			default:
				{
					throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: unsupported pixel format"), "ControladorCarga/CargaMultidimensional");
				}
		}
	}
	if (m_pReader.IsNotNull() && m_pPipelineJoiner.IsNotNull()) {
		m_pReader->Modified();
		
		m_pPipelineJoiner->Modified();
	}
	m_pOutput->Modified();
}
