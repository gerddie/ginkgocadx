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
#if defined(ENABLE_ZOOMTOOL)
#ifndef ZOOMTOOL_H
#define ZOOMTOOL_H
#include <export/tools/itoolzoom.h>
#include <api/api.h>
#include <widgets/wzoom.h>

namespace GNC
{
class wxToolZoomKeyHandler;
class EXTAPI ToolZoom : public GNC::GCS::IToolZoom
{
public:
        typedef GNC::GCS::Events::EventoRaton       TEventoRaton;
        typedef GNC::GCS::Events::EventoTeclado     TEventoTeclado;
        typedef GNC::GCS::Widgets::WZoomBuilder      TBuilder;
        ToolZoom();
        ~ToolZoom();

        virtual GNC::GCS::ITool* NewTool();
        virtual void Connect();
        virtual void Disconnect();
        virtual bool HasDropDownMenu();
        virtual void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);

        virtual void ZoomOneOne();
        virtual void ConnectKeyEvents(wxWindow* pWindow);

protected:
        friend class wxToolZoomKeyHandler;
        TBuilder*            m_pBuilder;
        wxToolZoomKeyHandler* m_pHandler;
};
}
#endif
#endif
