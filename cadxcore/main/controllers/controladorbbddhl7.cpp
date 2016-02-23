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

#include <sqlite/hl7sqlite.h>

#include "controladorbbddhl7.h"
#include "controladorextensiones.h"
#include <api/globals.h>
#include <api/imodelohl7.h>
#define LOGGER "ControladorBBDDHl7"
#include "controladorlog.h"
#include "dcmtk/dicomservers.h"
#include "../entorno.h"

#include <map>

#include <ctime>

#include <wx/xml/xml.h>
#include <wx/file.h>
#include <wx/sstream.h>
#ifndef _WIN32
#include <signal.h>
#endif
#include <wx/tokenzr.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/filename.h>
#include <wx/thread.h>

GIL::HL7::ControladorBBDDHl7* GIL::HL7::ControladorBBDDHl7::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new GIL::HL7::ControladorBBDDHl7();
	}
	return m_pInstance;
}

void GIL::HL7::ControladorBBDDHl7::FreeInstance()
{
	LOG_DEBUG("Core/BBDD", "Liberando instancia de ControladorBBDDHl7")
	if(m_pInstance!=NULL){
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

GIL::HL7::ControladorBBDDHl7* GIL::HL7::ControladorBBDDHl7::m_pInstance = NULL;


GIL::HL7::ControladorBBDDHl7::ControladorBBDDHl7()
{
	m_pConexion = new wxSQLite3Database();
	try{
		//pillamos la bbdd
		std::string pathFicheroDB;
		std::ostringstream ostr;
		ostr << GNC::Entorno::Instance()->GetGinkgoUserDir() << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgodatabasehl7.db";
		pathFicheroDB = ostr.str();
		wxString wxDirectorio = FROMPATH(GNC::Entorno::Instance()->GetGinkgoUserDir());
		
		if(!wxDirExists(wxDirectorio)){
			bool correcto = true;

			#ifdef _WIN32
			wxMkdir(wxDirectorio,511);
			#else
			correcto = wxMkDir(wxDirectorio.ToUTF8(), 0770) == -1? false : true;
			#endif
			if(!correcto){
			    //std::cout << "Error al crear el directorio " << wxDirectorio.ToUTF8() << std::endl;
				//se escribe en tmp...
				LOG_WARN("Core/BBDD", "No tiene permisos para escribir en el directorio del usuario, se corre el riesgo de no enviar todos los mensajes Hl7")
				std::ostringstream ostr2;
				ostr2 << GNC::Entorno::Instance()->GetGinkgoTempDir() << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgodatabasehl7.db";
				pathFicheroDB = ostr2.str();
			}
		}

		//existe??
		if(!wxFileExists(FROMPATH(pathFicheroDB))) {
			//se crea el fichero de base de datos
			CreateMensajesHl7DB(*m_pConexion,pathFicheroDB.c_str());
		}

		if(!m_pConexion->IsOpen()) {
			if( !AbrirConexionBBDDMensajes(*m_pConexion,pathFicheroDB) ){
				////////////////////////////////////////////error fatal no se puede abrir la bbdd
				LOG_ERROR("Core/BBDD", "Error fatal, no se puede escribir la base de datos de envío de mensajes")
			}
		}
	}catch (wxSQLite3Exception& ex) {
		LOG_WARN("Core/BBDD", "Error al crear la base de datos de mensajería: "<<ex.GetMessage())
	}
}

GIL::HL7::ControladorBBDDHl7::~ControladorBBDDHl7() {
	if(m_pConexion != NULL) {
		m_pConexion->Close();
		delete m_pConexion;
		m_pConexion =NULL;
	}
	LOG_DEBUG("Core/BBDD", "ControladorBBDDHl7 destruído")
}

bool GIL::HL7::ControladorBBDDHl7::AbrirConexionBBDD() {
	//lo dejamos en seccion critica para que no se inicialice mas de una vez
	wxCriticalSectionLocker locker(m_criticalSection);
	if(m_pConexion != NULL && m_pConexion->IsOpen()) {
		return true;
	}

	try {
		//pillamos la bbdd
		//existe??
		std::string pathFicheroDB = m_pathBBDD;
		if(!wxFileExists(FROMPATH(pathFicheroDB))) {
			pathFicheroDB = m_pathBBDDTemporal;
			if(!wxFileExists(FROMPATH(pathFicheroDB))) {
				return false;
			} else {
				LOG_WARN("Core/BBDD", "Buscando en la BBDD temporal " << pathFicheroDB)
			}
		}

		m_pConexion = new wxSQLite3Database();
		if(!m_pConexion->IsOpen()) {
			if( !AbrirConexionBBDDMensajes(*m_pConexion,pathFicheroDB) ){
				LOG_ERROR("Core/BBDD", "Error al abrir la BBDD de mensajes HL7 "<<pathFicheroDB)
				delete m_pConexion;
				m_pConexion = NULL;
				return false;
			} else {
				return true;
			}
		} else {
			return true;
		}
	}catch (wxSQLite3Exception& ex) {
		if(m_pConexion != NULL) {
			delete m_pConexion;
			m_pConexion = NULL;
		}
		LOG_ERROR("Core/BBDD", "Error al abrir la BBDD de mensajes HL7 " << ex.GetMessage().ToUTF8())
		throw GIL::HL7::HL7Exception(std::string(ex.GetMessage().ToUTF8()),"BBDD HL7");
	}
}



void GIL::HL7::ControladorBBDDHl7::GetMensajesHL7(std::list<MensajeHL7>& listaMensajes, bool soloNoEnviados)
{
	if(m_pConexion == NULL) {
		if(!AbrirConexionBBDD()){
			return;
		}
	}

	try {
		wxSQLite3StatementBuffer bufSQL;
		if(soloNoEnviados){
			bufSQL.Format("SELECT IDMensaje, Mensaje, URLEnvio, FechaEnvio,Enviado, MensajeError, Protocolo, ProcesarACK, MsgControlId "
				"FROM MensajesHL7 WHERE "
				"Enviado <> 'S' order by FechaEnvio DESC");
		} else {
			bufSQL.Format("SELECT IDMensaje, Mensaje, URLEnvio, FechaEnvio,Enviado, MensajeError, Protocolo, ProcesarACK, MsgControlId "
				"FROM MensajesHL7 order by FechaEnvio DESC");
		}
		wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
		while (resultados.NextRow()) {
			const int idMensaje = resultados.GetInt(0);
			const std::string mensaje(resultados.GetAsString(1).ToUTF8());
			const std::string urlEnvio(resultados.GetAsString(2).ToUTF8());
			const wxDateTime fechaEnvio = resultados.GetDateTime(3);
			std::string strFechaEnvio;
			if(fechaEnvio.IsValid()) {
				strFechaEnvio = fechaEnvio.Format(wxT("%d/%m/%Y %H:%M:%S")).ToUTF8();
			}
			std::string strEstado(resultados.GetAsString(4).ToUTF8());
			char estado = 'E';
			if(strEstado.size() == 1)
				estado = strEstado.c_str()[0];
			const std::string mensajeError(resultados.GetAsString(5).ToUTF8());
			int protocolo = resultados.GetInt(6);
			char charProcesarACK = (char)resultados.GetAsString(7)[0];
			bool procesarACK = charProcesarACK=='S';
			const std::string msgControlID(resultados.GetAsString(8).ToUTF8());

			MensajeHL7 mensajeHl7(idMensaje,mensaje,strFechaEnvio,urlEnvio,estado,mensajeError,protocolo,procesarACK,msgControlID);
			listaMensajes.push_back(mensajeHl7);
		}
	}catch (wxSQLite3Exception& ex) {
		LOG_ERROR("Core/BBDD", "Error al consultar la BBDD:" << ex.GetMessage().ToUTF8())
		throw GIL::HL7::HL7Exception(std::string(ex.GetMessage().ToUTF8()),"BBDD HL7");
	}
}

void GIL::HL7::ControladorBBDDHl7::PurgarMensajesEnviados()
{
	if(m_pConexion == NULL) {
		if(!AbrirConexionBBDD()){
			return;
		}
	}

	try {
		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("Delete FROM MensajesHL7 WHERE Enviado = 'S'");
		m_pConexion->ExecuteUpdate(bufSQL);
	}catch (wxSQLite3Exception& ex) {
		LOG_ERROR("Core/BBDD", "Error al consultar la BBDD:" << ex.GetMessage().ToUTF8())
		throw GIL::HL7::HL7Exception(std::string(ex.GetMessage().ToUTF8()),"BBDD HL7");
	}
}

void GIL::HL7::ControladorBBDDHl7::ActualizarEstadoMensaje(const int idMensaje, const GIL::HL7::MensajeHL7::TEstado estado, const std::string& mensajeError)
{
	if(m_pConexion == NULL) {
		if(!AbrirConexionBBDD()){
			return;
		}
	}

	try {
		char enviado;
		switch(estado) {
			case GIL::HL7::MensajeHL7::TE_Enviado:
				enviado = 'S';
				break;
			case GIL::HL7::MensajeHL7::TE_Error:
				enviado = 'E';
				break;
			case GIL::HL7::MensajeHL7::TE_Warning:
				enviado = 'W';
				break;
			case GIL::HL7::MensajeHL7::TE_Pendiente:
				enviado = 'P';
				break;
			default:
				enviado = 'P';
				break;
		}
		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("UPDATE MensajesHL7 set MensajeError='%q',Enviado = '%c'  "
			"WHERE IDMensaje = '%d'",mensajeError.c_str(),enviado, idMensaje);
		m_pConexion->ExecuteUpdate(bufSQL);
	}catch (wxSQLite3Exception& ex) {
		LOG_ERROR("Core/BBDD", "Error al consultar la BBDD:" << ex.GetMessage().ToUTF8())
		throw GIL::HL7::HL7Exception(std::string(ex.GetMessage().ToUTF8()),"BBDD HL7");
	}
}

void GIL::HL7::ControladorBBDDHl7::InsertarMensaje(MensajeHL7 mensaje)
{
	try{
		char charProcesar = 'S';
		if(!mensaje.m_procesarACK){
			charProcesar = 'N';
		}

		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("INSERT INTO MensajesHL7 (IDMensaje,FechaEnvio,Mensaje,URLEnvio,Protocolo,ProcesarACK,MsgControlId) VALUES (NULL,datetime('now','localtime'),'%q','%q','%d','%c','%q')"
			,mensaje.m_mensaje.c_str(), mensaje.m_destino.c_str(), mensaje.m_protocolo, charProcesar, mensaje.m_msgControlId.c_str());
		if(m_pConexion->ExecuteUpdate(bufSQL)>0){

		} else {
			throw HL7Exception("Error al insertar el mensaje en la cola, consulte a su proveedor", "NET");
		}
	}catch (wxSQLite3Exception& ex) {
		LOG_ERROR("Core/BBDD", "Error al insertar en  la BBDD:" << ex.GetMessage().ToUTF8())
		throw GIL::HL7::HL7Exception(std::string(ex.GetMessage().ToUTF8()),"BBDD HL7");
	}
}

void GIL::HL7::ControladorBBDDHl7::ActualizarMensaje(const int idMensaje, const std::string& destino, const std::string& mensaje, const MensajeHL7::TEstado estado)
{
	try{
		char enviado;
		switch(estado) {
			case GIL::HL7::MensajeHL7::TE_Enviado:
				enviado = 'S';
				break;
			case GIL::HL7::MensajeHL7::TE_Error:
				enviado = 'E';
				break;
			case GIL::HL7::MensajeHL7::TE_Warning:
				enviado = 'W';
				break;
			case GIL::HL7::MensajeHL7::TE_Pendiente:
				enviado = 'P';
				break;
			default:
				enviado = 'P';
				break;
		}

		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("UPDATE MensajesHL7 SET URLEnvio='%q', Mensaje ='%q', Enviado = '%c' where IDMensaje='%d'",destino.c_str(), mensaje.c_str(), enviado, idMensaje);
		if(m_pConexion->ExecuteUpdate(bufSQL)>0){

		} else {
			throw HL7Exception("Error al insertar el mensaje en la cola, consulte a su proveedor", "NET");
		}
	}catch (wxSQLite3Exception& ex) {
		LOG_ERROR("Core/BBDD", "Error al insertar en  la BBDD:" << ex.GetMessage().ToUTF8())
		throw GIL::HL7::HL7Exception(std::string(ex.GetMessage().ToUTF8()),"BBDD HL7");
	}
}

void GIL::HL7::ControladorBBDDHl7::EliminarMensaje(const int idMensaje)
{
	if(m_pConexion == NULL) {
		if(!AbrirConexionBBDD()){
			return;
		}
	}

	try {
		wxSQLite3StatementBuffer bufSQL;
		bufSQL.Format("Delete FROM MensajesHL7 WHERE IDMensaje = '%d'", idMensaje);
		m_pConexion->ExecuteUpdate(bufSQL);
	}catch (wxSQLite3Exception& ex) {
		LOG_ERROR("Core/BBDD", "Error al Eliminar en la BBDD:" << ex.GetMessage().ToUTF8())
		throw GIL::HL7::HL7Exception(std::string(ex.GetMessage().ToUTF8()),"BBDD HL7");
	}
}

