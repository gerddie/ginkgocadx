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


#include <deque>
#include <set>
#include <complex>

#include <api/globals.h>
#include <api/threads/thread.h>
#include <api/icommand.h>
#include <api/ientorno.h>

#include <api/controllers/icontroladorlog.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/progresocomando.h>
#include <main/controllers/historycontroller.h>

#include <wx/wx.h>
#include <wx/thread.h>

#include "commandcontroller.h"
#include <main/gui/mainwindow/ventanaprincipal.h>

//#undef GTRACE
//#define GTRACE(expr) std::cout << expr << std::endl

/** POLL Period in ms **/
#define POLL_PERIOD 100
/** Thread pool size **/
#define THREAD_POOL_SIZE 1

namespace GNC
{
class ThreadPool : public GNC::GCS::ILockable
{

public:
        ThreadPool() : m_Shutdown(false)
        {

                for (size_t i = 0; i < THREAD_POOL_SIZE; ++i) {
                        new GNC::CommandLauncher(this);
                }
        }

        /**
        * Borrows a launcher from the pool
        **/
        GNC::CommandLauncher* Borrow()
        {
                while (true) {
                        {
                                GNC::GCS::ILocker lock(this);
                                if (!m_Shutdown) {
                                        if (!available.empty()) {
                                                GNC::CommandLauncher* pCmd = available.front();
                                                available.pop_front();
                                                working.insert(pCmd);
                                                return pCmd;
                                        } else {
                                                new GNC::CommandLauncher(this);
                                        }
                                } else {
                                        return NULL;
                                }

                        }
                        wxThread::Sleep(POLL_PERIOD); // Wait for completion

                }
                return NULL;

        }

        /**
        * Returns a launcher to the pool
        **/
        void Return(GNC::CommandLauncher* pCmd)
        {
                pCmd->SetCommand(NULL);
                GNC::GCS::ILocker lock(this);
                working.erase(pCmd);
                available.push_back(pCmd);
        }


        void Shutdown()
        {
                GNC::GCS::ILocker lock(this);
                m_Shutdown = true;
                for (std::set<GNC::CommandLauncher*>::iterator it = working.begin(); it != working.end(); ++it) {
                        (*it)->m_Shutdown = true;
                }
                working.clear();
                for (std::deque<GNC::CommandLauncher*>::iterator it = available.begin(); it != available.end(); ++it) {
                        (*it)->m_Shutdown = true;
                        (*it)->m_pParker->Post();
                }
                available.clear();
        }



        friend class CommandLauncher;

        GNC::GCS::ILockable poolLock;
        bool                m_Shutdown;

        std::deque<GNC::CommandLauncher*> available;
        std::set<GNC::CommandLauncher*> working;

};
}


static GNC::ThreadPool* threadPool = NULL;


GNC::CommandLauncher::CommandLauncher(GNC::ThreadPool* pool)
{
        //std::cout << "CommandLauncher::CommandLauncher()" << this << std::endl;
        m_idThreadGinkgo = 0;
        m_pComando = NULL;
        m_Abortar = false;
        m_Shutdown = false;
        m_pPool = pool;
        m_pParker = new wxSemaphore(0);
        Create();
        Run();
        GNC::GCS::Threading::SetThreadName( GetId(), "Ginkgo Thread");
}

GNC::CommandLauncher::~CommandLauncher()
{

}


GNC::GCS::IComando* GNC::CommandLauncher::GetCommand()
{
        return m_pComando;
}

void GNC::CommandLauncher::SetCommand(GNC::GCS::IComando* pCmd)
{
        if (m_pComando != NULL) {
                //std::cout << "Error: Already has a command" << std::endl;
        }
        if (pCmd != NULL) {
                pCmd->SetNotificadorProgreso(this);
        }
        m_pComando = pCmd;
        m_Abortar = false;
}

