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
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/log4cplus/logdefines.h>
#include <wx/event.h>
#include "ventanacontrollogsbase.h"


namespace GNC
{
namespace GUI
{

class VentanaControlLogs : public VentanaControlLogsBase, public GNC::GCS::Logging::ILogger
{
public:
        //singleton
        static VentanaControlLogs* Instance();
        static void CerrarSiAbierta();
        void Refrescar();

        //region Interfaz ILogger
        virtual void Append(const GNC::GCS::Logging::LogEvent& le);

        //endregion

protected:
        static VentanaControlLogs*  m_pInstance;
        GNC::GCS::IControladorLog::MapaLogLevels m_MapaLogLevels;
        bool m_needRefresh;

        VentanaControlLogs();
        ~VentanaControlLogs();

        virtual void OnClose(wxCloseEvent& event);

        virtual void OnInternalIdle();

        virtual void OnLimpiarClick( wxCommandEvent& /*event*/ )
        {
                LimpiarRegistros();
        }
        virtual void OnLimpiarRegistros( wxCommandEvent& /*event*/ )
        {
                LimpiarRegistros();
        }

        virtual void OnGuardarRegistros( wxCommandEvent& event );
        virtual void OnNivelChoice( wxCommandEvent& event );

        virtual void OnSalirClick( wxCommandEvent& event );
        virtual void OnKeyDown( wxKeyEvent& event );
        void LimpiarRegistros();

        wxArrayString pendingLines;

};
}
}
