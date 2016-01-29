/*
*  
*  $Id: mensajes.h $
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