void GNC::CommandLauncher::Park()
{

        wxSemaError ret = wxSEMA_TIMEOUT;
        m_pPool->Return(this);

        while (!m_Shutdown && !m_Abortar && ret == wxSEMA_TIMEOUT) {
                ret = m_pParker->WaitTimeout(POLL_PERIOD);
        }
}

void GNC::CommandLauncher::UnPark()
{
        m_pParker->Post();
}

void* GNC::CommandLauncher::Entry()
{

        while (!m_Shutdown) {

                Park();
                if (m_Shutdown) {
                        return NULL;
                }

                this->IniciarUnlocker();

                //GNC::GCS::Threading::SetThreadName( GetId(), m_pComando->GetName());

                if (m_pComando != NULL && !m_pComando->EstaAbortado() && m_Dependencias.Size() > 0) {
                        while (m_Dependencias.NotEmpty()&& !m_pComando->EstaAbortado()) {
                                if (!m_Dependencias.Wait(POLL_PERIOD)) {
                                        //std::cout << "Timeout de espera para " << m_pComando->GetName() << "(" << m_idThreadGinkgo << ") " << m_Dependencias.Size() << " pendientes" << std::endl;
                                        m_Dependencias.TerminarPendientes();
                                }
                        }
                }

                wxWindow* w = GNC::GCS::IEntorno::Instance()->GetVentanaPrincipal();

                if (m_pComando != NULL && !m_pComando->EstaAbortado()) {

                        if (w != NULL) {
                                EventoProgreso ce(EventoProgreso::ComandoIniciado, m_idThreadGinkgo);
                                w->GetEventHandler()->AddPendingEvent(ce);
                        }

                        try {
                                // don't add the lock for GNC::CommandController::Instance() here,
                                // even though running tread sanitizer might suggest that this is a
                                //  good idea
                                m_pComando->Execute();
                        } catch (GNC::GCS::CommandControllerException& ) {
                                //comando abortado
                        } catch (const std::bad_alloc& ) {
                                //comando ha cascado por un bad alloc
                        } catch (const std::exception& ) {
                                //comando abortado
                        } catch (...) {
                                LOG_ERROR("CommandLauncher", "Error interno ejecutando el comando con id = " << m_pComando->GetId());
                        }

                        OnExit();
                } else if (m_pComando != NULL && m_pComando->EstaAbortado()) {
                        //aborted commands have to signal to free their dependencies
                        OnExit();
                }

        }
        return NULL;
}

