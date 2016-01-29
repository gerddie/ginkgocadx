/*
 *  
 *  $Id: overlays.h $
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
#include <api/controllers/ieventscontroller.h>
#include "../estudios/overlay.h"

namespace GNKVisualizator {
	namespace Events {
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

			~EventoRefrescarListaOverlays(){
				m_pListaOverlays = NULL;
			}

			TListaOverlays* GetListaOverlays(){return m_pListaOverlays;}

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

			~EventoMostrarOverlay(){}

			const TOverlay& GetOverlay(){return m_overlay;}

		protected:
			TOverlay m_overlay;
		};

	}
}
