/*
 *  
 *  $Id: iwindowlayoutcontractable.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include "iwindowlayoutcontractable.h"
#include <export/contracts/iwindowlayoutcontract.h>

GNC::GCS::IWindowLayoutContractable::IWindowLayoutContractable()
{
	WindowLayoutContract = NULL;
}
GNC::GCS::IWindowLayoutContractable::~IWindowLayoutContractable()
{
}

bool GNC::GCS::IWindowLayoutContractable::IsValidContract(IContract* contract)
{
	GNC::GCS::IWindowLayoutContract* pWContract = dynamic_cast<GNC::GCS::IWindowLayoutContract*>(contract);
	return pWContract != NULL;
}

void GNC::GCS::IWindowLayoutContractable::Register(IContract* contract)
{
	GNC::GCS::IContractable::Register(contract);
	WindowLayoutContract = static_cast<GNC::GCS::IWindowLayoutContract*>(contract);
}
