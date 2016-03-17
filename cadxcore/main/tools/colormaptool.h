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
#ifndef COLORMAPTOOL_H
#define COLORMAPTOOL_H

#include <export/tools/icolormaptool.h>
#include <api/api.h>

namespace GNC
{

class ColorMapTool : public GNC::GCS::IColorMapTool
{
public:
        virtual GNC::GCS::ITool* NewTool();
        ColorMapTool();
        ~ColorMapTool();

        virtual bool HasDropDownMenu();
        void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);

        virtual bool ExecuteAction();
        virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

        //cuando hacen click en ella...
        virtual bool SoportaMapas();
        virtual void SeleccionarMapa(int id);

        int GetIdLookupTable();
        virtual void InvertColorMap();

protected:

};
}
#endif
