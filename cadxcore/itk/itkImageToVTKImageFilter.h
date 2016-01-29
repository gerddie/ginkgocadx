/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToVTKImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007-11-20 18:46:10 +0100 (mar, 20 nov 2007) $
  Version:   $Revision: 477 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#pragma once

#include <itkVTKImageExport.h>
#include <vtkImageImport.h>
#include <vtkImageData.h>

namespace itk {

    /** \class ImageToVTKImageFilter
     * \brief Converts an ITK image into a VTK image and plugs a
     *  itk data pipeline to a VTK datapipeline.
     *
     *  This class puts together an itkVTKImageExporter and a vtkImageImporter.
     *  It takes care of the details related to the connection of ITK and VTK
     *  pipelines. The User will perceive this filter as an adaptor to which
     *  an itk::Image can be plugged as input and a vtkImage is produced as
     *  output.
     *
     * \ingroup   ImageFilters
     */
    template <class TInputImage >
    class ImageToVTKImageFilter : public ProcessObject {
    public:
	/** Standard class typedefs. */

	typedef ImageToVTKImageFilter Self;
	typedef ProcessObject Superclass;
	typedef SmartPointer<Self> Pointer;
	typedef SmartPointer<const Self> ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro(Self);

	/** Run-time type information (and related methods). */
	itkTypeMacro(ImageToVTKImageFilter, ProcessObject);

	/** Some typedefs. */
	typedef TInputImage InputImageType;
	typedef typename InputImageType::ConstPointer InputImagePointer;
	typedef VTKImageExport< InputImageType> ExporterFilterType;
	typedef typename ExporterFilterType::Pointer ExporterFilterPointer;

	/** Get the output in the form of a vtkImage.
	    This call is delegated to the internal vtkImageImporter filter  */
	vtkImageData * GetOutput() const;

	/** Set the input in the form of an itk::Image */
	void SetInput(const InputImageType *);

	/** Return the internal VTK image importer filter.
	    This is intended to facilitate users the access
	    to methods in the importer */
	vtkImageImport * GetImporter() const;

	/** Return the internal ITK image exporter filter.
	    This is intended to facilitate users the access
	    to methods in the exporter */
	ExporterFilterType * GetExporter() const;

	/** This call delegate the update to the importer */
	void Update();

    protected:
	ImageToVTKImageFilter();
	virtual ~ImageToVTKImageFilter();

    private:
	ImageToVTKImageFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	ExporterFilterPointer m_Exporter;
	vtkImageImport * m_Importer;

    };

    /**
     * Constructor
     */
    template <class TInputImage>
    ImageToVTKImageFilter<TInputImage>
    ::ImageToVTKImageFilter() {

	m_Importer = vtkImageImport::New();

	m_Exporter = ExporterFilterType::New();

	m_Importer->SetUpdateInformationCallback(m_Exporter->GetUpdateInformationCallback());
	m_Importer->SetPipelineModifiedCallback(m_Exporter->GetPipelineModifiedCallback());
	m_Importer->SetWholeExtentCallback(m_Exporter->GetWholeExtentCallback());
	m_Importer->SetSpacingCallback(m_Exporter->GetSpacingCallback());
	m_Importer->SetOriginCallback(m_Exporter->GetOriginCallback());
	m_Importer->SetScalarTypeCallback(m_Exporter->GetScalarTypeCallback());
	m_Importer->SetNumberOfComponentsCallback(m_Exporter->GetNumberOfComponentsCallback());
	m_Importer->SetPropagateUpdateExtentCallback(m_Exporter->GetPropagateUpdateExtentCallback());
	m_Importer->SetUpdateDataCallback(m_Exporter->GetUpdateDataCallback());
	m_Importer->SetDataExtentCallback(m_Exporter->GetDataExtentCallback());
	m_Importer->SetBufferPointerCallback(m_Exporter->GetBufferPointerCallback());
	m_Importer->SetCallbackUserData(m_Exporter->GetCallbackUserData());

    }

    /**
     * Destructor
     */
    template <class TInputImage>
    ImageToVTKImageFilter<TInputImage>
    ::~ImageToVTKImageFilter() {
	m_Importer->Delete();
    }

    /**
     * Set an itk::Image as input
     */
    template <class TInputImage>
    void ImageToVTKImageFilter<TInputImage>::SetInput(const InputImageType * inputImage) {
	m_Exporter->SetInput(inputImage);
    }

    /**
     * Get a vtkImage as output
     */
    template <class TInputImage>
    vtkImageData *ImageToVTKImageFilter<TInputImage>::GetOutput() const {
	return m_Importer->GetOutput();
    }

    /**
     * Get the importer filter
     */
    template <class TInputImage>
    vtkImageImport * ImageToVTKImageFilter<TInputImage>::GetImporter() const {
	return m_Importer;
    }

    /**
     * Get the exporter filter
     */
    template <class TInputImage>
    typename ImageToVTKImageFilter<TInputImage>::ExporterFilterType *ImageToVTKImageFilter<TInputImage>::GetExporter() const {
	return m_Exporter.GetPointer();
    }

    /**
     * Delegate the Update to the importer
     */
    template <class TInputImage>
    void ImageToVTKImageFilter<TInputImage>::Update() {
	m_Importer->Update();
    }


} // end namespace itk


