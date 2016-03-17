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
#include <vector>
#include <list>
#include <map>

#include <api/ievento.h>
#include <api/autoptr.h>
#include <eventos/eventosginkgo.h>

class wxWindow;
namespace GNC
{
namespace GCS
{
class IVista;
}
namespace GUI
{
//nodo seleccionable
class ISeleccionablePreview
{
protected:
        bool m_seleccionado;
public:
        ISeleccionablePreview()
        {
                m_seleccionado = false;
        }
        ~ISeleccionablePreview()
        {
        }
        bool EstaSeleccionado()
        {
                return m_seleccionado;
        }
        virtual void Seleccionar(bool seleccionar, bool force=false) = 0;
};

class INodoHistorial
{
public:
        typedef std::list<GNC::GUI::INodoHistorial*> TChildrenList;
        INodoHistorial(INodoHistorial* pPadre)
        {
                m_pPadre = pPadre;
        }
        ~INodoHistorial()
        {
                m_pPadre = NULL;
        }
        INodoHistorial* GetParentNode()
        {
                return m_pPadre;
        }

        virtual void GetNodeSize(int & /*x*/, int &/*y*/) {};
        virtual wxWindow* GetWxWindow() = 0;

protected:
        INodoHistorial* m_pPadre;
};

//eventos variados...
//evento de seleccion
namespace Events
{
class EventoSeleccionarHistorial: public GNC::GCS::Events::IEvent
{
public:
        EventoSeleccionarHistorial(GNC::GCS::IVista* pView):GNC::GCS::Events::IEvent(ginkgoEVT_Core_HistorialSeleccionar, 0, 100, pView)
        {
                m_Nombre = "SeleccionarHistorial";
                m_pSeleccionable=NULL;
        }

        EventoSeleccionarHistorial(GNC::GCS::IVista* pView, ISeleccionablePreview* pSeleccionable ):GNC::GCS::Events::IEvent(ginkgoEVT_Core_HistorialSeleccionar, 0 , 100, pView)
        {
                m_pSeleccionable = pSeleccionable;
        }

        ~EventoSeleccionarHistorial()
        {
        }

        ISeleccionablePreview* GetSeleccionable()
        {
                return m_pSeleccionable;
        }

protected:
        ISeleccionablePreview* m_pSeleccionable;
};

class EventoSetFocusHistorial: public GNC::GCS::Events::IEvent
{
public:

        EventoSetFocusHistorial(GNC::GCS::IVista* pView):GNC::GCS::Events::IEvent(ginkgoEVT_Core_SetFocus, 0, 100, pView)
        {
                m_Nombre = "SetFocusHistorial";
        }

        ~EventoSetFocusHistorial()
        {
        }
protected:
};

}//eventos
}
}
