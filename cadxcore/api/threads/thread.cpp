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

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/iexception.h>

#include <api/threads/thread.h>
#include <api/controllers/icontroladorlog.h>

#include <wx/thread.h>

GNC::GCS::ILockable GNC::GCS::ThreadController::Lock;

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

#if defined (_WINDOWS)
#define MS_VC_EXCEPTION 0x406D1388

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void GNC::GCS::Threading::SetThreadName( long threadID, const std::string& threadName)
{
   //Sleep(10);
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName.c_str();
   info.dwThreadID = threadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
}
#elif defined(__WXOSX__)
#include <pthread.h>
#include <sys/signal.h>

void GNC::GCS::Threading::SetThreadName( long /*threadID*/, const std::string& threadName)
{
	//pthread_setname_np(threadName.c_str());
}

void FooSignalHandler(int sig)
{
	GTRACE("SIGUSR1 recibido");
}
#else
#include <pthread.h>
#include <sys/signal.h>

void GNC::GCS::Threading::SetThreadName( long /*threadID*/, const std::string& /*threadName*/)
{
}

void FooSignalHandler(int /*sig*/)
{
	GTRACE("SIGUSR1 recibido");
}

#endif


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

namespace GNC {
	namespace GCS {
		class ThreadAdaptorPrivate: public wxThread {
		protected:
			ThreadAdaptorPrivate(GNC::GCS::Thread* iface, wxThreadKind kind ) : wxThread(kind)
			{
				this->m_threadIface = iface;
				this->m_threadId = 0;

			}

			~ThreadAdaptorPrivate()
			{
			}

			virtual void OnExit() {
				;
			}


			virtual void* Entry() {
				GNC::GCS::Threading::SetThreadName( GetId(), this->m_threadIface->GetName());
                                
				try {
					return this->m_threadIface->Task();
				}
				catch (GNC::GCS::IException& ex) {
					LOG_ERROR(_Std("Threading"), _Std("Error while running thread: ") << this->m_threadIface->GetName() << _Std(" : ") << ex.what() );
				}
				catch (std::exception& ex) {
					LOG_ERROR(_Std("Threading"), _Std("Error while running thread: ") << this->m_threadIface->GetName() << _Std(" : ") << ex.what() );
				}
				catch (...) {
					LOG_ERROR(_Std("Threading"), _Std("Internal error while running thread: ") << this->m_threadIface->GetName() );
				}
				return NULL;
			}

			virtual void SetTid(unsigned long tid)
			{
				this->m_threadId = tid;
			}

			Thread* m_threadIface;
			unsigned long m_threadId;
			friend class ThreadController;
		};
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

GNC::GCS::Thread::Thread(const std::string& name)
{
	this->m_ThreadName = name;
}

const std::string& GNC::GCS::Thread::GetName() const
{
	return this->m_ThreadName;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
GNC::GCS::ThreadController::ThreadMap GNC::GCS::ThreadController::RegisteredThreads;
std::atomic<unsigned long> GNC::GCS::ThreadController::TidCount(1);

unsigned long GNC::GCS::ThreadController::Launch(Thread* thread, bool detached)
{
	unsigned long tid = 0;
	ThreadAdaptorPrivate* threadPrivate = new ThreadAdaptorPrivate(thread, detached ? wxTHREAD_DETACHED : wxTHREAD_JOINABLE);
	threadPrivate->Create();	
	{
		tid = TidCount++;
		threadPrivate->SetTid(tid);

                GNC::GCS::ILocker lock(Lock);
                RegisteredThreads[tid] = threadPrivate;
	}	
	threadPrivate->Run();

	return tid;

}
void GNC::GCS::ThreadController::Stop(unsigned long tid)
{
	GNC::GCS::ILocker lock(Lock);
	ThreadMap::iterator it = RegisteredThreads.find(tid);
	if (it != RegisteredThreads.end()) {
		ThreadAdaptorPrivate* threadPrivate = (*it).second;
		RegisteredThreads.erase(it);
		threadPrivate->m_threadIface->Stop();
		threadPrivate->Wait();
		delete threadPrivate;
	}
}

void GNC::GCS::ThreadController::Wait(unsigned long tid)
{
	GNC::GCS::ILocker lock(Lock);
	ThreadMap::iterator it = RegisteredThreads.find(tid);
	if (it != RegisteredThreads.end()) {
		ThreadAdaptorPrivate* threadPrivate = (*it).second;
		threadPrivate->Wait();
		RegisteredThreads.erase(it);
	}
}

void GNC::GCS::ThreadController::OnThreadExit(unsigned long tid)
{
	GNC::GCS::ILocker lock(Lock);
	ThreadMap::iterator it = RegisteredThreads.find(tid);
	if (it != RegisteredThreads.end()) {
		ThreadAdaptorPrivate* threadPrivate = (*it).second;		
		delete threadPrivate;
		RegisteredThreads.erase(it);
	}
}

