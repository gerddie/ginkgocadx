/*
 *  
 *  $Id: idicomizador.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
#include <api/controllers/ipacscontroller.h>

namespace GIL {
	namespace DICOM {
		
		typedef enum TDICOMOutputFormat {
			TDOF_AUTO, //vlp for img formats and sc for sc
			TDOF_FOO,
			TDOF_VLP,
			TDOF_SC,
			TDOF_NSC,
			TDOF_SR
		} TDICOMOutputFormat;
		
		class EXTAPI IDICOMImg2DCM {
		public:


			/* Inicializa una conversión. IInspectCallback sirve para especificar un callback mediante el cual se pueden inspeccionar los atributos buscados.
			 *
			 * Formatos de entrada soportados:
			 *	  JPEG
			 * Formatos de salida soportados (DICOM SOP a generar):
			 *   VLP (Visible Light Photography)
			 *   SC (Secondary Capture)
			 *   NSC (New Secondary Capture)
			 */
			typedef struct TImageFile {
				void* pixelData;
				int width;
				int heigth;
				int samplesPerPixel;
				std::string photometricInterpretation;
				int bitsAllocated;
				int bitsStored;
				int highBit;
				int pixelRepr;
				int planarConf;
				int pixAspectH;
				int pixAspectV;
				bool recompress;

				//configuration for rgbFiles uncompressed...
				TImageFile(void* pixelData, const int with, const int heigth)
				{
					Init(pixelData, with, heigth, 3, "RGB", 8, 8, 7, 0, 0, 1, 1, true);					
				}

				TImageFile(void* pixelData, const int width, const int heigth, const int samplesPerPixel, const std::string& photometricInterpretation, 
					const int bitsAllocated, const int bitsStored, const int highBit, const int pixelRepr, const int planarConf, const int pixAspectH, 
					const int pixAspectV, const bool recompress) 
				{
					Init(pixelData, width, heigth, samplesPerPixel, photometricInterpretation, bitsAllocated, bitsStored, highBit, pixelRepr,planarConf, pixAspectH, pixAspectV, recompress);
				}
				
				void Init(void* pixelData, const int width, const int heigth, const int samplesPerPixel, const std::string& photometricInterpretation, 
					const int bitsAllocated, const int bitsStored, const int highBit, const int pixelRepr, const int planarConf, const int pixAspectH, 
					const int pixAspectV, const bool recompress) 
				{
					this->pixelData = pixelData;
					this->width = width;
					this->heigth = heigth;
					this->samplesPerPixel = samplesPerPixel;
					this->photometricInterpretation = photometricInterpretation;
					this->bitsAllocated = bitsAllocated;
					this->bitsStored = bitsStored;
					this->highBit = highBit;
					this->pixelRepr = pixelRepr;
					this->planarConf = planarConf;
					this->pixAspectH = pixAspectH;
					this->pixAspectV = pixAspectV;
					this->recompress = recompress;
				}
			} TImageFile;

			virtual ~IDICOMImg2DCM() {}

			virtual void Inicializar(TImageFile& fileDefinition, IInspectCallBack* pICallback = NULL, bool debug = false, TDICOMOutputFormat outputFormat = TDOF_VLP) = 0;
			virtual void Inicializar(std::string& inputFile, IInspectCallBack* pICallback = NULL, bool debug = false, TDICOMOutputFormat outputFormat = TDOF_AUTO)=0;
			
			virtual int InsertarJerarquia(DicomDataset& base)=0;

			virtual int InsertarTagsPrivados(TipoPrivateTags& base) = 0;
			
			/* Actualiza los campos DICOM a sobreescribir */
			virtual void ActualizarCampos(IInspectCallBack* pICallback)=0;
			
			/* Realiza una conversión ya inicializada */
			virtual bool Convertir(std::string& outputFile)=0;

			virtual bool CrearSRDoc(std::string& outputFile, DicomDataset& base, std::list<GNC::GCS::Ptr<TipoPrivateTags> >& tagsPrivados) = 0;
			
			/* Resetea el estado interno */
			virtual void Reset()=0;
			
		};
	}
}
