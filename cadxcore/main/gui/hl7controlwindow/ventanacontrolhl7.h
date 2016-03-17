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

#include "ventanacontrolhl7base.h"


class wxSQLite3Database;
namespace GNC
{
namespace GUI
{
class TimerControlHL7;
class VentanaControlHL7 : public VentanaControlHL7Base
{
public:
        //singleton
        static VentanaControlHL7* Instance();
        static void CerrarSiAbierta();
        void Refrescar();

protected:
        static VentanaControlHL7*  m_pInstance;
        TimerControlHL7* m_pTimerRefresh;

        VentanaControlHL7();
        ~VentanaControlHL7();

        void OnActivate(wxActivateEvent &event);
        void OnMensajeMenu(wxDataViewEvent& event);
        void OnMensajeDClick(wxDataViewEvent& event);
        void OnMostrarMensaje(wxCommandEvent &event);
        void OnEditarMensaje(wxCommandEvent &event);
        void OnListaKey(wxKeyEvent &event);
        void OnEliminarMensaje(wxCommandEvent &event);
        void OnPurgarMensajes(wxCommandEvent& event);
        void OnActualizar(wxCommandEvent &event);
        void OnArrancarPararClick(wxCommandEvent &event);
        void RefrescarMensajes();
        void ActualizarEstadoControlador();
        void MostrarVentanaMensaje();
        void EliminarMensaje();

        virtual void OnMenuSalir( wxCommandEvent& event );
        virtual void OnKeyDown( wxKeyEvent& event );

        //endregion

};
};
};
