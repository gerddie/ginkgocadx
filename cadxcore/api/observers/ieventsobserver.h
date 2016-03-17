#pragma once
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

#ifndef IEVENTSOBSERVER_H
#define IEVENTSOBSERVER_H

#include <list>

#include <api/api.h>

namespace GNC
{
namespace GCS
{
namespace Events
{
class IEvent;
}
class IEventsController;
}
}

namespace GNC
{
namespace GCS
{

//====================================================================================================
//= Interfaz del observador de eventos
//====================================================================================================
class EXTAPI IEventsObserver
{
public:
        IEventsObserver();

        virtual ~IEventsObserver();

        //virtual void OnRegistrar (IControladorEventos* pControlador);

        virtual void PreProcesarEvento(GNC::GCS::Events::IEvent*, std::list<GNC::GCS::Events::IEvent*>& );

        virtual bool FiltrarEvento(GNC::GCS::Events::IEvent*);

        virtual void ProcesarEvento(GNC::GCS::Events::IEvent* evt) = 0;

protected:
        //IControladorEventos* m_pControlador;
};
}
}
#endif
