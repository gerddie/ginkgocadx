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

#include "controladoracciones.h"
#include "../entorno.h"
#include <wx/thread.h>

#define TAMANIO_MAX_PILA 10
namespace GNC
{
namespace GCS
{
ControladorAcciones* ControladorAcciones::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new ControladorAcciones();
        }
        return m_pInstance;
}

void ControladorAcciones::FreeInstance()
{
        if(m_pInstance!=NULL) {
                delete m_pInstance;
                m_pInstance = NULL;
        }
}

ControladorAcciones* ControladorAcciones::m_pInstance = NULL;

ControladorAcciones::ControladorAcciones()
{
        m_pCriticalSection = new wxCriticalSection();
        GNC::Entorno::Instance()->ViewsObservers.push_back(this);
}
ControladorAcciones::~ControladorAcciones()
{
        GNC::Entorno::Instance()->ViewsObservers.remove(this);
        if(m_pCriticalSection != NULL) {
                delete m_pCriticalSection;
                m_pCriticalSection = NULL;
        }
        m_mapaAcciones.clear();
}

void ControladorAcciones::PushAccion(GNC::GCS::IVista* pVista, GNC::GCS::Ptr<Accion> pAccion)
{
        wxCriticalSectionLocker lock((*m_pCriticalSection));
        if(m_mapaAcciones.find(pVista) == m_mapaAcciones.end()) {
                m_mapaAcciones[pVista] = new TPilaDeshacer();
        }

        TListaAcciones& listaAcciones = m_mapaAcciones[pVista]->m_listaAcciones;
        TListaAcciones::iterator& accionActual = m_mapaAcciones[pVista]->m_iteradorAcciones;
        if(accionActual != listaAcciones.begin()) {
                //este estado es cuando hemos deshecho algun paso, tendremos que vaciar desde el principio hasta el puntero
                while(accionActual != listaAcciones.begin()) {
                        listaAcciones.pop_front();
                }
        }

        listaAcciones.push_front(pAccion);
        if(listaAcciones.size() > TAMANIO_MAX_PILA) {
                listaAcciones.pop_back();
        }
        accionActual = listaAcciones.begin();
}

void ControladorAcciones::Deshacer(GNC::GCS::IVista* pVista)
{
        wxCriticalSectionLocker lock((*m_pCriticalSection));
        if(m_mapaAcciones.find(pVista) != m_mapaAcciones.end()) { //si hay una pila de deshacer
                TListaAcciones& listaAcciones = m_mapaAcciones[pVista]->m_listaAcciones;
                TListaAcciones::iterator& accionActual = m_mapaAcciones[pVista]->m_iteradorAcciones;
                if(accionActual != listaAcciones.end()) { //la accion actual no apunta al final
                        (*accionActual)->Deshacer();
                        ++accionActual;
                }
        }
}

void ControladorAcciones::Hacer(GNC::GCS::IVista* pVista)
{
        wxCriticalSectionLocker lock((*m_pCriticalSection));
        if(m_mapaAcciones.find(pVista) != m_mapaAcciones.end()) { //si hay una pila de deshacer
                TListaAcciones& listaAcciones = m_mapaAcciones[pVista]->m_listaAcciones;
                TListaAcciones::iterator accionActual = m_mapaAcciones[pVista]->m_iteradorAcciones;
                if(accionActual != listaAcciones.begin()) { //y el puntero no esta al principio
                        --accionActual;
                        if((*accionActual)->PuedeRehacer()) { // y la accion se puede rehacer...
                                (*accionActual)->Hacer();
                                m_mapaAcciones[pVista]->m_iteradorAcciones = accionActual;
                        }
                }

        }
}

bool ControladorAcciones::PuedeHacer(GNC::GCS::IVista* pVista, std::string& nombreAccion)
{
        nombreAccion = "";
        wxCriticalSectionLocker lock((*m_pCriticalSection));
        if(m_mapaAcciones.find(pVista) == m_mapaAcciones.end()) {
                return false;
        } else {
                TListaAcciones& listaAcciones = m_mapaAcciones[pVista]->m_listaAcciones;
                TListaAcciones::iterator accionActual = m_mapaAcciones[pVista]->m_iteradorAcciones;
                if(accionActual == listaAcciones.begin()) {
                        return false;
                } else {
                        --accionActual;
                        nombreAccion = (*accionActual)->GetNombre();
                        return (*accionActual)->PuedeRehacer();
                }
        }
}

bool ControladorAcciones::PuedeDeshacer(GNC::GCS::IVista* pVista, std::string& nombreAccion)
{
        wxCriticalSectionLocker lock((*m_pCriticalSection));
        if(m_mapaAcciones.find(pVista) == m_mapaAcciones.end()) {
                return false;
        } else {
                TListaAcciones& listaAcciones = m_mapaAcciones[pVista]->m_listaAcciones;
                TListaAcciones::iterator& accionActual = m_mapaAcciones[pVista]->m_iteradorAcciones;
                if(accionActual == listaAcciones.end()) {
                        return false;
                } else {
                        nombreAccion = (*accionActual)->GetNombre();
                        return true;
                }
        }
}

void ControladorAcciones::OnVistaDestruida(GNC::GCS::IVista* pVista)
{
        //cuando se destruye una vista se vacia su pila de hacer/deshacer
        wxCriticalSectionLocker lock((*m_pCriticalSection));
        TMapaVista::iterator it = m_mapaAcciones.find(pVista);
        if(it != m_mapaAcciones.end()) {
                delete (*it).second;
                m_mapaAcciones.erase(it);
        }
}
}
}