bool GNC::CommandLauncher::RecalcularDependencias()
{
        //revisamos la lista de "aborta si" ya que en ese caso no arrancamos

        if(m_Abortar) {
                return false;
        }

        GNC::GCS::IPersistentCommand* pPersistent = dynamic_cast<GNC::GCS::IPersistentCommand*>(m_pComando);

        //GTRACE(">> comprobando el ''AbortaSi'' y el ''CancelaA'' : " << m_idThreadGinkgo);
        GNC::CommandController::MapaComandos* mapaComandosLanzados = &(GNC::CommandController::Instance()->m_ComandosLanzados);
        for(GNC::CommandController::MapaComandos::iterator it = mapaComandosLanzados->begin(); it!= mapaComandosLanzados->end(); ++it) {
                CommandLauncher* pLauncher = (*it).second;
                if (pLauncher == this) {
                        continue;
                }

                for(GNC::GCS::IComando::TipoListaIdComandos::iterator it1 = m_pComando->m_Conflictos.begin(); it1 != m_pComando->m_Conflictos.end(); ++it1) {
                        if(pLauncher->GetCommand() != NULL && pLauncher->GetCommand()->GetId() == (*it1) && pLauncher->m_idThreadGinkgo != m_idThreadGinkgo) {
                                return false;
                        }
                }
                //cancel persistent commands with same idBBDD
                if (pPersistent != NULL) {
                        GNC::GCS::IPersistentCommand* pPersistent2 = dynamic_cast<GNC::GCS::IPersistentCommand*>(pLauncher->GetCommand());
                        if (pPersistent2 != NULL && pPersistent2->getIdBBDD() == pPersistent->getIdBBDD()) {
                                return false;
                        }
                }
        }

        for(GNC::CommandController::MapaComandos::iterator it = mapaComandosLanzados->begin(); it!= mapaComandosLanzados->end(); ++it) {
                CommandLauncher* pLauncher = (*it).second;
                if (pLauncher == this) {
                        continue;
                }

                for(GNC::GCS::IComando::TipoListaIdComandos::iterator it1 = m_pComando->m_Reemplazos.begin(); it1 != m_pComando->m_Reemplazos.end(); ++it1) {
                        if(pLauncher != NULL && pLauncher->GetCommand()!=NULL && pLauncher->GetCommand()->GetId() == (*it1) && pLauncher->m_idThreadGinkgo != m_idThreadGinkgo) {
                                m_Dependencias.RegistrarEspera(pLauncher);
                                pLauncher->Terminar();
                        } else if (pLauncher == NULL) {
                                LOG_ERROR("CommandsController","Inconsistency detected: Launched command was dereferenced");
                        }
                }

                for(GNC::GCS::IComando::TipoListaIdComandos::iterator it1 = m_pComando->m_Dependencias.begin(); it1 != m_pComando->m_Dependencias.end(); ++it1) {
                        if(pLauncher != NULL && pLauncher->GetCommand()!=NULL && pLauncher->GetCommand()->GetId() == (*it1) && pLauncher->m_idThreadGinkgo != m_idThreadGinkgo) {
                                //GTRACE(">> encontrado un comando de ''Espera a'' => El hilo " << m_idThreadGinkgo<< " esprerara a "<<pLauncher->m_idThreadGinkgo);
                                //nos aseguramos de que el no esta esperando por nosostros... en tal caso habría un bloqueo mutuo!!!
                                //solo se permite en caso de que los dos comandos tengan el mismo id por ejemplo comandos que se tienen que ejecutar en serie,
                                //en tal caso solo le esperamos si su id es menor que el nuestro
                                if(pLauncher->GetCommand()->GetId() != this->GetCommand()->GetId() ||
                                    (pLauncher->GetCommand()->GetId() == this->GetCommand()->GetId() && m_idThreadGinkgo > pLauncher->m_idThreadGinkgo)) {
                                        m_Dependencias.RegistrarEspera(pLauncher);
                                }
                        }
                }
        }
        return true;
}

void GNC::CommandLauncher::OnExit()
{
        {
                GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());

                //region "Region sincrona con CommandController"
                if (m_pComando != NULL) {
                        m_pComando->SetNotificadorProgreso(NULL);
                }

                GNC::CommandController::MapaComandos::iterator it = GNC::CommandController::Instance()->m_ComandosLanzados.find(m_idThreadGinkgo);
                if (it != GNC::CommandController::Instance()->m_ComandosLanzados.end()) {
                        GNC::CommandController::Instance()->m_ComandosLanzados.erase(it);
                }

                //avisa a los que estan esperando

                wxWindow* w = GNC::GCS::IEntorno::Instance()->GetVentanaPrincipal();

                if (w != NULL) {
                        GNC::CommandController::Instance()->m_ComandosTerminados[m_idThreadGinkgo] = m_pComando;
                        EventoProgreso ce(EventoProgreso::ComandoFinalizado, m_idThreadGinkgo);
                        w->GetEventHandler()->AddPendingEvent(ce);
                } else {
                        LOG_WARN("CommandLauncher::OnExit()", "Error: No se invocara el evento de finalizacion del comando. El sistema puede quedar bloqueado o inestable");
                        if (m_pComando != NULL) {
                                m_pComando->Abort(); // Notificamos al comando de que va a abortar.
                                delete m_pComando;
                                m_pComando = NULL;
                        }
                }
        }

        Signal();

        //LOG_TRACE("CommandLauncher", "<< Comando Terminado");

        //endregion
}

