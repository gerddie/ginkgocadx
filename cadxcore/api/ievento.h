#pragma once
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

#ifndef IEVENT_H
#define IEVENT_H

#include <ostream>
#include <string>
#include <list>
#include <limits>
#include <api/api.h>
#include <api/ibutton.h>
#include <api/math/geometry.h>
#include <api/math/geometry3d.h>

#define ginkgoAmbitoGeneral  0

//----------------------------------------------------------------------------------------------------
//region "Forward Declarations"
class wxMouseEvent;
class wxKeyEvent;

namespace GNC
{
namespace GCS
{
class Contexto3D;
class IVista;
}
}
//endregion

namespace GNC
{
namespace GCS
{

namespace Events
{

//====================================================================================================
//= Interfaz de evento
//====================================================================================================
class EXTAPI IEvent
{
public:
        //codigo de evento es obligatorio y unico para cada evento
        //el codigo ambito sirve para jerarquizar los eventos entre eventos de guardado, de widgets...
        //prioridad: 0 es la maxima prioridad, se procesaran antes los eventos cuya prioridad sea menor
        //sincronoInterfaz: el procesamiento del evento ha de ser sincrono con la interfaz?
        //vista: vista a la que esta asociado, NULL es la vista general
        IEvent(long codigoEvento, long codigoAmbito = ginkgoAmbitoGeneral, long prioridad = 100, GNC::GCS::IVista* pVista = NULL);

        virtual ~IEvent();

        virtual bool IsEqual(const IEvent& evt) const;

        virtual void pushInfo(std::ostream& ) const;

        bool IsSkipped() const;

        void Skip(bool skip = true);

        long GetCodigoEvento() const;

        long GetCodigoAmbito() const;

        void SetAmbito(long codigoAmbito);

        GNC::GCS::IVista* GetVista() const;

        void SetVista(GNC::GCS::IVista* pVista);

        long GetPrioridad() const;

        void SetPrioridad(long prioridad);

        void SetNombre(const char* nombre);

        void SetNombre(std::string& nombre);

        const std::string& GetNombre() const;

        inline friend std::ostream& operator<<(std::ostream& out, const IEvent& e)
        {
                out <<
                    "[ nombre = " << e.m_Nombre.c_str() <<
                    ", codigo = " << e.m_codigoEvento <<
                    ", ambito = " << e.m_codigoAmbito <<
                    ", prio = "   << e.m_prioridad <<
                    ", vista =  " << e.m_pVista <<
                    ", [ ";
                e.pushInfo(out);
                out << " ] ]";
                return out;
        }

        inline friend std::ostream& operator<<(std::ostream& out, const IEvent* e)
        {
                if (e == NULL) {
                        out << "[ NULL ]";
                } else {
                        out << *e;
                }
                return out;
        }

protected:
        long              m_codigoEvento;
        long              m_codigoAmbito;
        long              m_prioridad;
        bool              m_skip;
        GNC::GCS::IVista* m_pVista;
        std::string       m_Nombre;
};

//====================================================================================================
//= Interfaz de evento de raton
//====================================================================================================
class EXTAPI EventoRaton
{

public:

        typedef enum Boton {
                EB_CUALQUIERA,
                EB_IZQUIERDO,
                EB_DERECHO,
                EB_CENTRAL
        } Boton;

        typedef enum Tipo {
                ET_UNKNOWN,
                ET_ENTER,
                ET_LEAVE,
                ET_MOTION,
                ET_DOWN,
                ET_UP,
                ET_DCLICK
        } Tipo;

public:

        EventoRaton(wxMouseEvent* evt, Contexto3D* pContexto);

        ~EventoRaton();

        bool ButtonDown(Boton but = EB_CUALQUIERA) const;

        bool ButtonDown(const GNC::GCS::TriggerButton& button) const;

        bool ButtonDClick(Boton but = EB_CUALQUIERA) const;

        bool ButtonDClick(const GNC::GCS::TriggerButton& button) const;

        bool ButtonUp(Boton but = EB_CUALQUIERA) const;

