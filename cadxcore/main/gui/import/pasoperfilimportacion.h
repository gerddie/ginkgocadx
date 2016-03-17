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

#include "pasosimportacionbase.h"

#include <api/iwizard.h>
#include <api/controllers/ipacscontroller.h>
#include <api/ientorno.h>

#include <string>
#include <vector>

namespace GNC
{

namespace GCS
{
class IEntorno;
class IModuleController;
}

namespace GUI
{
class ImportProfileToolBar;

class PasoPefilImportacion: public PasoPerfilImportacionBase, public IPasoWizard
{
public:
        PasoPefilImportacion(wxWindow* pParent,IWizard* pWizard);
        ~PasoPefilImportacion();

        GNC::GCS::IModuleController* GetControladorSeleccionado()
        {
                return m_pControladorModulo;
        };

protected:
        GNC::GCS::IModuleController* m_pControladorModulo;
        ImportProfileToolBar* m_pToolBar;

        //region "Metodos heredados de IPasoWizard"
        virtual std::string GetTitle();

        virtual std::string GetSubTitle();

        virtual bool Siguiente();

        virtual bool Anterior();

        virtual bool Cancelar();

        virtual bool Validar();
        //endregion
};
}
}