void GNC::CommandLauncher::IniciarUnlocker()
{
#if defined(_WINDOWS)

#else
        /*
        struct sigaction action;
        memset(&action, 0, sizeof(action));
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        action.sa_handler = FooSignalHandler;
        sigaction(SIGUSR2, &action, NULL);
        */
#endif
}

void GNC::CommandLauncher::DoTerminar()
{
        this->m_Abortar = true;
        if (m_pComando != NULL) {
                //std::cout << "CommandLauncher::DoTerminar(): Abortando comando:" << std::endl;
                m_pComando->Abort();
                GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoProgresoComando(GetCommand(), GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado, 0.0f, GetCommand()->GetName()));
        }
#if defined(_WINDOWS)

#else
        //pthread_kill((pthread_t)this->GetId(), SIGUSR2);

#endif
}

bool GNC::CommandLauncher::NotificarProgreso(float progresoNormalizado, const std::string& texto)
{
        if(m_Abortar) {
                return false;
        }
        //this protect system to overflow event message queue
        bool sendEvent = false;
        const float newProgress = std::max<float>(0, std::min<float>(progresoNormalizado,1.0f));
        LockProgreso();
        sendEvent = (std::abs<float>(m_ProgresoNormalizado - newProgress) > 0.01);
        SetProgresoNormalizado(newProgress);
        sendEvent = sendEvent || (texto != m_Texto);
        SetTextoProgreso(texto);
        UnLockProgreso();

        wxWindow* w = GNC::GCS::IEntorno::Instance()->GetVentanaPrincipal();
        if(sendEvent && w!=NULL) {
                EventoProgreso* ce = new EventoProgreso(EventoProgreso::ComandoEjecutando, m_idThreadGinkgo);
                wxQueueEvent(w->GetEventHandler(), ce);
        }
        return true;

}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

//region Controlador de comandos

GNC::CommandController* GNC::CommandController::m_psInstancia = NULL;

GNC::CommandController::CommandController():
        m_Destroying(false),
        m_EsperarComandos(false),
        m_pProgreso(nullptr),
        m_idThreads(1)
{
        threadPool = new GNC::ThreadPool();
}

GNC::CommandController::~CommandController()
{
        GTRACE(">> CommandController::~CommandController() ");
        AbortarComandosDeOwner(NULL);
        GTRACE("<< CommandController::~CommandController() ");
}

GNC::CommandController* GNC::CommandController::Instance()
{
        if (m_psInstancia == NULL) {
                m_psInstancia = new CommandController();
        }
        return m_psInstancia;
}

void GNC::CommandController::FreeInstance()
{
        if (m_psInstancia != NULL) {
                delete m_psInstancia;
                m_psInstancia = NULL;
        }

}

void GNC::CommandController::ProcessSync(GNC::GCS::IComando* cmd, bool autodelete, bool update, GNC::INotificadorProgreso* pNotifier)
{
        if (cmd == NULL) {
                return;
        }
        if (pNotifier != NULL) {
                cmd->SetNotificadorProgreso(pNotifier);
        }
        cmd->Execute();
        if (wxThread::IsMain() && update) {
                cmd->Update();
        }
        if (autodelete) {
                cmd->Free();
                delete cmd;
        }
}

