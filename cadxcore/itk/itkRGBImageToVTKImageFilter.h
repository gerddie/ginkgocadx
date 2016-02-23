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

#pragma once
#undef __DEPRECATED
#include <itkPixelTraits.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <itkImageConstIterator.h>
#include <itkImageLinearIteratorWithIndex.h>
#include <itkImageReverseConstIterator.h>
#include <itkImageIterator.h>
#include <itkImageReverseIterator.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionReverseConstIterator.h>

// FIXME: esta clase puede tener memmory leaks

namespace itk {

	typedef enum VTKScalarType {
		DOUBLE,
		FLOAT,
		LONG,
		UNSIGNED_LONG,
		INT,
		UNSIGNED_INT,
		SHORT,
		UNSIGNED_SHORT,
		CHAR,
		SIGNED_CHAR,
		UNSIGNED_CHAR
	} VTKScalarType;

	template <class TInputImage >
	class RGBImageToVTKImageFilter : public ProcessObject {
	public:
		/** Standard class typedefs. */
		typedef RGBImageToVTKImageFilter Self;
		typedef ProcessObject Superclass;
		typedef SmartPointer<Self> Pointer;
		//typedef SmartPointer<const Self>  ConstPointer;

		/** Method for creation through the object factory. */
		itkNewMacro(Self);

		/** Run-time type information (and related methods). */
		itkTypeMacro(ImageToVTKImageFilter, ProcessObject);

		/** Some typedefs. */
		typedef TInputImage InputImageType;
		typedef typename InputImageType::ConstPointer InputImagePointer;

		/** Get the output in the form of a vtkImage */
		vtkImageData * GetOutput() const;

		/** Set the input in the form of an itk::Image */
		void SetInput(const InputImageType *);

		/** Updates */
		void Update();

	protected:
		RGBImageToVTKImageFilter();
		virtual ~RGBImageToVTKImageFilter();

	private:
		RGBImageToVTKImageFilter(const Self&); // purposely not implemented
		void operator=(const Self&); // purposely not implemented

		InputImagePointer m_ImageInput;
		vtkImageData *m_ImageData;
		std::string m_ScalarTypeName;
		VTKScalarType m_ScalarType;
	};

	/**
	 * Constructor
	 */
	template <class TInputImage>RGBImageToVTKImageFilter<TInputImage>::RGBImageToVTKImageFilter()
	{
		m_ImageInput = NULL;
		typedef typename TInputImage::PixelType PixelType;
		typedef typename PixelTraits< PixelType >::ValueType ScalarType;

		if (typeid (ScalarType) == typeid (double)) {
			m_ScalarTypeName = "double";
			m_ScalarType = DOUBLE;
		} else if (typeid (ScalarType) == typeid (float)) {
			m_ScalarTypeName = "float";
			m_ScalarType = FLOAT;
		} else if (typeid (ScalarType) == typeid (long)) {
			m_ScalarTypeName = "long";
			m_ScalarType = LONG;
		} else if (typeid (ScalarType) == typeid (unsigned long)) {
			m_ScalarTypeName = "unsigned long";
			m_ScalarType = UNSIGNED_LONG;
		} else if (typeid (ScalarType) == typeid (int)) {
			m_ScalarTypeName = "int";
			m_ScalarType = INT;
		} else if (typeid (ScalarType) == typeid (unsigned int)) {
			m_ScalarTypeName = "unsigned int";
			m_ScalarType = UNSIGNED_INT;
		} else if (typeid (ScalarType) == typeid (short)) {
			m_ScalarTypeName = "short";
			m_ScalarType = SHORT;
		} else if (typeid (ScalarType) == typeid (unsigned short)) {
			m_ScalarTypeName = "unsigned short";
			m_ScalarType = UNSIGNED_SHORT;
		} else if (typeid (ScalarType) == typeid (char)) {
			m_ScalarTypeName = "char";
			m_ScalarType = CHAR;
		} else if (typeid (ScalarType) == typeid (unsigned char)) {
			m_ScalarTypeName = "unsigned char";
			m_ScalarType = UNSIGNED_CHAR;
		} else if (typeid (ScalarType) == typeid (signed char)) {
			m_ScalarTypeName = "signed char";
			m_ScalarType = SIGNED_CHAR;
		} else {
			itkExceptionMacro( << "Type currently not supported");
		}
		m_ImageData = NULL;
	}

	/**
	 * Destructor
	 */
	template <class TInputImage>RGBImageToVTKImageFilter<TInputImage>::~RGBImageToVTKImageFilter()
	{
		m_ImageData = NULL;
	}

