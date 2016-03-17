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
class IObservadorMarcadoArea
{
public:

        virtual ~IObservadorMarcadoArea() {}

        virtual void OnPoligonoCreado(GNC::GCS::Widgets::WMarcadoArea* pPoligono) = 0;
        virtual void OnPoligonoModificado(GNC::GCS::Widgets::WMarcadoArea* pPoligono) = 0;
        virtual void OnPoligonoDestruido(GNC::GCS::Widgets::WMarcadoArea* pPoligono) = 0;
        virtual void OnKeyEvent(GNC::GCS::Events::EventoTeclado & evt) = 0;
};
}

namespace Notificadores
{
class INotificadorMarcadoArea
{

protected:
        GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* m_pObservadorMarcado;

public:


        INotificadorMarcadoArea()
        {
                m_pObservadorMarcado = NULL;
        }

        INotificadorMarcadoArea(GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador)
        {
                m_pObservadorMarcado = pObservador;
        }

        virtual ~INotificadorMarcadoArea() {}

public:
        void SetObservador(GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador)
        {
                m_pObservadorMarcado = pObservador;
        }

        virtual void NotificarCreacion(WMarcadoArea* pWidget)
        {
                if (m_pObservadorMarcado != NULL) {
                        m_pObservadorMarcado->OnPoligonoCreado(pWidget);
                }
        }

        virtual void NotificarModificacion(WMarcadoArea* pWidget)
        {
                if (m_pObservadorMarcado != NULL) {
                        m_pObservadorMarcado->OnPoligonoModificado(pWidget);
                }
        }

        virtual void NotificarDestruccion(WMarcadoArea* pWidget)
        {
                if (m_pObservadorMarcado != NULL) {
                        m_pObservadorMarcado->OnPoligonoDestruido(pWidget);
                }
        }

        virtual void NotificarKeyEvent(GNC::GCS::Events::EventoTeclado& evt)
        {
                if (m_pObservadorMarcado != NULL) {
                        m_pObservadorMarcado->OnKeyEvent(evt);
                }
        }
};
}
}
}
}
