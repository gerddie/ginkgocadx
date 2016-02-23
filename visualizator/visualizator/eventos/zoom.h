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
