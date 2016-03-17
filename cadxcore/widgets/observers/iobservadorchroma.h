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
#include <stddef.h>
#include <api/ievento.h>


namespace GNC
{
namespace GCS
{
namespace Widgets
{
class WMarcadoArea;

namespace Observadores
{

/* Observador especifico de la herramienta lazo */
class IObservadorChroma
{
public:
        virtual ~IObservadorChroma() {};

        virtual void OnKeyEvent(GNC::GCS::Events::EventoTeclado & evt) = 0;
};
}

namespace Notificadores
{
class INotificadorChroma
{

protected:
        GNC::GCS::Widgets::Observadores::IObservadorChroma* m_pObservadorChroma;

public:
        INotificadorChroma()
        {
                m_pObservadorChroma = NULL;
        }

        INotificadorChroma(GNC::GCS::Widgets::Observadores::IObservadorChroma* pObservador)
        {
                m_pObservadorChroma = pObservador;
        }

        virtual ~INotificadorChroma() {};

public:
        void SetObservador(GNC::GCS::Widgets::Observadores::IObservadorChroma* pObservador)
        {
                m_pObservadorChroma = pObservador;
        }

        virtual void NotificarKeyEvent(GNC::GCS::Events::EventoTeclado& evt)
        {
                if (m_pObservadorChroma != NULL) {
                        m_pObservadorChroma->OnKeyEvent(evt);
                }
        }
};
}
}
}
}
