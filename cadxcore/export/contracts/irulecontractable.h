#pragma once
/*
 *  
 *  $Id: irulecontractable.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include <api/icontract.h>

namespace GNC {
	namespace GCS {
		class IRuleContract;
		class EXTAPI IRuleContractable: public GNC::GCS::IContractable
		{
		public:
			IRuleContractable();
			virtual ~IRuleContractable();

			virtual bool IsValidContract(IContract* contract);
			
			virtual void Register(IContract* contract);
		protected:
			GNC::GCS::IRuleContract* RuleContract;
		};
	}
}
