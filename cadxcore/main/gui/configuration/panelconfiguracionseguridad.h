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
#include <api/iconfiguracion.h>
#include "panelesconfiguracionginkgobase.h"
#include <string>
#include <vector>
#include <api/autoptr.h>
#include <wx/wxsqlite3/wxsqlite3.h>

namespace GNC
{
namespace GUI
{

class PanelConfiguracionSeguridad: public PanelConfiguracionSeguridadBase, public IPasoConfiguracion
{
public:
        PanelConfiguracionSeguridad(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
        ~PanelConfiguracionSeguridad();

        void Recargar();

        //region "Metodos de IPasoConfiguracion"
        wxWindow* GetPanel();

        std::string GetTitle();

        std::string GetCabecera();

        bool Validar();

        bool Guardar();
        //endregion


protected:
        virtual void OnSecTypeChoice(wxCommandEvent &event);

        virtual void OnListaUsuariosItemSelected(wxListEvent &event);

        virtual void OnListaUsuariosItemDeSelected(wxListEvent &event);

        virtual void OnNuevoClick(wxCommandEvent &event);

        virtual void OnCambiarPasswordClick(wxCommandEvent &event);

        virtual void OnEliminarClick(wxCommandEvent & event);

        virtual void OnSetMainPassword(wxCommandEvent &event);

        wxImageList* m_pImageList;
        GNC::GCS::Ptr<wxSQLite3Database> m_pSesionControlador;

};
}
}
