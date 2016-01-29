/*
 *  
 *  $Id: iwidgetscontractable.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include "iwidgetscontractable.h"
#include <export/contracts/iwidgetscontract.h>

GNC::GCS::IWidgetsContractable::IWidgetsContractable()
{
	WidgetsContract = NULL;
}

GNC::GCS::IWidgetsContractable::~IWidgetsContractable()
{
}

bool GNC::GCS::IWidgetsContractable::IsValidContract(IContract* contract)
{
	GNC::GCS::IWidgetsContract* pWContract = dynamic_cast<GNC::GCS::IWidgetsContract*>(contract);
	return pWContract != NULL;
}

			
void GNC::GCS::IWidgetsContractable::Register(IContract* contract)
{
	GNC::GCS::IContractable::Register(contract);
	WidgetsContract = static_cast<GNC::GCS::IWidgetsContract*>(contract);
}
