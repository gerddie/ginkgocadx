/*
 *  
 *  $Id: iviewcontractable.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include "iviewcontractable.h"
#include <export/contracts/iviewcontract.h>
GNC::GCS::IViewContractable::IViewContractable()
{
	ViewContract = NULL;
}
GNC::GCS::IViewContractable::~IViewContractable()
{
}

bool GNC::GCS::IViewContractable::IsValidContract(IContract* contract)
{
	GNC::GCS::IViewContract* pWContract = dynamic_cast<GNC::GCS::IViewContract*>(contract);
	return pWContract != NULL;
}

void GNC::GCS::IViewContractable::Register(IContract* contract)
{
	GNC::GCS::IContractable::Register(contract);
	ViewContract = static_cast<GNC::GCS::IViewContract*>(contract);
}
