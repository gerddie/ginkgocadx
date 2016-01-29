/*
 *  
 *  $Id: thread.h $
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

#include <string>
#include <map>
#include <api/api.h>
#include <api/ilock.h>

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
			static unsigned long TidCount;
		private:
			static ThreadMap RegisteredThreads;
		};
	}
}
