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

#include <string>
#include <map>
#include <api/api.h>
#include <api/ilock.h>
#include <atomic>

namespace GNC {
	namespace GCS {
		namespace Threading {
			void EXTAPI SetThreadName(long threadID, const std::string& threadName);
		}
	}
}

namespace GNC {
	namespace GCS {

		class ThreadAdaptorPrivate;

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		class EXTAPI Thread {
		
		public:

			Thread(const std::string& name = "thread");

			virtual ~Thread() {}

			const std::string& GetName() const;

			virtual void Stop() = 0;

		protected:

			virtual void* Task() = 0;
		
			std::string m_ThreadName;

			friend class ThreadAdaptorPrivate;
						
		};

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		class EXTAPI ThreadController {
		public:

			typedef std::map<unsigned long, ThreadAdaptorPrivate*> ThreadMap;

			static unsigned long Launch(Thread* thread, bool detached = true);

			static void Stop(unsigned long tid);
			static void Wait(unsigned long tid);
			static void Suspend(unsigned long tid);

			static void OnThreadExit(unsigned long tid);

		protected:
			static GNC::GCS::ILockable Lock;
			static std::atomic<unsigned long> TidCount;
		private:
			static ThreadMap RegisteredThreads;
		};
	}
}
