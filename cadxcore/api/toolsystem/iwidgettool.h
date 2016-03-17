#pragma once
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

#ifndef IWIDGETTOOL_H
#define IWIDGETTOOL_H

#include <api/toolsystem/itool.h>

namespace GNC
{

namespace GCS
{
class EXTAPI IWidgetTool: public GNC::GCS::ITool
{
public:
        virtual ~IWidgetTool();

        //new tool
        IWidgetTool(const ITool::UID& uid, const ITool::TToolFamily& familyUID, const std::string& name, const wxBitmap& icon, const int subFamily = 0xFFFFFFFF, const int& priority = 0);

        virtual bool AppendsInMenu()
        {
                return false;
        }
        virtual bool ExecuteAction();

        virtual void Connect() = 0;
        virtual void Disconnect() = 0;

        virtual void AppendToolIn(wxGinkgoToolBar* pParent);

        TriggerButton& GetTriggerButton()
        {
                return m_TriggerButtonMask;
        }

        TriggerButton m_TriggerButtonMask;
        //

};
}
}
#endif
