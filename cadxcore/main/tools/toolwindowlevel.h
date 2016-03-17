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
#if defined(ENABLE_WINDOWLEVELTOOL)
#ifndef WINDOWLEVELTOOL_H
#define WINDOWLEVELTOOL_H
#include <export/tools/itoolwindowlevel.h>
#include <api/controllers/ieventscontroller.h>
#include <api/api.h>
#include <widgets/observers/iobservadorwindowlevel.h>
#include <export/contracts/iwindowlevelcontract.h>

#include <map>

namespace GNC
{
namespace GCS
{
namespace Widgets
{
class WWindowLevelBuilder;
}
}

class EXTAPI ToolWindowLevel : public GNC::GCS::IToolWindowLevel, public GNC::GCS::Widgets::Observadores::IObservadorWindowLevel
{

public:
        typedef GNC::GCS::Widgets::WWindowLevelBuilder TBuilder;
        typedef GNC::GCS::IWindowLevelContract TWindowLevelContract;

        ToolWindowLevel();
        virtual ~ToolWindowLevel();

        //region "ITool/IContract realization"

        virtual GNC::GCS::ITool* NewTool();
        virtual void Connect();
        virtual void Disconnect();

        virtual bool HasDropDownMenu();
        void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
        //endregion

        //region "Realización de la interfaz IHerramienta"
        virtual void OnStart();
        virtual void OnAjuste(TVector& start, TVector& stop);
        virtual void OnEnd();
        //endregion

        //region "metodos propios de la herramienta window/level"
        virtual void ClampWindowLevel(double window, double level, double& clampedWindow, double& clampedLevel);

        void SetWindowLevel(const std::string& label);
        virtual void SetWindowLevel(double window, double level);
        void Resetear();
        void AutoCalcule();
        //endregion

protected:
        TBuilder*  m_pWindowLevelBuilder;
        GNC::GCS::IWindowLevelContract::WindowLevel m_WindowLevelInicial;

};
}
#endif
#endif
