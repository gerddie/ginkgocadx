/*
 *  
 *  $Id: dicommanager.h $
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
#include <api/globals.h>
#include <api/api.h>
#include <api/dicom/idicommanager.h>
#include <api/iproxynotificadorprogreso.h>

class DcmMetaInfo;
class DcmDataset;
class DcmFileFormat;
class DcmElement;
class OFCondition;
class DcmItem;
class DcmSequenceOfItems;

class wxCSConv;

namespace GIL
{
	namespace DICOM
	{

		class EXTAPI DICOMManager : public IDICOMManager
		{
		public:

			DICOMManager();
			DICOMManager(DcmDataset* dataset, const std::string& defaultCharset);

			~DICOMManager();

//region "Interfaz"
			/*Almacena la jerarquia cargada en un fichero*/
			virtual bool AlmacenarFichero(const std::string& inputFile, IInspectCallBack* pICallback = NULL, bool debug = false);

			/*Actualiza la jerarquia cargada*/
			virtual int ActualizarJerarquia(DicomDataset& base);

			/*actualiza los tags privados*/
			virtual int ActualizarTagsPrivados(TipoPrivateTags& tags);

			/*anonimiza los tags privados*/
			virtual void AnonimizarTagsPrivados();
			
			/*carga la MetaInfo*/
			virtual int CargarMetaInfo(const std::string& inputFile, GIL::DICOM::TipoMetaInfo& tags);

			/*carga los tags privados*/
			virtual int CargarTagsPrivados(GIL::DICOM::TipoPrivateTags& tags);

			/* Obtiene la jerarquia de campos DICOM del fichero */
			virtual bool CargarFichero(const std::string& inputFile, GIL::DICOM::DicomDataset& jerarquia, bool cargarSoloTagsInfo = true, IInspectCallBack* pICallback = NULL);

			/*Carga el fichero y responde a primitivas get tag, util para pillar tags especificos sin cargar toda la jerarquia*/
			virtual bool CargarFichero(const std::string&  inputFile, bool cargarSoloTagsInfo = true);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, std::string & valor);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, TagPrivadoUndefined& tagBinario);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, float& valor);
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, int& valor);virtual bool GetTag(unsigned int grupo,unsigned int elemento, GIL::DICOM::DicomDataset& secuencia);
			virtual bool ExtractTagToFile(unsigned int group, unsigned int element, const std::string& outputFile, GNC::IProxyNotificadorProgreso* pNotificador = NULL);

			virtual bool FindTag(unsigned int group,unsigned int element, TagPrivadoUndefined& tagBinario);
			
			/* returns the oid name or empty string if not found */
			virtual std::string GetOIDName(const std::string &oid);
			
			/* devuelve la descripcion de este tag*/
			virtual std::string GetDescription(const std::string &clave) const;
			

			virtual std::string GetNewUID();
//endregion

//region "External helpers"
			/*Comprueba el numero magico si es dicom*/
			static bool EsDicom(const std::string& inputFile);

			/* Crea un DCMElement parseando la cadena (g,e)=valor */
			static DcmElement* CrearElementoConValor(const char* s);

			/* Inserta la jerarquia en el dataset */
			int InsertarJerarquia(const DicomDataset& base, DcmItem* itemPadre, DcmSequenceOfItems* seqPadre);

			/* Carga la jerarquia desde el dataset */
			int CargarJerarquia(DicomDataset& base, unsigned int maximaLongitud, DcmElement * pElementoInicial = NULL);

			/*inserta los tags privados en el dataset*/
			int InsertarTagsPrivados(TipoPrivateTags& tags);
			
			/*carga los tags privados*/
			int PrivateCargarTagsPrivados(GIL::DICOM::TipoPrivateTags& tags);

			/*devuelve el identificador de elemento donde se deben almacenar los tags privados*/
			unsigned int GetElementIdentifier(GIL::DICOM::TipoPrivateTags& tags);

//endregion

//region "Internal helpers"

		private:
			void FindCharset(const std::string& defaultCharset = "ISO_IR 100");
			void CreateTempFile();
			void CleanupTempFile();
			DcmDataset* getSourceDataSet();
//endregion

		private:

			DcmFileFormat*  m_pDCMSourceFile;
			DcmDataset*     m_pDCMSourceDataset;
			wxCSConv* m_pConv;
		};

	}
}
