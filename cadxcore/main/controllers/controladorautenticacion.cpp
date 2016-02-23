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

#include <string>
#include <openssl/sha.h>
#include <sstream>

#include "controladorautenticacion.h"
#include "controladorpermisos.h"
#include "controladorlog.h"
#include <sqlite/seguridad.h>
#include <main/entorno.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/filename.h>


#define SALT "654w34qt5sdf/*-/435609'zvasd?asf"
#define USUARIO_MODO_PASSWORD ""

GSEC::Auth::ControladorAutenticacion* GSEC::Auth::ControladorAutenticacion::m_pInstance = NULL;

GSEC::Auth::ControladorAutenticacion::ControladorAutenticacion()
{
}

GSEC::Auth::ControladorAutenticacion::~ControladorAutenticacion()
{
}

GSEC::Auth::ControladorAutenticacion* GSEC::Auth::ControladorAutenticacion::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new ControladorAutenticacion();
	}
	return m_pInstance;
}

void GSEC::Auth::ControladorAutenticacion::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}			
}

GNC::GCS::Ptr<wxSQLite3Database> GSEC::Auth::ControladorAutenticacion::IniciarSesionModificacionBBDDSeguridad()
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion = GetNewConexionSeguridad();
	pConexion->Begin();
	return pConexion;
}

void GSEC::Auth::ControladorAutenticacion::GuardarCambiosBBDDSeguridad(GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	sesion->Commit();
	sesion->Begin();
}

void GSEC::Auth::ControladorAutenticacion::CancelarCambiosYCerrarBBDDSeguridad(GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	sesion->Rollback();
	sesion->Close();
}


GSEC::Auth::TipoAutenticacion GSEC::Auth::ControladorAutenticacion::GetTipoAutenticacion()
{
	//se busca en la configuracion...
	int secMode;
	GNC::GCS::ConfigurationController::Instance()->readIntGeneral("/GinkgoCore/Security", "SecMode", secMode, (int) TA_NO_LOGIN);

	switch (secMode) {
		case TA_NO_LOGIN:
			return TA_NO_LOGIN;
		case TA_USER_PASSWORD:
			return TA_USER_PASSWORD;
		case TA_PASSWORD:
			return TA_PASSWORD;
		default:
			return TA_NO_LOGIN;
	}
}

void GSEC::Auth::ControladorAutenticacion::SetTipoAutenticacion(GSEC::Auth::TipoAutenticacion tipo)
{
	GNC::GCS::ConfigurationController::Instance()->writeIntGeneral("/GinkgoCore/Security", "SecMode", (int) tipo);
}

bool GSEC::Auth::ControladorAutenticacion::Auth(const std::string& usuario, const std::string& password)
{
	TipoAutenticacion ta = GetTipoAutenticacion();
	if (ta == TA_PASSWORD) {
		return AutenticacionPassword(password);
	} else if(ta == TA_USER_PASSWORD) {
		return AutenticacionUsuarioPassword(usuario, password);
	} else {
		return true;
	}
}

bool GSEC::Auth::ControladorAutenticacion::InsertarUsuario(const std::string& user, const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion;
	
	if(sesion.IsValid()) {
		pConexion = sesion;
	} else {
		pConexion = GetNewConexionSeguridad();
	}

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return false;
	}

	bool resultado = false;
	try {
		wxSQLite3StatementBuffer bufSQL;
		std::string passwordHash = GetHashPassword(password);
		bufSQL.Format("INSERT INTO UserPass (User,Password) VALUES ('%q','%q')",user.c_str(),passwordHash.c_str());
		
		if(pConexion->ExecuteUpdate(bufSQL)>0){
			resultado = true;
		} 
	}catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al insertar el usuario " << ex.GetMessage().ToUTF8());
	}

	if(!sesion.IsValid()) {
		pConexion->Close();
	}
	return resultado;
}


bool GSEC::Auth::ControladorAutenticacion::CambiarPassword(const std::string& user, const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion;
	
	if(sesion.IsValid()) {
		pConexion = sesion;
	} else {
		pConexion = GetNewConexionSeguridad();
	}

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return false;
	}

	bool resultado = false;
	try {
		wxSQLite3StatementBuffer bufSQL;
		std::string passwordHash = GetHashPassword(password);
		bufSQL.Format("UPDATE UserPass SET Password='%q' WHERE user='%q'",passwordHash.c_str(), user.c_str());
		
		if(pConexion->ExecuteUpdate(bufSQL)>0){
			resultado = true;
		} 
	}catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al insertar el usuario " << ex.GetMessage().ToUTF8());
	}

	if(!sesion.IsValid()) {
		pConexion->Close();
	}
	return resultado;
}

