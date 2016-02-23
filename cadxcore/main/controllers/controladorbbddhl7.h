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
#include <api/api.h>
#include <wx/thread.h>
#include <list>

class wxSQLite3Database;
class wxCriticalSection;

namespace GIL {
	namespace HL7 {
		struct MensajeHL7 
		{
			typedef enum {
				TE_Error,
				TE_Warning,
				TE_Enviado,
				TE_Pendiente				
			} TEstado;
			int m_id;
			std::string m_mensaje;
			std::string m_fecha;
			std::string m_mensajeError;
			std::string m_destino;
			std::string m_msgControlId;
			int m_protocolo;
			bool m_procesarACK;
			TEstado m_estado;
			MensajeHL7(int id, const std::string& mensaje, const std::string& fecha, const std::string& destino,const char estado, const std::string& mensajeError, int protocolo, bool procesarACK, const std::string& msgControlId )
				: m_id(id), m_mensaje(mensaje), m_fecha(fecha), m_mensajeError(mensajeError), m_destino(destino), m_msgControlId(msgControlId), m_protocolo(protocolo), m_procesarACK(procesarACK)
			{
				switch(estado) {
					case 'S':
						m_estado = TE_Enviado;
						break;
					case 'N':
						m_estado = TE_Pendiente;
						break;
					case 'W':
						m_estado = TE_Warning;
						break;
					case 'E':
						m_estado = TE_Error;
						break;
				}				
			}

			~MensajeHL7()
			{
			}
		};
		typedef std::list<MensajeHL7> TListaMensajes;

		class EXTAPI ControladorBBDDHl7
		{
		public:
			static ControladorBBDDHl7* Instance();
			static void FreeInstance();

			void GetMensajesHL7(std::list<MensajeHL7>& listaMensajes, bool soloNoEnviados = false);
			void PurgarMensajesEnviados();
			void ActualizarEstadoMensaje(const int idMensaje, const MensajeHL7::TEstado estado, const std::string& mensajeError);
			void InsertarMensaje(MensajeHL7 mensaje);
			void ActualizarMensaje(const int idMensaje, const std::string& destino, const std::string& mensaje, const MensajeHL7::TEstado estado);
			void EliminarMensaje(const int idMensaje);
			

		protected:
			static ControladorBBDDHl7* m_pInstance;
			ControladorBBDDHl7();
			~ControladorBBDDHl7();

			bool AbrirConexionBBDD();

			wxSQLite3Database* m_pConexion;
			std::string        m_pathBBDD;
			std::string        m_pathBBDDTemporal;
			wxCriticalSection m_criticalSection;
		};
	}
}
