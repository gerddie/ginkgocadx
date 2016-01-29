/*
 *  
 *  $Id: iwindowlevelcontractable.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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