        bool ButtonUp(const GNC::GCS::TriggerButton& button) const;

        bool Button(Boton but) const;

        bool Button(const GNC::GCS::TriggerButton& button) const;

        bool ButtonIsDown(Boton but) const;

        bool ButtonIsDown(const GNC::GCS::TriggerButton& button) const;

        bool ControlDown() const;
        bool MetaDown() const;
        bool AltDown() const;
        bool ShiftDown() const;

        bool LeftDown() const;
        bool MiddleDown() const;
        bool RightDown() const;

        bool LeftUp() const;
        bool MiddleUp() const;
        bool RightUp() const;

        bool LeftDClick() const;
        bool MiddleDClick() const;
        bool RightDClick() const;

        bool LeftIsDown() const;
        bool MiddleIsDown() const;
        bool RightIsDown() const;
        bool AnyIsDown() const;


        bool Dragging() const;

        bool Moving() const;

        bool Entering() const;
        bool Leaving() const;
        int GetWheelRotation() const;

        int GetWheelDelta() const;

        int GetLinesPerAction() const;

        bool IsPageScroll() const;

        void Skip(bool skip = true);

        bool GetSkipped() const;

        void ParseWXEvent(wxMouseEvent* evt);

public:

        GNC::GCS::Vector3D wP;
        GNC::GCS::Vector iP;

        int sX;
        int sY;

        Contexto3D* c;


        bool   m_left;
        bool   m_middle;
        bool   m_right;
        bool   m_any;

        bool   m_leftDown;
        bool   m_middleDown;
        bool   m_rightDown;
        bool   m_anyDown;

        Tipo   m_tipo;

        bool   m_controlDown;
        bool   m_shiftDown;
        bool   m_altDown;
        bool   m_metaDown;

        int    m_wheelRotation;
        int    m_wheelDelta;
        int    m_linesPerAction;
        bool   m_skip;
};

//====================================================================================================
//= Interfaz de evento de teclado
//====================================================================================================
class EXTAPI EventoTeclado
{

public:
        Contexto3D* c;

public:

        EventoTeclado(wxKeyEvent* evt, Contexto3D* pContexto);

        ~EventoTeclado();

        bool ControlDown() const;
        bool ShiftDown() const;
        bool MetaDown() const;
        bool AltDown() const;

        bool HasModifiers() const;

        int GetKeyCode() const;

        wchar_t GetUnicodeKey() const;

        unsigned int GetRawKeyCode() const;

        unsigned int GetRawKeyFlags() const;

        void Skip(bool skip = true);

        bool GetSkipped() const;

        void ParseWXEvent(wxKeyEvent* evt);

public:
        bool			  m_isKeyDown;

        long          m_keyCode;

        bool          m_controlDown;
        bool          m_shiftDown;
        bool          m_altDown;
        bool          m_metaDown;

        wchar_t       m_uniChar;

        unsigned int  m_rawCode;
        unsigned int  m_rawFlags;

        bool          m_skip;

};

//====================================================================================================
//= Interfaz de subscriptor de ventos
//====================================================================================================
class EXTAPI ISubscriptorEventos
{
public:
        ISubscriptorEventos();
        virtual ~ISubscriptorEventos();

        // Propagacion de eventos de raton
        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

        // Propagacion de eventos de teclado
        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);
};

//====================================================================================================
//= Interfaz de publicacion de eventos
//====================================================================================================
class EXTAPI IPublicadorEventos
{
public:
        typedef std::list<GNC::GCS::Events::ISubscriptorEventos*> ListaListeners;
        typedef ListaListeners::iterator IteradorListaListeners;

        IPublicadorEventos();

        ~IPublicadorEventos();

        void InsertarListener(GNC::GCS::Events::ISubscriptorEventos* pListener);

        void EliminarListener(GNC::GCS::Events::ISubscriptorEventos* pListener);

protected:

        void PropagarEvento(GNC::GCS::Events::EventoRaton& evt);

        void PropagarEvento(GNC::GCS::Events::EventoTeclado& evt);

protected:
        ListaListeners m_Listeners;
};

}
}
}
#endif
