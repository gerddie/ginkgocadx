/*
 *  
 *  $Id: sincronizacion.h $
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
#include <api/ievento.h>
#include <api/controllers/ieventscontroller.h>

namespace AtencionPrimaria {
	namespace Events {
		class EventoSincronizacion: public GNC::GCS::Events::IEvent
		{
		public:
			typedef enum {
				Scroll,
				Dessincronizar
			} TipoSincronizacion;

			EventoSincronizacion(GNC::GCS::IVista* pVista, TipoSincronizacion  tipo = Scroll, int posicion = 0, bool relativa = true) : GNC::GCS::Events::IEvent(ginkgoEVT_GNKVisualizator_EventoSincronizacion, ginkgoAmbitoAtencionPrimaria, 100, pVista)
			{
				m_posicion = posicion;
				m_relativa = relativa;
				m_tipo = tipo;
				m_Nombre = "Sincronizacion";
			}

			~EventoSincronizacion(){}

			int GetPosicion(){return m_posicion;}

			bool EsRelativa(){return m_relativa;}

			TipoSincronizacion GetTipoEvento()
			{
				return m_tipo;
			}

			virtual void pushInfo(std::ostream& out) const {
				out << "Tipo = ";
				switch (m_tipo) {
					case Scroll:
						out << "Scroll";
						break;
					case Dessincronizar:
						out << "Dessincronizar";
						break;
				}
				out << ", Posicion = " << m_posicion << ", Relativa = " << m_relativa;
			}

		protected:
			TipoSincronizacion m_tipo;
			int m_posicion;
			bool m_relativa;
		};
	}
}
