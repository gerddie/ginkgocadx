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

#include <api/ievento.h>
#include "eventosginkgo.h"

namespace GNC {
	namespace GCS {
		namespace Events {

			//este evento servirá para lanzar mensajes al usuario, en el status bar y en un futuro en el popup window q sea
			class EXTAPI EventoMensajes : public GNC::GCS::Events::IEvent
			{
			public:
				typedef enum {
					StatusMessage,
					PopUpMessage
				} TipoEventoMensajes;

				typedef enum {
					Informacion,
					Aviso,
					Error
				} TipoGravedad;

				EventoMensajes(GNC::GCS::IVista* pVista, TipoEventoMensajes tipo = StatusMessage, TipoGravedad gravedad = Informacion): GNC::GCS::Events::IEvent(ginkgoEVT_Core_MensajesUsuario,0,100,pVista){
					m_Nombre = "Mensajes";	
					m_tipo = tipo;
					m_gravedad = gravedad;
					m_sincrono = true;
				}

				EventoMensajes(GNC::GCS::IVista* pVista,const std::string& mensaje, TipoEventoMensajes tipo = StatusMessage, bool sincrono = true, TipoGravedad gravedad = Informacion) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_MensajesUsuario,0,100,pVista)
				{
					m_Nombre = "Mensajes";
					m_Mensaje = mensaje;
					m_tipo = tipo;
					m_gravedad = gravedad;
					m_sincrono = sincrono;
				}

				~EventoMensajes()
				{
				}

				std::string& GetMensaje(){
					return m_Mensaje;
				}

				TipoEventoMensajes GetTipo(){
					return m_tipo;
				}

				void SetGravedad(TipoGravedad gravedad) {
					m_gravedad = gravedad;
				}

				TipoGravedad GetGravedad() {
					return m_gravedad;
				}

				bool IsSincronoConLaInterfaz() {
					return m_sincrono;
				}

				virtual void pushInfo(std::ostream& out) const {
					out << "Mensaje: " << m_Mensaje;
				}

			protected:
				std::string        m_Mensaje;
				TipoEventoMensajes m_tipo;
				TipoGravedad       m_gravedad;
				bool               m_sincrono;
			};
		}
	}
}
