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

namespace AtencionPrimaria
{
namespace Events
{
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
        ~ChangePositionEvent() {}

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
