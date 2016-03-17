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

#include "enabled_tools.h"
#if defined(ENABLE_DETACHWINDOWTOOL)
#ifndef DETACHWINDOWTOOL_H
#define DETACHWINDOWTOOL_H
#include <export/tools/idetachwindowtool.h>
#include <api/api.h>

#include <map>


namespace GNC
{

class EXTAPI DetachWindowTool : public GNC::GCS::IDetachWindowTool
{

protected:

public:
        virtual GNC::GCS::ITool* NewTool();
        DetachWindowTool();
        ~DetachWindowTool();
        virtual bool ExecuteAction();
        virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
        bool IsDetached();
        void doDetach();

protected:

};
}
#endif
#endif
