/*
 *  
 *  $Id: controladorautenticacion.h $
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
#include <string>
#include <list>

#include <api/api.h>
#include <api/controllers/icontroladorautenticacion.h>
#include <api/autoptr.h>


class wxSQLite3Database;
namespace GSEC {
	namespace Auth {
		typedef enum {
			TA_NO_LOGIN = 0,
			TA_USER_PASSWORD,
			TA_PASSWORD
		} TipoAutenticacion;

		class EXTAPI ControladorAutenticacion : public IControladorAutenticacion {
		public:

			static ControladorAutenticacion* Instance();
			static void FreeInstance();

			GNC::GCS::Ptr<wxSQLite3Database> IniciarSesionModificacionBBDDSeguridad();
			void GuardarCambiosBBDDSeguridad(GNC::GCS::Ptr<wxSQLite3Database> sesion);
			void CancelarCambiosYCerrarBBDDSeguridad(GNC::GCS::Ptr<wxSQLite3Database> sesion);


			TipoAutenticacion GetTipoAutenticacion();
			void SetTipoAutenticacion(TipoAutenticacion tipo);

			virtual bool Auth(const std::string& user, const std::string& password);

			bool InsertarUsuario(const std::string& user, const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

			bool CambiarPassword(const std::string& user, const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

			bool SetPasswordGeneral(const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion = NULL);

			bool EliminarUsuario(const std::string& user, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

			void GetAllUsers(std::list<std::string>& listaUsuarios, GNC::GCS::Ptr<wxSQLite3Database> sesion =NULL);

			bool ExisteUsuario(const std::string& user, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

			bool IsPasswordGeneralSetted(GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

			int GetMaxLongitudUsuario();			

		protected:
			ControladorAutenticacion();
			~ControladorAutenticacion();

			static ControladorAutenticacion* m_pInstance;

			bool AutenticacionUsuarioPassword(const std::string& usuario, const std::string& password);
			bool AutenticacionPassword(const std::string& password);

			std::string GetHashPassword(const std::string& password);
			GNC::GCS::Ptr<wxSQLite3Database> GetNewConexionSeguridad();

		};
	}
}