	/**
	 * Set an itk::Image as input
	 */
	template <class TInputImage> void RGBImageToVTKImageFilter<TInputImage>::SetInput(const InputImageType * inputImage)
	{
		m_ImageInput = inputImage;
		if (m_ImageData != NULL) {
			m_ImageData->Delete();
		}
		m_ImageData = vtkImageData::New();


		typename TInputImage::RegionType img_region = m_ImageInput->GetLargestPossibleRegion();

		unsigned int imageNumComponents = img_region.GetImageDimension();
		typename TInputImage::RegionType::SizeType imageSize = img_region.GetSize();

		m_ImageData->SetDimensions(imageSize[0], imageSize[1], imageSize[2]);
		// TODO: Fijar Origen y espaciado
		m_ImageData->SetOrigin(0.0, 0.0, 0.0);
		m_ImageData->SetSpacing(1.0, 1.0, 1.0);
		switch (m_ScalarType) {
			case DOUBLE:
				m_ImageData->SetScalarTypeToDouble();
				break;
			case FLOAT:
				m_ImageData->SetScalarTypeToFloat();
				break;
			case LONG:
				m_ImageData->SetScalarTypeToLong();
				break;
			case UNSIGNED_LONG:
				m_ImageData->SetScalarTypeToUnsignedLong();
				break;
			case INT:
				m_ImageData->SetScalarTypeToInt();
				break;
			case UNSIGNED_INT:
				m_ImageData->SetScalarTypeToUnsignedInt();
				break;
			case SHORT:
				m_ImageData->SetScalarTypeToShort();
				break;
			case UNSIGNED_SHORT:
				m_ImageData->SetScalarTypeToUnsignedShort();
				break;
			case CHAR:
				m_ImageData->SetScalarTypeToChar();
				break;
			case SIGNED_CHAR:
				m_ImageData->SetScalarTypeToSignedChar();
				break;
			case UNSIGNED_CHAR:
				m_ImageData->SetScalarTypeToUnsignedChar();
				break;
			default:
				itkExceptionMacro( << "Type currently not supported");
				break;
		}
		m_ImageData->SetNumberOfScalarComponents(imageNumComponents);
		m_ImageData->AllocateScalars();
	}

	/**
	 * Get a vtkImage as output
	 */
	template <class TInputImage>vtkImageData *RGBImageToVTKImageFilter<TInputImage>::GetOutput() const
	{
		return m_ImageData;
	}

	/**
	 * Delegate the Update to the importer
	 */
	template <class TInputImage> void RGBImageToVTKImageFilter<TInputImage>::Update()
	{
		typename TInputImage::RegionType img_region = m_ImageInput->GetLargestPossibleRegion();

		unsigned int imageNumComponents = img_region.GetImageDimension();
		typename TInputImage::RegionType::SizeType imageSize = img_region.GetSize();

		itk::ImageLinearIteratorWithIndex<const TInputImage> it(m_ImageInput, img_region);

		double *tuple = new double[imageNumComponents];

		//int cid;
		typename TInputImage::PixelType pixel;

		vtkDataArray* scalars = m_ImageData->GetPointData()->GetScalars();

		unsigned long offset = 0;
		unsigned int cid;

		it.SetDirection(0);
		it.GoToBegin();

		while (!it.IsAtEnd()) {
			it.GoToBeginOfLine();
			while (!it.IsAtEndOfLine()) {
				pixel = it.Get();
				for (cid = 0; cid < pixel.GetNumberOfComponents(); ++cid) {
					tuple[cid] = pixel.GetNthComponent(cid);
				}
				scalars->SetTuple(offset++, tuple);
				++it;
			}
			it.NextLine();
		}
		/*

			int cid;
			y = 0;

			it.SetDirection(0);
			for (z = 0, it.GoToBegin(); !it.IsAtEnd() ; z++) {
				for (y = 0; !it.IsAtReverseEnd(); it.PreviousLine(), y++) {
					 it.GoToBeginOfLine();
					 for (x = 0; !it.IsAtEndOfLine(); ++it, ++x) {
						pixel = it.Get();
						for (cid = 0; cid < pixel.GetNumberOfComponents(); ++cid) {
							 tuple[cid] = pixel.GetNthComponent(cid);
						}
						m_ImageData->GetPointData()->GetScalars()->SetTuple(z * imageSize[1] + y * imageSize[0] + x, tuple);
					 }
				}
			}*/

		delete[] tuple;
	}

} // end namespace itk



