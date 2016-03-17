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

#include <api/ievento.h>
#include "eventosginkgo.h"

namespace GNC
{
namespace GCS
{

namespace Widgets
{
class IWidget;
}

namespace Events
{

class EXTAPI EventoModificacionWidget : public GNC::GCS::Events::IEvent
{
public:

        typedef enum {
                WidgetCreado,
                WidgetModificado,
                WidgetDestruido
        } TipoModificacion;

        EventoModificacionWidget(GNC::GCS::IVista* pVista = NULL, TipoModificacion tipo = WidgetModificado, GNC::GCS::Widgets::IWidget* w = NULL) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ModificacionWidget,0,100,pVista)
        {
                m_Nombre = "ModificacionWidget";
                m_pWidget = w;
                m_Tipo = tipo;
        }

        ~EventoModificacionWidget()
        {
        }

        virtual void pushInfo(std::ostream& out) const
        {
                out << "Tipo = ";
                switch (m_Tipo) {
                case WidgetCreado:
                        out << "WidgetCreado";
                        break;
                case WidgetModificado:
                        out << "WidgetModificado";
                        break;
                case WidgetDestruido:
                        out << "WidgetDestruido";
                        break;
                }
        }

        GNC::GCS::Widgets::IWidget* GetWidget()
        {
                return m_pWidget;
        }

        TipoModificacion GetTipo()
        {
                return m_Tipo;
        }

protected:
        TipoModificacion            m_Tipo;
        GNC::GCS::Widgets::IWidget* m_pWidget;
};
}
}
}