bool GSEC::Auth::ControladorAutenticacion::SetPasswordGeneral(const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion;
	
	if(sesion.IsValid()) {
		pConexion = sesion;
	} else {
		pConexion = GetNewConexionSeguridad();
	}

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return false;
	}

	bool resultado = false;
	try {
		wxSQLite3StatementBuffer bufSQL;
		std::string passwordHash = GetHashPassword(password);
		bufSQL.Format("INSERT OR REPLACE INTO UserPass (User,Password) VALUES ('%q','%q')",USUARIO_MODO_PASSWORD,passwordHash.c_str());
		
		if(pConexion->ExecuteUpdate(bufSQL)>0){
			resultado = true;
		} 
	}catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al establecer password general " << ex.GetMessage().ToUTF8());
	}

	if(!sesion.IsValid()) {
		pConexion->Close();
	}
	return resultado;
}

bool GSEC::Auth::ControladorAutenticacion::EliminarUsuario(const std::string& user, GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion;
	
	if(sesion.IsValid()) {
		pConexion = sesion;
	} else {
		pConexion = GetNewConexionSeguridad();
	}

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return false;
	}

	bool resultado = false;
	try {
		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("DELETE FROM UserPass WHERE User = '%q'",user.c_str());
		
		if(pConexion->ExecuteUpdate(bufSQL)>0){
			resultado = true;
		} 
	}catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al eliminar el usuario de la base de datos " << ex.GetMessage().ToUTF8());
	}

	if(!sesion.IsValid()) {
		pConexion->Close();
	}
	return resultado;
}

void GSEC::Auth::ControladorAutenticacion::GetAllUsers(std::list<std::string>& listaUsuarios, GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion;
	
	if(sesion.IsValid()) {
		pConexion = sesion;
	} else {
		pConexion = GetNewConexionSeguridad();
	}

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return;
	}

	try {
		wxSQLite3StatementBuffer bufSQL;
			bufSQL.Format("SELECT User "
				"FROM UserPass WHERE User <> '%q'", USUARIO_MODO_PASSWORD);
		wxSQLite3ResultSet resultados = pConexion->ExecuteQuery(bufSQL);
		while (resultados.NextRow()) {
			const std::string user(resultados.GetAsString(0).ToUTF8());
			listaUsuarios.push_back(user);
		}
	} catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al realizar la consulta de usuarios " << ex.GetMessage().ToUTF8());
	}
	if(!sesion.IsValid()) {
		pConexion->Close();
	}
}

bool GSEC::Auth::ControladorAutenticacion::ExisteUsuario(const std::string& user, GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion;
	
	if(sesion.IsValid()) {
		pConexion = sesion;
	} else {
		pConexion = GetNewConexionSeguridad();
	}

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return false;
	}

	bool resultado = false;
	try {
		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("SELECT count(*) FROM UserPass WHERE User='%q'", user.c_str());
		resultado = pConexion->ExecuteScalar(bufSQL) > 0;
	} catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al realizar la consulta IsPasswordGeneralSetted " << ex.GetMessage().ToUTF8());
	}
	if(!sesion.IsValid()) {
		pConexion->Close();
	}
	return resultado;
}

bool GSEC::Auth::ControladorAutenticacion::IsPasswordGeneralSetted(GNC::GCS::Ptr<wxSQLite3Database> sesion)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion;
	
	if(sesion.IsValid()) {
		pConexion = sesion;
	} else {
		pConexion = GetNewConexionSeguridad();
	}

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return false;
	}

	bool resultado = false;
	try {
		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("SELECT count(*) FROM UserPass WHERE User='%q'", USUARIO_MODO_PASSWORD);
		resultado = pConexion->ExecuteScalar(bufSQL) > 0;
	} catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al realizar la consulta IsPasswordGeneralSetted " << ex.GetMessage().ToUTF8());
	}
	if(!sesion.IsValid()) {
		pConexion->Close();
	}
	return resultado;
}

int GSEC::Auth::ControladorAutenticacion::GetMaxLongitudUsuario()
{
	return 20;
}

