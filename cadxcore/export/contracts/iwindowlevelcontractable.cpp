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


#include "iwindowlevelcontractable.h"
#include <export/contracts/iwindowlevelcontract.h>

GNC::GCS::IWindowLevelContractable::IWindowLevelContractable()
{
        WindowLevelContract = NULL;
}

GNC::GCS::IWindowLevelContractable::~IWindowLevelContractable()
{
}

bool GNC::GCS::IWindowLevelContractable::IsValidContract(IContract* contract)
{
        GNC::GCS::IWindowLevelContract* pWContract = dynamic_cast<GNC::GCS::IWindowLevelContract*>(contract);
        return pWContract != NULL;
}


void GNC::GCS::IWindowLevelContractable::Register(IContract* contract)
{
        GNC::GCS::IContractable::Register(contract);
        WindowLevelContract = static_cast<GNC::GCS::IWindowLevelContract*>(contract);
}