void GNC::CommandController::ProcessAsync(const std::string& /*str*/, GNC::GCS::IComando* cmd, void * owner)
{
        // in the release build we don't want to crash the program here
        if (!cmd) {
                LOG_ERROR("CommandController", "Initiate command with an empty command structure");
                return;
        }

        cmd->SetOwner(owner);
        cmd->SetNotificadorProgreso(NULL);
        CommandLauncher* thread = threadPool->Borrow();
        if (thread) {
                thread->SetCommand(cmd);

                //thread->SetPriority(WXTHREAD_MAX_PRIORITY);

                GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());

                long threadId = m_idThreads++;
                thread->m_idThreadGinkgo = threadId;

                try {
                        if (thread->RecalcularDependencias()) {
                                //if command is persistent... save it
                                GNC::GCS::IPersistentCommand* pPersistent = dynamic_cast<GNC::GCS::IPersistentCommand*>(cmd);
                                if (pPersistent != NULL) {
                                        GNC::GCS::HistoryController::Instance()->SaveTask(pPersistent);
                                }
                        } else {
                                thread->SetCommand(NULL);
                                cmd->Free();
                                delete cmd;

                                thread = NULL;
                        }
                } catch (...) {
                        LOG_ERROR("CommandController", "Error interno al lanzar el comando");
                        if (cmd != NULL) {
                                thread->SetCommand(NULL);
                                cmd->Free();
                                delete cmd;
                        }
                        thread = NULL;
                }
        } else {
                if (cmd != NULL) {
                        thread->SetCommand(NULL);
                        cmd->Free();
                        delete cmd;
                }
        }

        if (thread != NULL) {
                m_ComandosLanzados[thread->m_idThreadGinkgo] = thread;
                MapaOwners::iterator it = m_MapaOwners.find(owner);
                if (it != m_MapaOwners.end()) {
                        (*it).second.push_back(thread->m_idThreadGinkgo);
                } else {
                        ListaHilos l;
                        l.push_back(thread->m_idThreadGinkgo);
                        m_MapaOwners[owner] = l;
                }
                thread->UnPark();
        }
}

//este metodo es un poco malo porque la busqueda iterada...
void GNC::CommandController::AbortarComando(GNC::GCS::IComando* pComando, bool sincrono)
{
        GNC::GCS::WaitQueue wqueue;
        GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());
        // Recorremos la lista de threads_ids de la vista
        for(MapaComandos::iterator it = m_ComandosLanzados.begin(); it!= m_ComandosLanzados.end(); ++it) {
                CommandLauncher* pLauncher = (*it).second;
                if(pLauncher == NULL || (*it).second->GetCommand() != pComando) {
                        continue;
                }
                if (sincrono ) {
                        wqueue.RegistrarEspera(pLauncher);
                        std::cerr << "Registrando espera para tarea: ptr = " << pLauncher << std::endl;
                }
                pLauncher->Terminar();
        }
}

void GNC::CommandController::AbortarComando(long threadId, bool sincrono)
{
        GNC::GCS::WaitQueue wqueue;

        //---------------------------------------------------------------------
        // REGION CRITICA
        //---------------------------------------------------------------------
        {
                // BEGIN Modificación del estado interno del controlador de comandos.
                GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());
                // Recorremos la lista de threads_ids de la vista
                MapaComandos::iterator it = m_ComandosLanzados.find(threadId);
                if (it != m_ComandosLanzados.end()) {
                        CommandLauncher* pLauncher = (*it).second;
                        if (pLauncher != NULL) { //
                                if (sincrono ) {
                                        wqueue.RegistrarEspera(pLauncher);
                                        std::cerr << "Registrando espera para tarea: threadId = " << (unsigned long) threadId << " ptr = " << pLauncher << std::endl;
                                }
                                pLauncher->Terminar();
                                pLauncher->SetCommand(NULL);
                        } else {
                                std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando lanzado desreferenciado. Flujo de comando perdido." << std::endl;
                        }
                } else { // Buscamos el id en la lista de comandos terminados
                        MapaComandosTerminados::iterator it3 = m_ComandosTerminados.find(threadId);
                        if (it3 != m_ComandosTerminados.end()) {
                                GNC::GCS::IComando* pComando = (*it3).second;
                                if (pComando != NULL) {
                                        std::cerr << "Anulando comando terminado: threadId = " << (unsigned long) threadId << std::endl;

                                        pComando->Abort();
                                        pComando->Free();
                                        if (pComando != NULL) {
                                                pComando->Free();
                                                delete pComando;
                                                pComando = NULL;
                                        }

                                } else {
                                        std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando terminado nulo." << std::endl;
                                }
                                m_ComandosTerminados.erase(it3);
                        } else {
                                std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando desreferenciado. Flujo de comando perdido." << std::endl;
                        }
                }
        }
        // END Modificación del estado interno del controlador de comandos.
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        while (wqueue.NotEmpty()) {
                if (!wqueue.Wait(POLL_PERIOD)) {
                        wqueue.TerminarPendientes( );
                }
        }
}

