/*
 *  
 *  $Id: ieventscontroller.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

#include <api/api.h>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations
namespace GNC {
	namespace GCS {
		class IEventsObserver;
		namespace Events {
			class IEvent;
		}
	}
}
//endregion

namespace GNC {
	namespace GCS {

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
