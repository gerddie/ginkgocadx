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
#include <vector>

#include <api/iwidgets.h>
#include <widgets/observers/iobservadorwindowlevel.h>

namespace GNC
{
namespace GCS
{
namespace Widgets
{
//---------------------------------------------------------------------

class WWindowLevelBuilder : public GNC::GCS::Widgets::IWidgetBuilder
{
public:

        typedef GNC::GCS::Vector TVector;
        typedef GNC::GCS::Widgets::Observadores::IObservadorWindowLevel TObservador;

        WWindowLevelBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

        void SetObservador(TObservador* pObservador)
        {
                m_pObservador = pObservador;
        };

        ~WWindowLevelBuilder();

        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

        virtual void Render(GNC::GCS::Contexto3D* c);

        virtual GNC::GCS::Widgets::TipoCursor GetCursor();

protected:
        TObservador*             m_pObservador;
        TVector                  m_NodoMoviendose;  // Cursor
        TVector                  m_Start;           // Origen del trazado de la recta
        bool                     m_MouseDown;
        bool                     m_Changed;
};
};
};
};
