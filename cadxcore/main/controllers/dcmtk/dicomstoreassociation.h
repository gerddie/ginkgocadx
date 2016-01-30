/*
 *  
 *  $Id: dicomstoreassociation.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *  Code adapted from OsiriX
 *
 */
#pragma once
#include <string>
#include <vector>
#include <api/controllers/ipacscontroller.h>
#include <dcmtk/dcmnet/dimse.h>
class DicomServer;

namespace GNC{
	class IProxyNotificadorProgreso;
}

namespace GIL {
	namespace DICOM {


		class DicomStoreAssociation{
		public:

			DicomStoreAssociation();
			~DicomStoreAssociation();
			typedef std::list<std::string> ListaRutas;
			void Store(ListaRutas listaFicheros, const GNC::GCS::Ptr<DicomServer>& server, std::string nombreAETLocal,GNC::IProxyNotificadorProgreso* pNotificador, GIL::DICOM::TipoTransferSyntaxEnvio transferSyntax);
			void FreeMemory();
			static void progressCallback(void * /*callbackData*/,T_DIMSE_StoreProgress *progress,T_DIMSE_C_StoreRQ * /*req*/);
			
			void SetTLS(const std::string& CliCert, const std::string& CliKey, bool validate) { m_TLS = true; m_CliCert = CliCert; m_CliKey = CliKey; m_Validate = validate; }

			void SetUserPass(const std::string& PacsUser, const std::string& PacsPass) { 
				m_UseUserPass = true; m_pacsUser = PacsUser, m_pacsPass = PacsPass; 
			}
			bool UseUserPass() { return m_UseUserPass; }
			const std::string& GetUser() {return m_pacsUser;}
			const std::string& GetPass() {return m_pacsPass;}
			
			bool IsSecure() const { return m_TLS; }
			
			const std::string& GetCliCert() const { return m_CliCert; }
			const std::string& GetCliKey() const { return m_CliKey; }
			 bool GetValidate() const { return m_Validate; }
			
		protected:
			bool m_TLS;
			std::string m_CliCert;
			std::string m_CliKey;
			bool m_UseUserPass;
			std::string m_pacsUser;
			std::string m_pacsPass;
			bool m_Validate;
			T_ASC_Association *assoc;			
			T_ASC_Network *net;
			
		};
	}
}
