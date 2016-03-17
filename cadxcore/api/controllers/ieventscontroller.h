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

#include <api/api.h>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations
namespace GNC
{
namespace GCS
{
class IEventsObserver;
namespace Events
{
class IEvent;
}
}
}
//endregion

namespace GNC
{
namespace GCS
{

//====================================================================================================
//= Interfaz del controlador de eventos
//====================================================================================================
class EXTAPI IEventsController
{

protected:
        IEventsController();
        virtual ~IEventsController();

public:

        static IEventsController* Instance();
        static void FreeInstance();

        //se apunta a escuchar un evento
        virtual void Registrar (GNC::GCS::IEventsObserver* pObservador, const GNC::GCS::Events::IEvent& evento) = 0;
        //se desapunta de escuchar un evento
        virtual void DesRegistrar (GNC::GCS::IEventsObserver* pObservador, const GNC::GCS::Events::IEvent& evento) = 0;
        //se desapunta de todos los eventos a los que escucha
        virtual void DesRegistrar (GNC::GCS::IEventsObserver* pObservador) = 0;
        //avisa a los observadores registrados a este evento
        virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt) = 0;
};

}
}
