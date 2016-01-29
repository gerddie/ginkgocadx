/*
 *  
 *  $Id: pacscontroller.h $
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

#include <api/api.h>
#include <api/controllers/ipacscontroller.h>

class DcmDataset;
class Network;
class DicomServer;
namespace GNC{
	class IProxyNotificadorProgreso;
}

namespace GIL {
	namespace DICOM {
		class Service;
	}
}

namespace GIL {
	namespace DICOM {
		class EXTAPI PACSController : public IPACSController {
		public:

			// TODO: corregir
			typedef std::map<void*, IDICOMImg2DCM*> ListaInstanciasImg2DCMType;
			
			typedef std::map<void*, Network> NetworkConnectionList;

			static PACSController* Instance();
			static void FreeInstance();

			virtual void SubirArchivos(void* connectionKey, const std::string& serverId, const std::list<std::string>& pathsSubida, GNC::IProxyNotificadorProgreso* pNotificador, TipoTransferSyntaxEnvio transferSyntax);

			virtual void FillInQuery(const GIL::DICOM::DicomDataset& base, DcmDataset* query, const GNC::GCS::Ptr<DicomServer>& server);
			
			virtual bool Print(
				void* connectionKey,
				const std::string& serverId,
				const GIL::DICOM::DicomDataset& film,
				const GIL::DICOM::DicomDataset& layout,
				const GIL::DICOM::DicomDataset& job,
				const std::list<std::string> files,
				GNC::IProxyNotificadorProgreso* pNotificador
			);

		protected:
			virtual bool ObtenerEstudio(
				void* connectionKey,
				const std::string& serverId,
				const GIL::DICOM::DicomDataset& base,
				IModeloDicom* pModelo,
				GNC::IProxyNotificadorProgreso* pNotificador,
				bool link
			);

			virtual bool ObtenerSerie(
				void* connectionKey,
				const std::string& serverId,
				const GIL::DICOM::DicomDataset& base,
				IModeloDicom* pModelo,
				GNC::IProxyNotificadorProgreso* pNotificador,
				bool link
			);

			virtual bool ObtenerImagen(
				void* connectionKey,
				const std::string& serverId,
				const GIL::DICOM::DicomDataset& base,
				IModeloDicom* pModelo,
				GNC::IProxyNotificadorProgreso* pNotificador
			);

			virtual bool DownloadWADOImages(const std::string& serverId, 
				const std::string& studyInstanceUID,
				const std::string& seriesInstanceUID,
				const std::list<std::string>& sopInstanceUID,
				const std::list<long>& instanceNumbers,
				IModeloDicom* pModelo,
				GNC::IProxyNotificadorProgreso* pNotificador,
				bool link);

		public:

			/**If there is an error throws exception GIL::DICOM::PACSException*/
			virtual bool DownloadFileFromURL(const std::string& url, 
				const std::string& filePath,
				GNC::IProxyNotificadorProgreso* pNotifier);

			/** Perform query */
			virtual bool Query(
				void* connectionKey,
				const std::string& abstractSyntax,
				const GIL::DICOM::DicomDataset& query,
				std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& resultsWraper,
				const std::string& serverId,
				GNC::IProxyNotificadorProgreso* pNotificador
			);

			virtual bool QueryRetrieve(
				void* connectionKey,
				const std::string& serverId,
				IModeloDicom* pModelo,
				const GIL::DICOM::DicomDataset& base,
				GNC::IProxyNotificadorProgreso* pNotificador,
				bool link
			);

			virtual void TestWADOURL(const std::string& url);

			virtual GIL::DICOM::ICustomAssociation* CreateCustomAssociation(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificador = NULL);
			virtual void DestroyCustomAssociation(GIL::DICOM::ICustomAssociation* assoc);

			virtual void StartServer();

			virtual void StopServer();

			virtual void  GetConnection(void* connectionKey);

			virtual void  ReleaseConnection(void* connectionKey);

			bool LeerDicomDir(const std::string& pathDicomDir, IModeloDicom* pModelo);

			virtual bool findAndGetTagFromFile(const std::string& ruta, unsigned short group, unsigned short element, std::string& value);

			virtual bool EsDICOM(const std::string& path, bool accept_dicomdir, bool accept_dicomfile) const;

			virtual bool EsPDF(const std::string& path) const;

			private:
			virtual bool GetRutaSerie(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, std::string& ruta, bool crearDirectorios = true, bool temporal = false);
			virtual bool GetRutaImagen(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, const std::string& uidImagen, std::string& ruta, bool crearDirectorios = true);
			public:
			virtual bool GetRutaImagenTemp(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, const std::string& uidImagen, std::string& ruta, bool crearDirectorios = true);

			void DumpFileElements(const std::string& rutaFichero, IInspectCallBack* callback);


//region "Métodos de Dicomización"

			IDICOMImg2DCM* CrearInstanciaDeImportacion();
			void LiberarInstanciaDeImportacion(IDICOMImg2DCM* instancia);

//endregion

//region "Métodos de carga/almacenamiento de DICOMS"

			IDICOMManager* CrearInstanciaDeDICOMManager();

			void LiberarInstanciaDeDICOMManager(IDICOMManager* instancia);

//endregion

		protected:
			PACSController();
			~PACSController();

			static void PurgarDirectorioTemporal();

		protected:

			static PACSController*     m_pInstance;
			ListaInstanciasImg2DCMType m_ListaInstanciasDicomImg2DCM;
			GIL::DICOM::Service*       m_pServiceInstance;

		private:
			void buildDicomDir();
		};
	};
};
