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
#include <wx/wizardginkgo/wxwizardginkgobase.h>
#include <api/iwizard.h>
#include "datospersistentesexportacion.h"
#include <api/ientorno.h>
#include <string>
#include <vector>

namespace GNC
{
namespace GCS
{
class WidgetsContract;
}
namespace GUI
{
class wxWidzardExportacionGinkgo: public wxWizardGinkgoBase
{

public:
        wxWidzardExportacionGinkgo(wxWindow* pParent, GNC::GCS::IWidgetsContract* WidgetsContract);
        ~wxWidzardExportacionGinkgo();

        virtual void OnCancelarClick(wxCommandEvent& event);
        virtual void OnSiguienteClick(wxCommandEvent& event);
        virtual void OnAnteriorClick(wxCommandEvent &event);

        void CargarCurrent();

        bool IsOK();


protected:
        typedef std::list<IPasoWizard*> ListaPasos;
        ListaPasos m_ListaPasos;

        ListaPasos::size_type m_currentPasoIndex;
        bool            m_OK;

};
};
};
