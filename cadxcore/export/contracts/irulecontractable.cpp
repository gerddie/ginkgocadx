/*
 *  
 *  $Id: iRulecontractable.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include "irulecontractable.h"
#include <export/contracts/irulecontract.h>

GNC::GCS::IRuleContractable::IRuleContractable()
{
	RuleContract = NULL;
}

GNC::GCS::IRuleContractable::~IRuleContractable()
{
}

bool GNC::GCS::IRuleContractable::IsValidContract(IContract* contract)
{
	GNC::GCS::IRuleContract* pWContract = dynamic_cast<GNC::GCS::IRuleContract*>(contract);
	return pWContract != NULL;
}

			
void GNC::GCS::IRuleContractable::Register(IContract* contract)
{
	GNC::GCS::IContractable::Register(contract);
	RuleContract = static_cast<GNC::GCS::IRuleContract*>(contract);
}
