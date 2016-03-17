/*
 *  Ginkgo CADx Project
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
#include "selecciontextobase.h"
#include "../wtextbox.h"

namespace GNC
{
namespace GCS
{
namespace Widgets
{
namespace Dialogos
{
class SeleccionTexto : public SeleccionTextoBase
{
public:
        typedef GNC::GCS::Widgets::WTextBox TWidget;
        typedef GNC::GCS::IWidgetsManager TManager;

        SeleccionTexto(wxWindow* parent, TWidget* pWidget, TManager* pManager);
        ~SeleccionTexto();

        virtual void OnClickEnMenu(wxCommandEvent& event);

protected:

        virtual void OnDialogoKeyDown( wxKeyEvent& event );

        virtual void OnControlTextoKeyDown( wxKeyEvent& event );

        virtual void OnControlTextoTextoActualizado( wxCommandEvent& event );

        virtual void OnBotonCancelarClick( wxCommandEvent& event );

        virtual void OnBotonAceptarClick( wxCommandEvent& event );

        virtual void OnActivar(wxActivateEvent &event);

        void Actualizar(bool actualizarWidget = true);

        TWidget*   m_pWidget;
        TManager*  m_pManager;
};
}
}
}
}
