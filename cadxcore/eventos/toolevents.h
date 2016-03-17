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

class wxBitmap;
namespace GNC
{
namespace GCS
{
class ITool;
class IToolController;
namespace Events
{
class EXTAPI ToolConnectedEvent : public GNC::GCS::Events::IEvent
{
private:
        bool connected;
        GNC::GCS::ITool* pTool;
        GNC::GCS::IToolController* pToolBar;
public:
        ToolConnectedEvent() : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolConnected, 0, 100, NULL)
        {
                connected = false;
                pTool = NULL;
                pToolBar = NULL;
        }

        ToolConnectedEvent(bool connected, GNC::GCS::ITool* pTool, GNC::GCS::IToolController* pToolBar): GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolConnected, 0, 100, NULL)
        {
                this->connected = connected;
                this->pTool = pTool;
                this->pToolBar = pToolBar;
        }

        ~ToolConnectedEvent()
        {
        }

        bool IsConnected()
        {
                return connected;
        }

        GNC::GCS::ITool* GetTool()
        {
                return pTool;
        }

        GNC::GCS::IToolController* GetToolBar()
        {
                return pToolBar;
        }
};

class EXTAPI ToolEnabledEvent : public GNC::GCS::Events::IEvent
{
private:
        bool enabled;
        GNC::GCS::ITool* pTool;
        GNC::GCS::IToolController* pToolBar;
public:
        ToolEnabledEvent() : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolEnabled, 0, 100, NULL)
        {
                enabled = false;
                pTool = NULL;
                pToolBar = NULL;
        }

        ToolEnabledEvent(bool enabled, GNC::GCS::ITool* pTool, GNC::GCS::IToolController* pToolBar): GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolEnabled, 0, 100, NULL)
        {
                this->enabled = enabled;
                this->pTool = pTool;
                this->pToolBar = pToolBar;
        }

        ~ToolEnabledEvent()
        {
        }

        bool IsEnabled()
        {
                return enabled;
        }

        GNC::GCS::ITool* GetTool()
        {
                return pTool;
        }

        GNC::GCS::IToolController* GetToolBar()
        {
                return pToolBar;
        }
};

class EXTAPI ChangeToolIconEvent : public GNC::GCS::Events::IEvent
{
private:
        const wxBitmap* pBitmap;
        GNC::GCS::ITool* pTool;
        GNC::GCS::IToolController* pToolBar;
public:
        ChangeToolIconEvent() : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolIconChanged, 0, 100, NULL)
        {
                pBitmap = NULL;
                pTool = NULL;
                pToolBar = NULL;
        }

        ChangeToolIconEvent(const wxBitmap* bmp, GNC::GCS::ITool* pTool, GNC::GCS::IToolController* pToolBar): GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolIconChanged, 0, 100, NULL),pBitmap(bmp)
        {
                this->pTool = pTool;
                this->pToolBar = pToolBar;
        }

        ~ChangeToolIconEvent()
        {
        }

        const wxBitmap* GetBitmap()
        {
                return pBitmap;
        }

        GNC::GCS::ITool* GetTool()
        {
                return pTool;
        }

        GNC::GCS::IToolController* GetToolBar()
        {
                return pToolBar;
        }
};

}
}
}
