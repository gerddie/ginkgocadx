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
#include "historypanel3base.h"
#include <wx/ginkgostyle/ginkgostyle.h>

//----------DIALOGO CONFIRMAR ELIMINAR...-------------
namespace GNC
{
namespace GUI
{
class ConfirmDeleteDialog:public ConfirmDeleteDialogBase
{
public:
        ConfirmDeleteDialog(wxWindow* parent, const wxString& label):ConfirmDeleteDialogBase(parent)
        {
                m_pLabelEliminar->SetLabel(m_pLabelEliminar->GetLabel() + label + _(" from History?"));
                m_pTitled->Layout();
                m_body->Layout();
                Layout();
        };

        ~ConfirmDeleteDialog()
        {
        };

        void OnEliminarClick(wxCommandEvent &)
        {
                EndModal(wxOK);
        }

        void OnCancelarClick(wxCommandEvent &)
        {
                EndModal(wxCANCEL);
        }

protected:
};
};
};
