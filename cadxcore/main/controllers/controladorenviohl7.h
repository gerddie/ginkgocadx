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
