#pragma once
/*
 *  
 *  $Id: iwaitqueue.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef IWAITQUEUE_H
#define IWAITQUEUE_H

#include <api/ilock.h>
class wxSemaphore;

namespace GNC {
	namespace GCS {

		class WaitQueueTask;
		class EXTAPI TipoListaEspera : public std::list<WaitQueueTask*, std::allocator<WaitQueueTask*> > {};

		//----------------------------------------------------------------------------------------------------
		//region Cola de espera
		//----------------------------------------------------------------------------------------------------
		class EXTAPI WaitQueue : public GNC::GCS::ILockable {

			//----------------------------------------------------------------------------------------------------
			//region Definiciones
			public:

			//endregion

			//----------------------------------------------------------------------------------------------------
			//region Constructor y destructor
			public:

				WaitQueue();
				~WaitQueue();
			//endregion

			//----------------------------------------------------------------------------------------------------
			//region Interfaz general
			public:

				TipoListaEspera::size_type Size() const;

				bool Empty() const;

				bool NotEmpty() const;

				TipoListaEspera::size_type Pending();

				/* Espera bloqueante hasta que alguna de las tareas registradas termine */
				bool Wait();

				/* Espera bloqueante con limite de tiempo hasta que alguna de las tareas registradas termine */
				bool Wait(long msecs);

				/* Registra una tarea para su espera */
				void RegistrarEspera(WaitQueueTask* task);

				/* Itera sobre todas las tareas pendientes y si tienen el flag de terminacion activo, vuelve a enviarles una senyal de terminacion */
				void TerminarPendientes();
			//endregion

			//----------------------------------------------------------------------------------------------------
			//region Interfaz privada


			//endregion

			//----------------------------------------------------------------------------------------------------
			//region Atributos
			private:

				wxSemaphore*               m_pSignaler;

				TipoListaEspera m_ListaEspera;
				TipoListaEspera::size_type m_Size;

			//endregion

			friend class WaitQueueTask;
		};
		//endregion

		class EXTAPI TipoListaNotificaciones : public std::list<WaitQueue*, std::allocator<WaitQueue*> > {};

		//----------------------------------------------------------------------------------------------------
		// Tarea de la cola de espera
		//----------------------------------------------------------------------------------------------------
		class EXTAPI WaitQueueTask : public GNC::GCS::ILockable {

			//----------------------------------------------------------------------------------------------------
			//region Definiciones
			public:

			//endregion

			//region Interfaz general
			public:
				/* Senala todas las waitqueue a las que se ha registrado esta tarea */
				void Signal();

				/* Metodo que activa el flag de terminacion e invoca a DoTerminar() */
				void Terminar();
			//endregion

			//region Interfaz privada
			protected:
				/* Metodo privado a implementar en la tarea. Implementa la funcionalidad necesaria para que la tarea se detenga (de manera activa o pasiva, segun corresponda) */
				virtual void DoTerminar() = 0;

			//endregion

			//----------------------------------------------------------------------------------------------------
			//region Constructor y destructor
			public:

				WaitQueueTask();

				virtual ~WaitQueueTask();

			//endregion

			//region Atributos
			protected:

				TipoListaNotificaciones            m_ListaNotificacion;
				bool                               m_NotificarTerminacion;
			//endregion

			friend class WaitQueue;
		};
		//endregion

		class EXTAPI NotifyQueue {

		public:
			NotifyQueue();
			~NotifyQueue();

			void Wait();
			void NotifyAll();

			unsigned int Size() const;

			bool Requested() const;

			GNC::GCS::ILockable RegistryLock;
			unsigned int RegistryCount;
			wxSemaphore* NotifyObject;
		};
	}
}
#endif
