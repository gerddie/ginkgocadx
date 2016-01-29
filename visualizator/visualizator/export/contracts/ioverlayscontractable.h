#pragma once
/*
 *  
 *  $Id: iviewcontractable.h 5037 2012-07-20 12:54:39Z tovar $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include <api/icontract.h>

namespace GNKVisualizator {
	class IOverlaysContract;
	class IOverlaysContractable: public GNC::GCS::IContractable
	{
	public:
		IOverlaysContractable();

		~IOverlaysContractable();

		virtual bool IsValidContract(GNC::GCS::IContract* contract);

		
		virtual void Register(GNC::GCS::IContract* contract);
	protected:
		GNKVisualizator::IOverlaysContract* OverlaysContract;
	};
}