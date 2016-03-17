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


#include "ioverlayscontractable.h"
#include <visualizator/export/contracts/ioverlayscontract.h>
GNKVisualizator::IOverlaysContractable::IOverlaysContractable()
{
        OverlaysContract = NULL;
}
GNKVisualizator::IOverlaysContractable::~IOverlaysContractable()
{
}

bool GNKVisualizator::IOverlaysContractable::IsValidContract(GNC::GCS::IContract* contract)
{
        GNKVisualizator::IOverlaysContract* pWContract = dynamic_cast<GNKVisualizator::IOverlaysContract*>(contract);
        return pWContract != NULL;
}

void GNKVisualizator::IOverlaysContractable::Register(GNC::GCS::IContract* contract)
{
        GNC::GCS::IContractable::Register(contract);
        OverlaysContract = static_cast<GNKVisualizator::IOverlaysContract*>(contract);
}
