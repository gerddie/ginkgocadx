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
#include <api/internal.h>
#include <api/controllers/ieventscontroller.h>
#include <wx/thread.h>
#include <map>
#include <list>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations

//endregion

namespace GNC
{
namespace GCS
{
class IVista;
}
}

namespace GNC
{

namespace GCS
{

//====================================================================================================
//= Controlador de eventos
//====================================================================================================
class ControladorEventos: public IEventsController
{

public:
        //lista de punteros a eventos
        typedef std::list<GNC::GCS::Events::IEvent*> TipoListaPunterosEventos;

protected:
        typedef std::list<IEventsObserver* > TipoListaObservadores;
        //codigo de evento,observador
        typedef std::map<long , TipoListaObservadores > TipoMapaCodigos;
        //codigo de ambito, lista de observadores
        typedef std::map<long , TipoMapaCodigos> TipoMapaAmbito;
        //vista, ambitos
        typedef std::map<GNC::GCS::IVista* , TipoMapaAmbito > TipoMapaVistas;
        //lista de eventos
        typedef std::list<GNC::GCS::Events::IEvent> TipoListaEventos;
        //observadores,Evtentos para desregistrarse
        typedef std::map<IEventsObserver*, TipoListaEventos> TipoMapaEventos;

public:

        //se apunta a escuchar un evento
        void Registrar ( IEventsObserver* pObservador, const GNC::GCS::Events::IEvent& evento);
        //se desapunta de escuchar un evento
        void DesRegistrar (IEventsObserver* pObservador, const GNC::GCS::Events::IEvent& evento);
        //se desapunta de todos los eventos a los que escucha
        void DesRegistrar (IEventsObserver* pObservador);
        //avisa a los observadores registrados a este evento
        void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

        static ControladorEventos *Instance();
        static void FreeInstance();

        static ControladorEventos* m_psInstancia;

protected:

        ControladorEventos();
        ~ControladorEventos();

        void NotificarEvento(GNC::GCS::Events::IEvent *evt, TipoListaObservadores & listaObservadores);

        //semaforo para controlar las secciones criticas
        wxCriticalSection m_eventosCriticalSection;

        //atributos donde se lleva la cuenta de observadores/eventos
        TipoMapaVistas m_mapaVistas;
        TipoMapaEventos m_mapaEventos;
};
}
}
