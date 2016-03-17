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
#include <api/controllers/ieventscontroller.h>
#include "../estudios/overlay.h"

namespace GNKVisualizator
{
namespace Events
{
class EventoRefrescarListaOverlays: public GNC::GCS::Events::IEvent
{
public:
        EventoRefrescarListaOverlays(GNC::GCS::IVista* pVista) : GNC::GCS::Events::IEvent(ginkgoEVT_GNKVisualizator_EventoRefrescarListaOverlays, ginkgoAmbitoAtencionPrimaria, 100, pVista)
        {
                m_pListaOverlays=NULL;
                m_Nombre = "RefrescarListaOverlays";
        }

        EventoRefrescarListaOverlays(GNC::GCS::IVista* pVista,TListaOverlays* pListaOverlays) : GNC::GCS::Events::IEvent(ginkgoEVT_GNKVisualizator_EventoRefrescarListaOverlays, ginkgoAmbitoAtencionPrimaria, 100, pVista)
        {
                m_pListaOverlays=pListaOverlays;
                m_Nombre = "RefrescarListaOverlays";
        }

        ~EventoRefrescarListaOverlays()
        {
                m_pListaOverlays = NULL;
        }

        TListaOverlays* GetListaOverlays()
        {
                return m_pListaOverlays;
        }

protected:
        TListaOverlays* m_pListaOverlays;
};

class EventoMostrarOverlay: public GNC::GCS::Events::IEvent
{
public:

        EventoMostrarOverlay(GNC::GCS::IVista* pVista) : GNC::GCS::Events::IEvent(ginkgoEVT_GNKVisualizator_EventoMostrarOverlay, ginkgoAmbitoAtencionPrimaria, 100, pVista)
        {
                m_Nombre = "MostrarOverlay";
        }

        EventoMostrarOverlay(GNC::GCS::IVista* pVista,TOverlay& overlay) : GNC::GCS::Events::IEvent(ginkgoEVT_GNKVisualizator_EventoMostrarOverlay, ginkgoAmbitoAtencionPrimaria, 100, pVista)
        {
                m_overlay = overlay;
                m_Nombre = "MostrarOverlay";
        }

        ~EventoMostrarOverlay() {}

        const TOverlay& GetOverlay()
        {
                return m_overlay;
        }

protected:
        TOverlay m_overlay;
};

}
}
