#pragma once
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


#ifndef ECGRULETOOL_H
#define ECGRULETOOL_H
#include <visualizator/export/tools/iecgruletool.h>
#include <api/api.h>

namespace GNKVisualizator
{
namespace Widgets
{
class WECGRuleBuilder;
}

class EXTAPI ECGRuleTool : public GNKVisualizator::IECGRuleTool
{
public:
        ECGRuleTool();
        ~ECGRuleTool();


//------------------------------------------------------------------------------
//region "ITool/IContract realization"

        virtual GNC::GCS::ITool* NewTool();
        virtual void Connect();
        virtual void Disconnect();

//endregion

protected:
        GNKVisualizator::Widgets::WECGRuleBuilder* m_pRuleBuilder;
};
}
#endif
