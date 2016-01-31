/*
 *  
 *  $Id: helpercompresion.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "helpercompresion.h"

#include <cstddef>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkCommand.h>
#include <itkMetaDataObject.h>
#include <itkMetaDataDictionary.h>
#include <itkImageSeriesReader.h>
#include <itkImageSeriesWriter.h>
#include <itkImageFileWriter.h>

//descomprime una imagen utilizando itk...
bool DecompressJPEG2000(const std::string& fileIn, const std::string& fOut)
{
	typedef double PixelType;
	typedef itk::Image<PixelType, 3 > ImageType;
	typedef itk::GDCMImageIO ImageIOType;
	typedef itk::ImageSeriesReader<ImageType> GenericReaderType;

	ImageIOType::Pointer              dicomIO    = ImageIOType::New();
	GenericReaderType::Pointer        reader     = GenericReaderType::New();

	try {
		reader->SetImageIO(dicomIO);

		//reader->ReleaseDataFlagOn();
		reader->SetFileName(fileIn);

		reader->UpdateOutputInformation();
	}
	catch (itk::ExceptionObject& /*ex*/)
	{
		reader->ResetPipeline();
		return false;
	}



	itk::ProcessObject::Pointer processObjectReader;
	itk::ProcessObject::Pointer processObjectWriter;
	ImageIOType::Pointer     tdicomIO = ImageIOType::New();
	tdicomIO->SetLoadPrivateTags(true);
	tdicomIO->SetLoadSequences(true);

	if (dicomIO->GetNumberOfComponents() == 1) {

		switch(dicomIO->GetComponentType()) {
		case ImageIOType::UCHAR:
			{
				typedef unsigned char TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::CHAR:
			{
				typedef char TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::USHORT:
			{
				typedef unsigned short TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::SHORT:
			{
				typedef short TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::UINT:
			{
				typedef unsigned int TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::INT:
			{
				typedef int TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::ULONG:
			{
				typedef unsigned long TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::LONG:
			{
				typedef long TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::FLOAT:
			{
				typedef float TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::DOUBLE:
			{
				typedef double TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::UNKNOWNCOMPONENTTYPE:
		default:
			//throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: unsupported pixel format"), "ControladorCarga/CargaMultidimensional");
			return false;
		}
	}
	else if (dicomIO->GetNumberOfComponents() == 3) {
		switch(dicomIO->GetComponentType()) {
		case ImageIOType::UCHAR:
			{
				typedef itk::RGBPixel<unsigned char> TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		default:
			return false;
		}

	}
	else {
		return false;
	}

	try {
		processObjectReader->Update();
		//descomprimir
		tdicomIO->SetUseCompression(false);
		tdicomIO->SetByteOrder(itk::ImageIOBase::LittleEndian);
		//
		processObjectWriter->Update();
	}
	catch (itk::ExceptionObject& /*ex*/)
	{
		processObjectReader->ResetPipeline();
		return false;
	}
	catch (...)
	{
		processObjectReader->ResetPipeline();
		return false;
	}
	/*if (processObjectReader->GetAbortGenerateData()) {
	processObject->ResetPipeline();
	return false;
	}*/
	return true;
}



//descomprime una imagen utilizando itk...
bool CompressJPEG2000(const std::string& fileIn, const std::string& fOut)
{
	typedef double PixelType;
	typedef itk::Image<PixelType, 3 > ImageType;
	typedef itk::GDCMImageIO ImageIOType;
	typedef itk::ImageSeriesReader<ImageType> GenericReaderType;

	ImageIOType::Pointer              dicomIO    = ImageIOType::New();
	GenericReaderType::Pointer        reader     = GenericReaderType::New();

	try {
		reader->SetImageIO(dicomIO);

		reader->SetUseStreaming(true);
		//reader->ReleaseDataFlagOn();
		reader->SetFileName(fileIn);

		reader->UpdateOutputInformation();
	}
	catch (itk::ExceptionObject& /*ex*/)
	{
		reader->ResetPipeline();
		return false;
	}



	itk::ProcessObject::Pointer processObjectReader;
	itk::ProcessObject::Pointer processObjectWriter;
	ImageIOType::Pointer     tdicomIO = ImageIOType::New();
	tdicomIO->SetLoadPrivateTags(true);
	tdicomIO->SetLoadSequences(true);
	if (dicomIO->GetNumberOfComponents() == 1) {

		switch(dicomIO->GetComponentType()) {
		case ImageIOType::UCHAR:
			{
				typedef unsigned char TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::CHAR:
			{
				typedef char TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::USHORT:
			{
				typedef unsigned short TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::SHORT:
			{
				typedef short TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::UINT:
			{
				typedef unsigned int TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::INT:
			{
				typedef int TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::ULONG:
			{
				typedef unsigned long TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::LONG:
			{
				typedef long TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::FLOAT:
			{
				typedef float TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::DOUBLE:
			{
				typedef double TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);

				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		case ImageIOType::UNKNOWNCOMPONENTTYPE:
		default:
			//throw GNC::GCS::ControladorCargaException( _Std("Error reading the study: unsupported pixel format"), "ControladorCarga/CargaMultidimensional");
			return false;
		}
	}
	else if (dicomIO->GetNumberOfComponents() == 3) {
		switch(dicomIO->GetComponentType()) {
		case ImageIOType::UCHAR:
			{
				typedef itk::RGBPixel<unsigned char> TypedPixelType;
				typedef itk::Image<TypedPixelType, 3 > TypedImageType;
				typedef itk::ImageSeriesReader<TypedImageType> TypedReaderType;
				typedef itk::ImageSeriesWriter<TypedImageType,TypedImageType> TypedWriterType;

				TypedReaderType::Pointer treader = TypedReaderType::New();
				TypedWriterType::Pointer twriter = TypedWriterType::New();
				treader->SetImageIO(tdicomIO);
				treader->SetFileName(fileIn);
				treader->SetUseStreaming(true);
				//treader->GetOutput()->ReleaseDataFlagOn();

				twriter->SetInput(treader->GetOutput());
				twriter->SetImageIO(tdicomIO);
				twriter->SetFileName(fOut);
				twriter->UseCompressionOn();

				processObjectReader = treader;
				processObjectWriter = twriter;
			}
			break;
		default:
			return false;
		}
	}
	else{
		return false;
	}

	try {
		processObjectReader->Update();
		//comprimir
		tdicomIO->SetByteOrderToLittleEndian();
		tdicomIO->SetUseCompression(true);
		tdicomIO->SetLoadPrivateTags(true);
		tdicomIO->SetLoadSequences(true);

		tdicomIO->SetCompressionType(itk::GDCMImageIO::JPEG2000);
		//
		processObjectWriter->Update();
	}
	catch (itk::ExceptionObject& /*ex*/)
	{
		processObjectReader->ResetPipeline();
		return false;
	}
	catch (...)
	{
		processObjectReader->ResetPipeline();
		return false;
	}
	/*if (processObjectReader->GetAbortGenerateData()) {
	processObject->ResetPipeline();
	return false;
	}*/
	return true;
}
