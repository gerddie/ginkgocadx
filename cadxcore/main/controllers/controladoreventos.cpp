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

#include "controladoreventos.h"
#include <api/ientorno.h>
#include <api/ievento.h>
#include <api/iexception.h>
#include <api/controllers/icontroladorlog.h>
#include <api/observers/ieventsobserver.h>
#include <api/internationalization/internationalization.h>
#include "inactivitycontroller.h"

namespace GNC
{
namespace GCS
{

ControladorEventos * ControladorEventos::m_psInstancia = NULL;

ControladorEventos::ControladorEventos()
{
        GTRACE("ControladorEventos::ControladorEventos()");
}

ControladorEventos::~ControladorEventos()
{
        //se vacia el mapa de vistas
        for(TipoMapaVistas::iterator itVistas = m_mapaVistas.begin(); itVistas != m_mapaVistas.end(); ++itVistas) {
                for(TipoMapaAmbito::iterator itAmbito =  (*itVistas).second.begin(); itAmbito != (*itVistas).second.end(); ++ itAmbito) {
                        for(TipoMapaCodigos::iterator itCodigos = (*itAmbito).second.begin(); itCodigos != (*itAmbito).second.end(); ++itCodigos) {
                                for(TipoListaObservadores::iterator itObservadores = (*itCodigos).second.begin(); itObservadores != (*itCodigos).second.end(); ++itObservadores) {
                                        (*itObservadores) = NULL;
                                }
                                (*itCodigos).second.clear();
                        }
                        (*itAmbito).second.clear();
                }
                (*itVistas).second.clear();
        }
        m_mapaVistas.clear();

        //se vacia el mapa de eventos
        for(TipoMapaEventos::iterator it = m_mapaEventos.begin(); it!= m_mapaEventos.end(); ++it) {
                (*it).second.clear();
        }
        m_mapaEventos.clear();
}


//se apunta a escuchar un evento
void ControladorEventos::Registrar ( GNC::GCS::IEventsObserver* pObservador, const GNC::GCS::Events::IEvent& evento)
{
        wxCriticalSectionLocker pLocker(m_eventosCriticalSection);

        //se mete en el mapa observadores->eventos
        {
                TipoMapaEventos::iterator itEventos = m_mapaEventos.find(pObservador);
                if(itEventos == m_mapaEventos.end()) {
                        TipoListaEventos lista;
                        m_mapaEventos[pObservador] = lista;
                        itEventos = m_mapaEventos.find(pObservador);
                }
                //se busca si ya esta el evento
                for (TipoListaEventos::iterator it = (*itEventos).second.begin(); it != (*itEventos).second.end(); ++it) {
                        if((*it).IsEqual(evento)) {
                                LOG_WARN("ControladorEventos", "Se está intentando registrar dos veces al mismo evento");
                                return;
                        }
                }
                (*itEventos).second.push_back(evento);
        }

        //se mete en el mapa vistas->eventos
        {
                TipoMapaVistas::iterator itVistas;
                TipoMapaAmbito::iterator itAmbito;
                TipoMapaCodigos::iterator itCodigos;

                //se busca la entrada en el mapa de vistas
                itVistas= m_mapaVistas.find(evento.GetVista());
                if(itVistas == m_mapaVistas.end()) {
                        TipoMapaAmbito mapa;
                        m_mapaVistas[evento.GetVista()] = mapa;
                        itVistas = m_mapaVistas.find(evento.GetVista());
                }
                //se busca la entrada en el mapa de ambitos
                itAmbito = (*itVistas).second.find(evento.GetCodigoAmbito());
                if(itAmbito == (*itVistas).second.end()) {
                        TipoMapaCodigos mapa;
                        (*itVistas).second[evento.GetCodigoAmbito()] = mapa;
                        itAmbito = (*itVistas).second.find(evento.GetCodigoAmbito());
                }

                itCodigos = (*itAmbito).second.find(evento.GetCodigoEvento());
                if(itCodigos == (*itAmbito).second.end()) {
                        TipoListaObservadores lista;
                        (*itAmbito).second[evento.GetCodigoEvento()] = lista;
                        itCodigos = (*itAmbito).second.find(evento.GetCodigoEvento());
                }

                (*itCodigos).second.push_back(pObservador);
        }

        //pObservador->OnRegistrar(this);
}

//se desapunta de escuchar un evento
void ControladorEventos::DesRegistrar (GNC::GCS::IEventsObserver* pObservador, const GNC::GCS::Events::IEvent& evento)
{
        wxCriticalSectionLocker* pLocker = new wxCriticalSectionLocker(m_eventosCriticalSection);

        TipoMapaVistas::iterator itVistas;
        TipoMapaAmbito::iterator itAmbito;
        TipoMapaCodigos::iterator itCodigos;

        //se busca la entrada en el mapa de vistas
        itVistas= m_mapaVistas.find(evento.GetVista());
        if(itVistas != m_mapaVistas.end()) {
                //se busca la entrada en el mapa de ambitos
                itAmbito = (*itVistas).second.find(evento.GetCodigoAmbito());
                if(itAmbito != (*itVistas).second.end()) {
                        //se busca en el mapa de codigos
                        itCodigos = (*itAmbito).second.find(evento.GetCodigoEvento());
                        if(itCodigos != (*itAmbito).second.end()) {
                                (*itCodigos).second.remove(pObservador);
                                //se purgan los mapas
                                if((*itCodigos).second.size() == 0) {
                                        // FIXME: Race condition
                                        (*itAmbito).second.erase(itCodigos);
                                        if((*itAmbito).second.size() == 0) {
                                                (*itVistas).second.erase(itAmbito);
                                                if((*itVistas).second.size() == 0) {
                                                        m_mapaVistas.erase(itVistas);
                                                }
                                        }
                                }
                        }

                }
        }

        //se elimina del mapa de eventos
        TipoMapaEventos::iterator itEventos = m_mapaEventos.find(pObservador);
        if(itEventos != m_mapaEventos.end()) {
                for(TipoListaEventos::iterator it = (*itEventos).second.begin(); it!= (*itEventos).second.end(); ++it) {
                        if((*it).IsEqual(evento)) {
                                (*itEventos).second.erase(it);
                                break;
                        }
                }
                itEventos = m_mapaEventos.find(pObservador);
                if((*itEventos).second.size() == 0) {
                        m_mapaEventos.erase(itEventos);
                }
        }

        delete pLocker;
}

//se desapunta de todos los eventos a los que escucha
void ControladorEventos::DesRegistrar (GNC::GCS::IEventsObserver* pObservador)
{
        for(TipoMapaEventos::iterator itEventos = m_mapaEventos.find(pObservador); itEventos != m_mapaEventos.end(); itEventos = m_mapaEventos.find(pObservador)) {
                //desregistramos el primer evento de la lista
                if((*itEventos).second.size() > 0) {
                        DesRegistrar(pObservador,(*itEventos).second.front());
                } else {
                        m_mapaEventos.erase(itEventos);
                        break;
                }
        }
}

//procesado sincrono con la interfaz
void ControladorEventos::ProcesarEvento(GNC::GCS::Events::IEvent *pEvt)
{
        GNC::GCS::InactivityController::Instance()->ResetsInactivity();

        wxCriticalSectionLocker* pLocker = new wxCriticalSectionLocker(m_eventosCriticalSection);
        //Se buscan los notificadores
        TipoListaObservadores listaObservadores;
        TipoMapaVistas::iterator itVistas;
        TipoMapaAmbito::iterator itAmbito;
        TipoMapaCodigos::iterator itCodigos;

        itVistas= m_mapaVistas.find(pEvt->GetVista());
        if(itVistas != m_mapaVistas.end()) {
                //se busca la entrada en el mapa de ambitos
                itAmbito = (*itVistas).second.find(pEvt->GetCodigoAmbito());
                if(itAmbito != (*itVistas).second.end()) {
                        //se busca en el mapa de codigos
                        itCodigos = (*itAmbito).second.find(pEvt->GetCodigoEvento());
                        if(itCodigos != (*itAmbito).second.end()) {
                                listaObservadores = (*itCodigos).second;
                        }
                }
        }
        delete pLocker;

        NotificarEvento(pEvt,listaObservadores);
}

void FreeEvents(const std::list<GNC::GCS::Events::IEvent*>& lista)
{
        for(std::list<GNC::GCS::Events::IEvent*>::const_iterator itEventos = lista.begin(); itEventos != lista.end(); ++itEventos) {
                delete *itEventos;
        }
}

//inyecta en la cola correspondiente el evento en cuestion
void ControladorEventos::NotificarEvento(GNC::GCS::Events::IEvent *pEvt, TipoListaObservadores & listaObservadores)
{
#ifdef _GINKGO_TRACE
        std::cout << "<< Evento: " << pEvt << std::endl;
#endif

        std::list<GNC::GCS::Events::IEvent*> lista;

        try {
                lista.push_back(pEvt);
                for(TipoListaObservadores::iterator it = listaObservadores.begin(); it != listaObservadores.end(); ++it) {
                        if((*it) != NULL) {
                                if(!(*it)->FiltrarEvento(pEvt)) {
                                        (*it)->PreProcesarEvento(pEvt,lista);
                                }
                        }
                }
#ifdef _GINKGO_TRACE
                int i1 = 0;
                for (TipoListaPunterosEventos::iterator it = lista.begin(); it != lista.end(); ++it) {
                        std::cout << "Lista de eventos generada:" << std::endl;
                        std::cout << " " << i1++ << " Evento: " << *it << std::endl;
                }
#endif

                // Etapa de compactacion de eventos repetidos

                unsigned int i = lista.size();

                for(std::list<GNC::GCS::Events::IEvent*>::reverse_iterator itCur = lista.rbegin(); itCur != lista.rend(); ++itCur, --i) {
                        GNC::GCS::Events::IEvent *pCur = *itCur;
                        unsigned int j = 1;
                        for (TipoListaPunterosEventos::iterator itPrev = lista.begin(); i != j; ++itPrev, ++j) {
                                GNC::GCS::Events::IEvent *pPrev = *itPrev;
                                if (pPrev->IsEqual(*pCur)) {
                                        pPrev->Skip(true);
                                }
                        }
                }

                for(std::list<GNC::GCS::Events::IEvent*>::iterator itEventos = lista.begin(); itEventos!= lista.end(); ++itEventos) {
                        GNC::GCS::Events::IEvent* pE = (*itEventos);
                        for(TipoListaObservadores::iterator itObservadores = listaObservadores.begin(); pE->IsSkipped() && itObservadores != listaObservadores.end(); ++itObservadores) {
                                GNC::GCS::IEventsObserver* pO = (*itObservadores);
                                if( pO != NULL) {
                                        if(!pO->FiltrarEvento(pE)) {
                                                pO->ProcesarEvento(pE);
                                        }
                                }

                        }
                }
        } catch (GNC::GCS::IException& ex) {
                LOG_ERROR("ControladorEventos", "Exception while processing event " << ex.str());
                FreeEvents(lista);
                throw;
        } catch (std::exception&) {
                FreeEvents(lista);
                throw;
        } catch (...) {
                FreeEvents(lista);
                throw GNC::GCS::IException(_Std("Internal error"), _Std("Unknown"), false);
        }

        FreeEvents(lista);

#ifdef _GINKGO_TRACE
        std::cout << " --- " << std::endl;
#endif
}

ControladorEventos * ControladorEventos::Instance()
{
        if (m_psInstancia == NULL) {
                m_psInstancia = new ControladorEventos();
        }
        return m_psInstancia;
}

void ControladorEventos::FreeInstance()
{
        if (m_psInstancia != NULL) {
                delete m_psInstancia;
                m_psInstancia = NULL;
        }
}
}
}
