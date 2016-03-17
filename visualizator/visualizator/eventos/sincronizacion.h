/*
 *  Ginkgo CADx Project
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
#include <api/controllers/ieventscontroller.h>

namespace AtencionPrimaria
{
namespace Events
{
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

        ~EventoSincronizacion() {}

        int GetPosicion()
        {
                return m_posicion;
        }

        bool EsRelativa()
        {
                return m_relativa;
        }

        TipoSincronizacion GetTipoEvento()
        {
                return m_tipo;
        }

        virtual void pushInfo(std::ostream& out) const
        {
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
