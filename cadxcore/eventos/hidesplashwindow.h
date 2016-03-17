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

#include <api/ievento.h>
#include "eventosginkgo.h"

namespace GNC
{
namespace GCS
{
namespace Events
{

class EXTAPI EventHideSplashWindow : public GNC::GCS::Events::IEvent
{
public:

        EventHideSplashWindow(GNC::GCS::IVista* pVista = NULL) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_HideSplashWindow, 0, 100, pVista)
        {
                m_Nombre = "EventHideSplashWindow";
        }

        ~EventHideSplashWindow()
        {
        }

        virtual void pushInfo(std::ostream& /*out*/) const
        {
        }

protected:
};

}
}
}
