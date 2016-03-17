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
#include "panelesconfiguracionginkgobase.h"

#include <api/iconfiguracion.h>
#include <api/controllers/imodulecontroller.h>
#include <api/ientorno.h>

#include <string>
#include <vector>

namespace GNC
{
namespace GUI
{

class DialogoConfiguracion: public IDialogoConfiguracion, public DialogoConfiguracionBase
{
public:
        DialogoConfiguracion(wxWindow* pParent);
        ~DialogoConfiguracion();

        virtual void OnCambioNodoNavegacion(wxTreeEvent &event);

        void CargarCurrent();

        void OnPropiedadCambiada();

        void OnCancelarClick(wxCommandEvent &event);

        void OnAceptarClick(wxCommandEvent &event);

        void OnAplicarClick(wxCommandEvent &event);

        void OnExportarClick(wxCommandEvent &event);

        void OnImportarClick(wxCommandEvent &event);

        typedef std::map<wxTreeItemIdValue ,IPasoConfiguracion*> MapaPasos;

        MapaPasos m_MapaPasos;

        IPasoConfiguracion* m_currentPaso;

};
}
}
