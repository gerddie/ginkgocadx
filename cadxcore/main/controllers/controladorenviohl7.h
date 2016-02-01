/*
 *  
 *  $Id: controladorenviohl7.h $
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
#include <api/api.h>
#include <api/observers/ieventsobserver.h>
#include <wx/thread.h>
#include <atomic>

class wxSQLite3Database;
class wxCriticalSection;

namespace GIL {
	namespace HL7 {
		class EXTAPI ControladorEnvioHl7:  protected wxThread, public GNC::GCS::IEventsObserver
		{
		public:
			static void FreeInstance();
			static void Arrancar();
			static bool EstaArrancado();
			void Abortar();
			bool EstaAbortado();

		protected:
			static ControladorEnvioHl7* m_pInstance;
			static wxCriticalSection m_criticalSection;

			ControladorEnvioHl7();
			~ControladorEnvioHl7();

			virtual void* Entry();

			virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt) ;
			wxSemaphore		m_semaphore;
			std::atomic<bool> m_Abortado;
		};
	}
}
