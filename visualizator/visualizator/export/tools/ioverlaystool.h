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

#ifndef IOVERLAYSTOOL_H
#define IOVERLAYSTOOL_H
#include <visualizator/export/contracts/ioverlayscontractable.h>
#include <api/toolsystem/itool.h>
#include "ivisualizatortools.h"

namespace GNKVisualizator
{
class IOverlaysTool:  public GNC::GCS::ITool, public GNKVisualizator::IOverlaysContractable
{
public:
        static GNC::GCS::ITool::UID ID;
        IOverlaysTool();
};
}
#endif
