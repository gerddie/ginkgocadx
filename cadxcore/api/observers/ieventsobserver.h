#pragma once
/*
 *  
 *  $Id: ieventsobserver.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef IEVENTSOBSERVER_H
#define IEVENTSOBSERVER_H

#include <list>

#include <api/api.h>

namespace GNC {
	namespace GCS {
		namespace Events {
			class IEvent;
		}
		class IEventsController;
	}
}

namespace GNC {
	namespace GCS {

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