void GNC::CommandController::AbortarComandosDeOwner(void * owner)
{

        GNC::GCS::WaitQueue wqueue;

        {
                GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());
                // Recorremos la lista de threads_ids de la vista
                MapaOwners::iterator it1 = m_MapaOwners.find(owner);
                MapaComandos::iterator itcl;
                if (it1 != m_MapaOwners.end()) {
                        for (ListaHilos::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2) {
                                long threadId = (*it2);
                                // Buscamos el id en la lista de comandos lanzados
                                itcl = m_ComandosLanzados.find(threadId);
                                if (itcl != m_ComandosLanzados.end()) { // El comando esta lanzado
                                        CommandLauncher* pLauncher = (*itcl).second;
                                        if (pLauncher != NULL) { //

                                                wqueue.RegistrarEspera(pLauncher);
                                                pLauncher->Terminar();

                                                std::cerr << "Registrando espera para tarea: threadId = " << (unsigned long) threadId << " ptr = " << pLauncher << std::endl;
                                        } else {
                                                std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando lanzado desreferenciado. Flujo de comando perdido." << std::endl;
                                        }
                                } else { // Buscamos el id en la lista de comandos terminados
                                        MapaComandosTerminados::iterator it3 = m_ComandosTerminados.find(threadId);
                                        if (it3 != m_ComandosTerminados.end()) {
                                                GNC::GCS::IComando* pComando = (*it3).second;
                                                if (pComando != NULL) {
                                                        std::cerr << "Anulando comando terminado: threadId = " << (unsigned long) threadId << std::endl;
                                                        pComando->Abort();
                                                        pComando->Free();
                                                        delete pComando;
                                                        pComando = NULL;
                                                } else {
                                                        std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando terminado nulo." << std::endl;
                                                }
                                                m_ComandosTerminados.erase(it3);
                                        } else {
                                                std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando desreferenciado. Flujo de comando perdido." << std::endl;
                                        }
                                }
                        }
                        m_MapaOwners.erase(it1);
                }
        }

        while (wqueue.NotEmpty()) {
                if (!wqueue.Wait(POLL_PERIOD)) {
                        wqueue.TerminarPendientes();
                }
        }
}

void GNC::CommandController::AbortarComandosDeOwnerAsincrono(void * owner)
{
        GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());
        // Recorremos la lista de threads_ids de la vista
        MapaOwners::iterator it1 = m_MapaOwners.find(owner);
        if (it1 != m_MapaOwners.end()) {
                for (ListaHilos::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2) {
                        long threadId = (*it2);
                        // Buscamos el id en la lista de comandos lanzados
                        MapaComandos::iterator it = m_ComandosLanzados.find(threadId);
                        if (it != m_ComandosLanzados.end()) { // El comando esta lanzado
                                CommandLauncher* pLauncher = (*it).second;
                                if (pLauncher != NULL) { //
                                        pLauncher->Terminar();
                                } else {
                                        std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando lanzado desreferenciado. Flujo de comando perdido." << std::endl;
                                }
                        } else { // Buscamos el id en la lista de comandos terminados
                                MapaComandosTerminados::iterator it3 = m_ComandosTerminados.find(threadId);
                                if (it3 != m_ComandosTerminados.end()) {
                                        GNC::GCS::IComando* pComando = (*it3).second;
                                        if (pComando != NULL) {
                                                std::cerr << "Anulando comando terminado: threadId = " << (unsigned long) threadId << std::endl;
                                                pComando->Abort();
                                                pComando->Free();
                                                delete pComando;
                                                pComando = NULL;
                                        } else {
                                                std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando terminado nulo." << std::endl;
                                        }
                                        m_ComandosTerminados.erase(it3);
                                } else {
                                        std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando desreferenciado. Flujo de comando perdido." << std::endl;
                                }
                        }
                }
                m_MapaOwners.erase(it1);
        }
}

