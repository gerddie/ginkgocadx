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

#include "ipixelsbluringcontractable.h"
#include <export/contracts/ipixelsbluringcontract.h>

GNC::GCS::IPixelsBluringContractable::IPixelsBluringContractable()
{
	PixelsBluringContract = NULL;
}

GNC::GCS::IPixelsBluringContractable::~IPixelsBluringContractable()
{
}

bool GNC::GCS::IPixelsBluringContractable::IsValidContract(IContract* contract)
{
	GNC::GCS::IPixelsBluringContract* pWContract = dynamic_cast<GNC::GCS::IPixelsBluringContract*>(contract);
	return pWContract != NULL;
}

			
void GNC::GCS::IPixelsBluringContractable::Register(IContract* contract)
{
	GNC::GCS::IContractable::Register(contract);
	PixelsBluringContract = static_cast<GNC::GCS::IPixelsBluringContract*>(contract);
}
