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

#include <api/iwaitqueue.h>
#include <api/controllers/icontroladorlog.h>
#include <wx/thread.h>


//----------------------------------------------------------------------------------------------------
//region Cola de espera
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//region Constructor y destructor

GNC::GCS::WaitQueue::WaitQueue()
{
        m_pSignaler = new wxSemaphore(0, 0);
        m_Size = 0;
}

GNC::GCS::WaitQueue::~WaitQueue()
{
        GNC::GCS::ILocker q_Locker(this);
        if (m_Size > 0 ) {
                LOG_ERROR("WaitQueue", "Inconsistency detected: The queue finished while some pending task non signaled.");
        }
        delete m_pSignaler;
        m_pSignaler = NULL;
}
//endregion

//----------------------------------------------------------------------------------------------------
//region Interfaz general

GNC::GCS::TipoListaEspera::size_type GNC::GCS::WaitQueue::Size() const
{
        return m_Size;
}

bool GNC::GCS::WaitQueue::Empty() const
{
        return (m_Size == 0);
}

bool GNC::GCS::WaitQueue::NotEmpty() const
{
        return m_Size != 0;
}

GNC::GCS::TipoListaEspera::size_type GNC::GCS::WaitQueue::Pending()
{
        ILocker q_lock(this);
        const TipoListaEspera::size_type npending = m_Size - m_ListaEspera.size();
        return npending;
}


bool GNC::GCS::WaitQueue::Wait()
{
        wxSemaError err = m_pSignaler->Wait();
        GNC::GCS::ILocker q_Locker(this);
        if (err != wxSEMA_NO_ERROR) {
                return false;
        } else {
                m_Size--;
        }
        return true;
}

bool GNC::GCS::WaitQueue::Wait(long msecs)
{
        wxSemaError err = m_pSignaler->WaitTimeout(msecs);
        GNC::GCS::ILocker q_Locker(this);
        if (err != wxSEMA_NO_ERROR) {
                return false;
        } else {
                m_Size--;
        }
        return true;
}

void GNC::GCS::WaitQueue::RegistrarEspera(WaitQueueTask* task)
{
        GNC::GCS::ILocker q_Locker(this);
        GNC::GCS::ILocker t_Locker(task);
        task->m_ListaNotificacion.push_back(this);
        m_ListaEspera.push_back(task);
        m_Size++;
}

void GNC::GCS::WaitQueue::TerminarPendientes()
{
        GNC::GCS::ILocker q_locker(this);

        for (TipoListaEspera::iterator it = m_ListaEspera.begin(); it != m_ListaEspera.end(); ++it) {
                GNC::GCS::WaitQueueTask* wqt = *it;
                GNC::GCS::ILocker t_locker(wqt);
                if (wqt->m_NotificarTerminacion) {
                        wqt->Terminar();
                }
        }
}
//endregion

//----------------------------------------------------------------------------------------------------
//region Interfaz privada

//endregion

//endregion

//----------------------------------------------------------------------------------------------------
// Tarea de la cola de espera
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//region Constructor y destructor

GNC::GCS::WaitQueueTask::WaitQueueTask()
{
        m_NotificarTerminacion = false;
}

GNC::GCS::WaitQueueTask::~WaitQueueTask()
{
}
//endregion

//----------------------------------------------------------------------------------------------------
//region Interfaz general

void GNC::GCS::WaitQueueTask::Signal()
{
        //GNC::GCS::ILocker t_locker(this, loc);
        for (TipoListaNotificaciones::iterator it_q = m_ListaNotificacion.begin(); it_q != m_ListaNotificacion.end(); ++it_q) {
                WaitQueue* wq = (*it_q);
                ILocker q_locker(wq);

                bool notfound = true;
                for (TipoListaEspera::iterator it_t = wq->m_ListaEspera.begin(); it_t != wq->m_ListaEspera.end(); ++it_t) {
                        WaitQueueTask* qt = *it_t;
                        if ( qt == this) {
                                wq->m_ListaEspera.remove(this);
                                wq->m_pSignaler->Post();
                                notfound = false;
                                break;
                        }
                }

                if (notfound) {
                        LOG_ERROR("WaitQueue", "Error: Inconsistency in waitqueue mechanism. It wast notified a non registered or alreadt notified dependency");
                }

        }
        m_ListaNotificacion.clear();
}

void GNC::GCS::WaitQueueTask::Terminar()
{
        m_NotificarTerminacion = true;
        DoTerminar();
}
//endregion

//region Interfaz privada

//endregion

//endregion


GNC::GCS::NotifyQueue::NotifyQueue()
{
        RegistryCount = 0;
        NotifyObject = new wxSemaphore(0, 0);
}

GNC::GCS::NotifyQueue::~NotifyQueue()
{
}

void GNC::GCS::NotifyQueue::NotifyAll()
{

        GNC::GCS::ILocker lock(RegistryLock);

        for (unsigned int i = 0; i < RegistryCount; i++) {
                NotifyObject->Post();
        }

}

void GNC::GCS::NotifyQueue::Wait()
{
        {
                GNC::GCS::ILocker lock(RegistryLock);
                RegistryCount++;
        }
        /*wxSemaError err =*/ NotifyObject->Wait();
}

unsigned int GNC::GCS::NotifyQueue::Size() const
{
        return RegistryCount;
}

bool GNC::GCS::NotifyQueue::Requested() const
{
        return RegistryCount > 0;
}

