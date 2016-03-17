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

#include <list>
#include <map>

#include <api/internal.h>
#include <api/controllers/icommandcontroller.h>
#include <api/icommand.h>
#include <api/controllers/iprogresscontroller.h>
#include <api/ilock.h>
#include <api/iwaitqueue.h>

// TODO: Refactor this
#include <wx/thread.h>

#include <atomic>

//----------------------------------------------------------------------------------------------------
//region forward declarations
namespace GNC
{
class CommandController;
class ThreadPool;
}

class wxWindow;
//endregion

namespace GNC
{

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class EXTAPI CommandLauncher : public wxThread, public GNC::GCS::WaitQueueTask, public GNC::INotificadorProgreso
{
public:
        //id Ginkgo
        unsigned long m_idThreadGinkgo;

        GNC::GCS::IComando* GetCommand();

        void SetCommand(GNC::GCS::IComando* pCmd);

        virtual void* Entry();

        virtual void OnExit();

        bool RecalcularDependencias();

        void IniciarUnlocker();

        void Park();

private:
        void UnPark();

protected:

        virtual void DoTerminar();

private:

        CommandLauncher(GNC::ThreadPool* pool);

        ~CommandLauncher();

        virtual bool NotificarProgreso(float progresoNormalizado, const std::string& texto);

private:

        GNC::GCS::IComando*    m_pComando;

        GNC::GCS::WaitQueue    m_Dependencias;

        // this varable should be an atomic or guarded by a mutex
        std::atomic<bool>      m_Abortar;

        bool                   m_Shutdown;

        wxSemaphore*           m_pParker;

        GNC::ThreadPool*       m_pPool;

#if defined(_WINDOWS)
        //HANDLE              m_hEventoCancelacion;
#else

#endif

        friend class GNC::ThreadPool;
        friend class GNC::CommandController;
};



//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class EXTAPI CommandController : public GNC::GCS::ICommandController, public GNC::GCS::ILockable
{
public:

        typedef std::map<long, CommandLauncher*> MapaComandos;
        typedef std::map<long, GNC::GCS::IComando*> MapaComandosTerminados;
        typedef std::list<long> ListaHilos;
        typedef std::map<void*, ListaHilos > MapaOwners;
        typedef std::list<std::string> ListaNombreComandos;

        static CommandController *Instance();
        static void FreeInstance();

        virtual ~CommandController();
        virtual void ProcessSync(GNC::GCS::IComando* cmd, bool autodelete, bool update = true, GNC::INotificadorProgreso* pNotifier = NULL);
        virtual void ProcessAsync(const std::string& str, GNC::GCS::IComando* cmd, void* owner);
        virtual void AbortarComando(GNC::GCS::IComando* pComando, bool sincrono = true);
        virtual void AbortarComando(long threadId, bool sincrono = true);
        virtual void AbortarComandosDeOwner(void* owner);
        virtual void AbortarComandosDeOwnerAsincrono(void* owner);
        virtual void WaitToOwnerCommands(void* owner);

        virtual unsigned int GetNumActiveCommands();

        void RegistrarProgreso(GNC::IControladorProgreso* pProgreso)
        {
                m_pProgreso = pProgreso;
        }

        GNC::IControladorProgreso* GetProgresoRegistrado()
        {
                return m_pProgreso;
        }

        void OnComandoLanzado(long threadId);
        void OnComandoProgreso(long threadId);
        void OnComandoFinalizado(long threadId, bool lock=true);

        ListaNombreComandos GetComandosActivos();

protected:

        CommandController();

        static CommandController* m_psInstancia;
        bool m_Destroying;

        MapaComandos m_ComandosLanzados;
        MapaComandosTerminados m_ComandosTerminados;
        MapaOwners m_MapaOwners;
        bool m_EsperarComandos;

        GNC::IControladorProgreso* m_pProgreso;

        unsigned long m_idThreads;

        friend class GNC::CommandLauncher;
};

}
