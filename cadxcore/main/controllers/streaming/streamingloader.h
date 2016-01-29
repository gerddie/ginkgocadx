/*
*  
*  $Id: streamingloader.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*
*/
#pragma once
#if defined _WINDOWS
#pragma warning(disable : 4702)
#endif

#include <api/api.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/ilock.h>
#include <itkProcessObject.h>
#include <itkGDCMImageIO.h>
#include <vtkSmartPointer.h>

//region "Forward Declarations"

class vtkImageData;
class vtkImageChangeInformation;

namespace GNC {
	namespace GCS {
		class IComando;
	}
}
//endregion

namespace GNC {

	class EXTAPI StreamingLoader : public GNC::GCS::IStreamingLoader {
	public:
		typedef itk::GDCMImageIO         ImageIO;
		typedef ImageIO::IOComponentType ImageComponentType;
		typedef ImageIO::IOPixelType     ImagePixelType;

		//region Constructor
		StreamingLoader();
		virtual ~StreamingLoader();

		virtual bool IsSignalFile() { return m_SignalFile; }
		virtual void SetInput(const std::string& fichero);

		//------------------------------------------------------------------------------------------------
		//region IPipelineProperties realization

		/** Updates **/
		virtual void Update();

		/** Updates output information **/
		virtual void UpdateInformation();

		/* Gets original (3D Volume) direction cosines */
		virtual const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GetDirection();

		/** Gets the original (3D Volume) origin **/
		virtual void GetOrigin(double origin[3]);

		/** Gets the original (3D Volume) spacing **/
		virtual void GetSpacing(double spacing[3]);

		/** Gets the original (3D Volume) dimensions **/
		virtual void GetDimensions(int dims[3]);

		/** Gets the slice direction cosines **/
		virtual const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GetOutputDirection();

		/** Gets the slice output origin **/
		virtual void GetOutputOrigin(double origin[3]);

		/** Gets the slice output spacing **/
		virtual void GetOutputSpacing(double spacing[3]);

		/** Gets the slice output dimensions **/
		virtual void GetOutputDimensions(int currentDimensions[3]);

		//endregion

		/** Sets the output spacing **/
		virtual void SetOutputSpacing(double spacing[3]);

		/** Sets the output origin **/
		virtual void SetOutputOrigin(double origin[3]);

		virtual std::string GetPatientPosition();

		virtual vtkSmartPointer<vtkAlgorithmOutput> GetOutputPort(); // Devuelve el puerto de salida
		virtual void GetOutputCopy(vtkSmartPointer<vtkImageData>& output);    // Devuelve por parametro una copia de la imagen
		virtual bool GetDefaultWindowLevel(double &window, double &level);

	protected:
		void CargarITK(GNC::GCS::IComando* cmd, int* orientacion, double* spacing = NULL);

		itk::ProcessObject::Pointer                m_pReader;
		itk::ProcessObject::Pointer                m_pPipelineJoiner;
		vtkSmartPointer<vtkImageChangeInformation> m_pOutput;
		GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>  DirectionCosines;
		double                                     m_Origin[3];
		double                                     m_Spacing[3];


	private:
		ImageIO::Pointer    m_IO;
		unsigned int        m_CurrentNumberOfComponents;
		ImageComponentType  m_CurrentComponentType;
		ImagePixelType      m_CurrentPixelType;
		unsigned int        m_CurrentDimensions[3];
		std::string         m_CurrentFile;
		bool                m_SignalFile;

		void RecomponerPipeline();
	};
}
