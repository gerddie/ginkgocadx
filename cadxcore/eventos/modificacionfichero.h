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

#include <string>
#include <vector>

#include <api/ievento.h>
#include "eventosginkgo.h"

namespace GNC
{
namespace GCS
{
class IStudyContext;
namespace Events
{
class EXTAPI EventoModificacionFichero: public GNC::GCS::Events::IEvent
{
public:

        typedef std::list<std::string> ListaRutas;

        typedef enum {
                FicheroGuardado,
                FicheroModificado,
                FicheroCerrado
        } TipoModificacion;

        EventoModificacionFichero(GNC::GCS::IVista* pVistaModificada = NULL,  TipoModificacion  tipo = FicheroModificado, GNC::GCS::IVista* pVistaAmbito = NULL):GNC::GCS::Events::IEvent(ginkgoEVT_Core_ModificacionFichero,0,100,pVistaAmbito)
        {
                m_pVistaModificada = pVistaModificada;
                m_tipo = tipo;
                m_pContexto = NULL;
                m_Nombre = "ModificacionFichero";
        }

        EventoModificacionFichero(GNC::GCS::IVista* pVistaModificada, IStudyContext* pContexto = NULL, TipoModificacion  tipo = FicheroModificado, GNC::GCS::IVista* pVistaAmbito = NULL):GNC::GCS::Events::IEvent(ginkgoEVT_Core_ModificacionFichero,0,100,pVistaAmbito)
        {
                m_pVistaModificada = pVistaModificada;
                m_tipo = tipo;
                m_pContexto = pContexto;
                m_Nombre = "ModificacionFichero";
        }

        ~EventoModificacionFichero()
        {
                m_pVistaModificada=NULL;
                m_pContexto = NULL;
        }

        TipoModificacion GetTipoEvento()
        {
                return m_tipo;
        }


        IStudyContext* GetContextoEstudio()
        {
                return m_pContexto;
        }

        GNC::GCS::IVista* GetVistaModificada()
        {
                return m_pVistaModificada;
        }

        virtual void pushInfo(std::ostream& out) const
        {
                out << "Tipo = ";
                switch (m_tipo) {
                case FicheroGuardado:
                        out << "FicheroGuardado";
                        break;
                case FicheroModificado:
                        out << "FicheroModificado";
                        break;
                case FicheroCerrado:
                        out << "FicheroCerrado";
                        break;
                }
        }

protected:
        GNC::GCS::IVista*  m_pVistaModificada;
        TipoModificacion   m_tipo;
        IStudyContext*  m_pContexto;
};
}
}
}