void GNC::CommandController::WaitToOwnerCommands(void* owner)
{
        GNC::GCS::WaitQueue wqueue;

        {
                GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());
                // Recorremos la lista de threads_ids de la vista
                MapaOwners::iterator it1 = m_MapaOwners.find(owner);
                MapaComandos::iterator itcl;
                if (it1 != m_MapaOwners.end()) {
                        for (ListaHilos::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2) {
                                long threadId = (*it2);
                                // Buscamos el id en la lista de comandos lanzados
                                itcl = m_ComandosLanzados.find(threadId);
                                if (itcl != m_ComandosLanzados.end()) { // El comando esta lanzado
                                        CommandLauncher* pLauncher = (*itcl).second;
                                        if (pLauncher != NULL) { //

                                                wqueue.RegistrarEspera(pLauncher);
                                                pLauncher->Terminar();

                                                std::cerr << "Registrando espera para tarea: threadId = " << (unsigned long) threadId << " ptr = " << pLauncher << std::endl;
                                        } else {
                                                std::cerr << "Detectada inconsistencia en el controlador de comandos: Comando lanzado desreferenciado. Flujo de comando perdido." << std::endl;
                                        }
                                }
                        }
                }
        }

        while (wqueue.NotEmpty()) {
                if (!wqueue.Wait(POLL_PERIOD)) {
                }
        }
}

unsigned int GNC::CommandController::GetNumActiveCommands()
{
        GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());
        return m_ComandosLanzados.size();
}

void GNC::CommandController::OnComandoLanzado(long threadId)
{

        GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());

        MapaComandos::iterator it = m_ComandosLanzados.find(threadId);
        if (it != m_ComandosLanzados.end()) {
                CommandLauncher* pLauncher = (*it).second;
                pLauncher->GetCommand()->SetNotificadorProgreso(pLauncher);
                if (m_pProgreso) {
                        m_pProgreso->InsertarTarea(threadId, pLauncher->GetCommand()->GetName());
                }
                GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoProgresoComando(pLauncher->GetCommand(), GNC::GCS::Events::EventoProgresoComando::TEP_Iniciado, 0.0f, pLauncher->GetCommand()->GetName()));
        } else {
                MapaComandosTerminados::iterator it = m_ComandosTerminados.find(threadId);
                if (it != m_ComandosTerminados.end()) {
                        GNC::GCS::IComando* pComando = (*it).second;
                        if (m_pProgreso) {
                                m_pProgreso->InsertarTarea(threadId, pComando->GetName());
                        }
                        GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoProgresoComando(pComando, GNC::GCS::Events::EventoProgresoComando::TEP_Iniciado, 1.0f, pComando->GetName()));
                } else {
                        LOG_WARN("CommandController", "Error al iniciar comunicacion de progreso del comando. Thread Id no encontrado.");
                        //std::cout << "OnComandoLanzado: Thread Id not found: " << threadId << std::endl;
                }
        }
}

