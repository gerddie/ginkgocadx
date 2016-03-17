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
#include "calibracionbase.h"
#include <widgets/wrule.h>

namespace GNC
{
namespace GCS
{
namespace Widgets
{
namespace Dialogos
{
class Calibracion : public CalibracionBase
{
public:
        typedef GNC::GCS::Widgets::WRule TWidget;
        typedef GNC::GCS::IWidgetsManager TManager;

        Calibracion(wxWindow* parent, TWidget* pWidget, TManager* pManager);
        ~Calibracion();

        virtual void OnClickEnMenu(wxCommandEvent& event);

protected:
        virtual void OnDialogoKeyDown( wxKeyEvent& event );

        virtual void OnControlDistanciaKeyDown( wxKeyEvent& event );

        virtual void OnBotonCancelarClick( wxCommandEvent& event );

        virtual void OnBotonAceptarClick( wxCommandEvent& event );

        virtual void OnActivate(wxActivateEvent& event);

        bool Calibrar();

        bool ValidarDouble(wxString strNumero, double min, double max);
        bool ValidarDouble(double valor, double min, double max);

        TWidget*   m_pWidget;
        TManager*  m_pManager;
};
}
}
}
}
