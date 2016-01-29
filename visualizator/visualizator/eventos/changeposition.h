/*
 *  
 *  $Id: changeposition.h $
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
		class ChangePositionEvent: public GNC::GCS::Events::IEvent
		{
		public:

			ChangePositionEvent(GNC::GCS::IVista* pVista = NULL, const double pos[3] = NULL, const double focal[3] = NULL) : GNC::GCS::Events::IEvent(ginkgoEVT_GNKVisualizator_ChangePositionEvent, ginkgoAmbitoAtencionPrimaria, 100, pVista)
			{
				m_Nombre = "ChangePositionEvent";

				if (pos != NULL) {
					for (int i = 0; i < 3; i++) {
						m_pos[i] = pos[i];
					}
				}

				if (focal != NULL) {
					for (int i = 0; i < 3; i++) {
						m_focal[i] = focal[i];
					}
				}
			}
			~ChangePositionEvent(){}

			void SetPosition(const double pos[3]) 
			{
				for (int i = 0; i < 3; i++) {
					m_pos[i] = pos[i];
				}
			}

			const double* GetPosition() 
			{
				return m_pos;
			}

			void SetFocal(const double focal[3]) 
			{
				for (int i = 0; i < 3; i++) {
					m_focal[i] = focal[i];
				}
			}

			const double* GetFocal() 
			{
				return m_focal;
			}
		protected:
			double m_pos[3], m_focal[3];
		};
	}
}