bool GSEC::Auth::ControladorAutenticacion::AutenticacionUsuarioPassword(const std::string& usuario, const std::string& password)
{
	GNC::GCS::Ptr<wxSQLite3Database> pConexion = GetNewConexionSeguridad();

	if(!pConexion.IsValid())
	{
		//ha habido error porque no se ha podido abrir la bbdd de seguridad, asumimos que no hay ni users ni passwords ni na
		return true;
	}

	bool resultado = false;

	try {
		wxSQLite3StatementBuffer bufSQL;
			bufSQL.Format("SELECT Password "
				"FROM UserPass WHERE "
				"User = '%q'",usuario.c_str());
		wxSQLite3ResultSet resultados = pConexion->ExecuteQuery(bufSQL);
		if (resultados.NextRow()) {
			const std::string passWordBBDD(resultados.GetAsString(0).ToUTF8());
			resultado = passWordBBDD == GetHashPassword(password);
		}
	} catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al realizar la consulta de password " << ex.GetMessage().ToUTF8());
	}
	//si ha habido errores... devolvemos true para que siga
	pConexion->Close();
	if (resultado) {
		LOG_INFO("Core/Autenticacion", "El usuario " << usuario << " se ha autenticado correctamente");
	} else {
		LOG_INFO("Core/Autenticacion", "El usuario " << usuario << " ha introducido un password inválido");
	}

	return resultado;
}

bool GSEC::Auth::ControladorAutenticacion::AutenticacionPassword(const std::string& password)
{
	return AutenticacionUsuarioPassword(USUARIO_MODO_PASSWORD, password);
}


std::string GSEC::Auth::ControladorAutenticacion::GetHashPassword(const std::string& password)
{
	std::ostringstream ostr;
	ostr << password;
	//salt 
	ostr << SALT;
	std::string cadenaEntrada = ostr.str();

	unsigned char salida[SHA512_DIGEST_LENGTH];
	SHA512((const unsigned char*)cadenaEntrada.c_str(), cadenaEntrada.size(), salida);

	std::ostringstream cadenaSHA;
	cadenaSHA.setf ( std::ios::hex, std::ios::basefield );
	for(int i = 0; i<SHA512_DIGEST_LENGTH; i++)
	{
		cadenaSHA << (int)salida[i];
	}
	return cadenaSHA.str();
}

GNC::GCS::Ptr<wxSQLite3Database> GSEC::Auth::ControladorAutenticacion::GetNewConexionSeguridad()
{
	wxSQLite3Database* pConexion = new wxSQLite3Database();
	try{
		//pillamos la bbdd
		std::string pathFicheroDB;
		std::ostringstream ostr;
		ostr << GNC::Entorno::Instance()->GetGinkgoUserDir() << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgosec.db";
		pathFicheroDB = ostr.str();
		wxString wxDirectorio = FROMPATH(GNC::Entorno::Instance()->GetGinkgoUserDir());
		
		if(!wxDirExists(wxDirectorio)){
			#ifdef _WIN32
			bool correcto = wxMkdir(wxDirectorio,511);
			#else
			bool correcto = wxMkDir(wxDirectorio.ToUTF8(), 0770) == -1? false : true;
			#endif
			if(!correcto){
			    //std::cout << "Error al crear el directorio " << wxDirectorio.ToUTF8() << std::endl;
				//se escribe en tmp...
				LOG_WARN("Core/BBDD", "No tiene permisos para escribir en el directorio del usuario, esto hace que la seguridad deje de funcionar dado que no puede crearse la base de datos")
				std::ostringstream ostr2;
				ostr2 << GNC::Entorno::Instance()->GetGinkgoTempDir() << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgosec.db"<<std::ends;
				pathFicheroDB = ostr2.str();
				return NULL;
			}
		}

		//existe??
		if(!wxFileExists(FROMPATH(pathFicheroDB))) {
			//se crea el fichero de base de datos
			CreateSeguridadDB(*pConexion,pathFicheroDB.c_str());
		}

		if(!pConexion->IsOpen()) {
			if( !AbrirConexionBBDDSeguridad(*pConexion,pathFicheroDB) ){
				////////////////////////////////////////////error fatal no se puede abrir la bbdd
				LOG_ERROR("Core/BBDD", "Error fatal, no se puede escribir la base de datos de seguridad")
				return NULL;
			}
		}
	}catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al crear la base de datos de mensajería: "<<ex.GetMessage())
		return NULL;
	}

	return GNC::GCS::Ptr<wxSQLite3Database>(pConexion);
}

