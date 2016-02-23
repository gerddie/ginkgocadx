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

#include <ostream>
#include <string>
#include <exception>
#include <vector>
#include <list>
#include <map>

#include <api/api.h>
#include <api/controllers/imodulecontroller.h>
#include <api/ivista.h>
#include <api/dicom/idicom.h>
#include <api/iproxynotificadorprogreso.h>
#include <api/iexception.h>

class IModeloDicom;
class DcmElement;

namespace GIL {
	namespace DICOM {
		class IInspectCallBack;
	}
}

namespace GIL {
	namespace DICOM {		
		class ICustomAssociation;

		/** PACS Exception **/
		class EXTAPI PACSException : public GNC::GCS::IException {
		public:
			/** PACS Exception constructor **/
			PACSException(const std::string& msg, const std::string& component="GIL/PACS", bool fatal = true) throw() ;
		};

		typedef struct ContextoResultado {
			bool error;
			std::string UIDPaciente;
			std::string UIDEstudio;
			std::list<std::string> RutaDICOM;
			std::string ServidorPACSID;
			bool fromPACS;
			bool recursivo;

			inline ContextoResultado()
			{
				error = false;
				fromPACS=false;
				recursivo = false;
			}

			inline ~ContextoResultado()
			{
			}
		} ContextoResultado;

		

		typedef enum TipoTransferSyntaxEnvio {
			SendExplicitLittleEndian,
			SendImplicitLittleEndian,
			SendJPEG2000Lossless,
			SendJPEG2000Lossy10,
			SendJPEG2000Lossy20,
			SendJPEG2000Lossy50,
			SendJPEGLossless,
			SendJPEGLossy9,
			SendJPEGLossy8,
			SendJPEGLossy7,
			SendDefault
		} TipoTransferSyntaxEnvio;

		class EXTAPI IPACSController {
		public:

			virtual ~IPACSController();
			
			virtual bool QueryRetrieve(
				void* connectionKey,
				const std::string& serverId,
				IModeloDicom* pModelo,
				const GIL::DICOM::DicomDataset& base,
				GNC::IProxyNotificadorProgreso* pNotificador,
				bool link
			) = 0;
			
			virtual bool Print(
				void* connectionKey,
				const std::string& serverId,
				const GIL::DICOM::DicomDataset& film,
				const GIL::DICOM::DicomDataset& layout,
				const GIL::DICOM::DicomDataset& job,
				const std::list<std::string> files,
				GNC::IProxyNotificadorProgreso* pNotificador
			) = 0;


			/** Perform query */
			virtual bool Query(
				void* connectionKey,
				const std::string& abstractSyntax,
				const GIL::DICOM::DicomDataset& query,
				std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& resultsWraper,
				const std::string& serverId,
				GNC::IProxyNotificadorProgreso* pNotificador
			) = 0;

			/** Test WADO URL **/
			virtual void TestWADOURL(const std::string& url) = 0;

			/** Downloads url to specified file */
			virtual bool DownloadFileFromURL(const std::string& url, 
				const std::string& filePath,
				GNC::IProxyNotificadorProgreso* pNotifier) = 0;

			virtual GIL::DICOM::ICustomAssociation* CreateCustomAssociation(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificador = NULL) = 0;
			virtual void DestroyCustomAssociation(GIL::DICOM::ICustomAssociation* assoc) = 0;

			virtual void SubirArchivos(void* connectionKey, const std::string& serverId, const std::list<std::string>& pathsSubida, GNC::IProxyNotificadorProgreso * pNotificador, TipoTransferSyntaxEnvio transferSyntax) =0;

			virtual void GetConnection(void* connectionKey) = 0;

			virtual void ReleaseConnection(void* connectionKey) = 0;

			virtual bool LeerDicomDir(const std::string& pathDicomDir, IModeloDicom* pModelo) = 0;

			virtual bool findAndGetTagFromFile(const std::string& ruta, unsigned short group, unsigned short element, std::string& value) = 0;

			virtual bool EsDICOM(const std::string& path, bool accept_dicomdir, bool accept_dicomfile) const = 0;
			
			virtual bool EsPDF(const std::string& path) const = 0;

			/* Devuelve la ruta en disco (asignada a la variable ruta pasada por referencia) en base a los IDs unicos de estudio y serie de una serie de imagenes obtenida*/
			/*virtual bool GetRutaImagen(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, const std::string& uidImagen, const std::string& dicomDirPath, std::string& ruta, bool crearDirectorios = true) = 0;*/
			virtual bool GetRutaImagenTemp(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, const std::string& uidImagen, std::string& ruta, bool crearDirectorios = true) = 0;

			void DumpFileElements(const std::string& rutaFichero, IInspectCallBack* callback);

	//region "Metodos de Dicomizacion"

			virtual IDICOMImg2DCM* CrearInstanciaDeImportacion() = 0;

			virtual void LiberarInstanciaDeImportacion(IDICOMImg2DCM* instancia) = 0;

	//endregion

	//region "Metodos de carga/almacenamiento de DICOMS"

			virtual IDICOMManager* CrearInstanciaDeDICOMManager() = 0;

			virtual void LiberarInstanciaDeDICOMManager(IDICOMManager* instancia) = 0;

	//endregion

		protected:

			IPACSController();
		};
	}
}