void GNC::CommandController::OnComandoProgreso(long threadId)
{
        GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());

        MapaComandos::iterator it = m_ComandosLanzados.find(threadId);
        if (it != m_ComandosLanzados.end()) {
                CommandLauncher* pLauncher = (*it).second;
                pLauncher->LockProgreso();
                float progreso = pLauncher->GetProgresoNormalizado();
                std::string texto = pLauncher->GetTextoProgreso();
                pLauncher->UnLockProgreso();
                m_pProgreso->SetProgresoTarea(threadId, progreso, texto);
                GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoProgresoComando(pLauncher->GetCommand(), GNC::GCS::Events::EventoProgresoComando::TEP_Progreso, progreso, texto));
        } else {
                //std::cout << "OnComandoProgreso: Thread Id not found: " << threadId << std::endl;
        }

}

void GNC::CommandController::OnComandoFinalizado(long threadId, bool lock)
{
        GNC::GCS::IComando* pComando = NULL;

        bool actualizar=false;

        GNC::GCS::ILocker* pLocker = NULL;

        //LOG_TRACE("CommandController", "OnComandoFinalizado(" << threadId << ")");

        {
                if (lock) {
                        pLocker = new GNC::GCS::ILocker(GNC::CommandController::Instance());
                }
                try {
                        if(m_pProgreso != NULL) {
                                m_pProgreso->EliminarTarea(threadId);
                        }

                        MapaComandosTerminados::iterator it1 = m_ComandosTerminados.find(threadId);
                        if (it1 != m_ComandosTerminados.end()) {
                                actualizar = true;
                                pComando = (*it1).second;
                                m_ComandosTerminados.erase(it1);
                        } else {
                                //LOG_TRACE("CommandController", "Id thread no encontrado en finalizados");
                        }


                        // TODO: Añadir un diccionario inverso o usar un set
                        for (MapaOwners::iterator it2 = m_MapaOwners.begin(); it2 != m_MapaOwners.end(); ++it2) {
                                ListaHilos::iterator it3 = (*it2).second.begin();
                                bool found = false;
                                while (!found && it3 != (*it2).second.end()) {
                                        if ((*it3) == threadId) {
                                                found = true;
                                        } else {
                                                ++it3;
                                        }
                                }
                                if (found) {
                                        (*it2).second.erase(it3);
                                }
                        }
                } catch (std::exception&) {
                        throw;
                }

                if (pLocker != NULL) {
                        delete pLocker;
                        pLocker = NULL;
                }
        }

        if (pComando != NULL) {
                if (actualizar && !pComando->EstaAbortado()) {
                        pComando->Update();
                        //if command is persistent... drop from bbdd
                        GNC::GCS::IPersistentCommand* pPersistent = dynamic_cast<GNC::GCS::IPersistentCommand*>(pComando);
                        if (pPersistent != NULL) {
                                GNC::GCS::HistoryController::Instance()->SaveTask(pPersistent);
                        }
                } else {
                        LOG_DEBUG("CommandController" , "IComando::Update no invocado");
                }
                //LOG_TRACE("CommandController", "Procesando evento de comando finalizado");
                GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoProgresoComando(pComando, GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado));
                pComando->Free();
                delete pComando;
                pComando = NULL;
        } else {
                LOG_WARN("CommandController", "Comando nulo. no se propagara el evento de fin de comando ni se actualizara");
                //std::cout << "CommandController>> pComando = NULL!!!!" << std::endl;
        }

}

GNC::CommandController::ListaNombreComandos GNC::CommandController::GetComandosActivos()
{
        ListaNombreComandos lista;
        GNC::GCS::ILocker pLocker(GNC::CommandController::Instance());
        for(MapaComandos::iterator it = m_ComandosLanzados.begin(); it!=m_ComandosLanzados.end(); ++it) {
                std::stringstream strstream;
                const GNC::GCS::IComando* pCmd =  (*it).second->GetCommand();
                if (pCmd != NULL) {
                        strstream << pCmd->GetName();
                } else {
                        strstream << "<NULL>";
                }
                strstream << ": " << (*it).second->GetTextoProgreso();
                lista.push_back( strstream.str());
        }
        return lista;
}

//endregion
