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
#include "enabled_tools.h"
#if defined(ENABLE_MARKERTOOL)
#ifndef MARKERTOOL_H
#define MARKERTOOL_H
#include <api/api.h>
#include <export/tools/imarkertool.h>


namespace GNC
{

namespace GCS
{
namespace Widgets
{
class IWidgetBuilder;
}
}

class EXTAPI MarkerTool : public GNC::GCS::IMarkerTool
{
public:
        typedef enum TipoMarcado {
                TW_Ninguno,
                TW_Flecha,
                TW_Elipse,
                TW_Punto,
                TW_Trapezoide,
                TW_FreeForm,
                TW_Polygon
        } TipoMarcado;

        MarkerTool();
        ~MarkerTool();

//region "ITool/IContract realization"
        virtual GNC::GCS::ITool* NewTool();
        virtual void Connect();
        virtual void Disconnect();

        virtual bool HasDropDownMenu();
        void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
//endregion
        //region "Eventos refinados desde los paneles"

        virtual void SetTipoMarcado(TipoMarcado tipo);

        //endregion

protected:
        GNC::GCS::Widgets::IWidgetBuilder*                 m_pBuilder;
        TipoMarcado                                        m_MarcadoActivo;
};
}
#endif
#endif
