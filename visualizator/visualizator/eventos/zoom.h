/*
 *  
 *  $Id: zoom.h $
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

namespace AtencionPrimaria {
	namespace Events {
		class EventoZoom: public GNC::GCS::Events::IEvent
		{
		public:

			EventoZoom(GNC::GCS::IVista* pVista = NULL, const double zoom = 0.0f) : GNC::GCS::Events::IEvent(ginkgoEVT_GNKVisualizator_EventoZoom, ginkgoAmbitoAtencionPrimaria, 100, pVista)
			{
				m_zoom = zoom;
				m_Nombre = "Zoom";
			}

			~EventoZoom(){}

			double GetZoom(){return m_zoom;}

		protected:
			double m_zoom;
		};
	}
}
