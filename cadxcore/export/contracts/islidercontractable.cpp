/*
 *  
 *  $Id: islidercontractable.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include "islidercontractable.h"
#include <export/contracts/islidercontract.h>

GNC::GCS::ISliderContractable::ISliderContractable()
{
	SliderContract = NULL;
}

GNC::GCS::ISliderContractable::~ISliderContractable()
{
}

bool GNC::GCS::ISliderContractable::IsValidContract(IContract* contract)
{
	GNC::GCS::ISliderContract* pWContract = dynamic_cast<GNC::GCS::ISliderContract*>(contract);
	return pWContract != NULL;
}

			
void GNC::GCS::ISliderContractable::Register(IContract* contract)
{
	GNC::GCS::IContractable::Register(contract);
	SliderContract = static_cast<GNC::GCS::ISliderContract*>(contract);
}
