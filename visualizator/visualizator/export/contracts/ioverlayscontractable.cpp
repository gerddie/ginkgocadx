/*
 *  
 *  $Id: iviewcontractable.cpp 5037 2012-07-20 12:54:39Z tovar $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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
