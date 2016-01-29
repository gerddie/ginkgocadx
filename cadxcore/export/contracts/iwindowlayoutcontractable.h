#pragma once
/*
 *  
 *  $Id: iwindowlayoutcontractable.h $
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
		class IWindowLayoutContract;
		class EXTAPI IWindowLayoutContractable: public GNC::GCS::IContractable
		{
		public:
			IWindowLayoutContractable();
			virtual ~IWindowLayoutContractable();

			virtual bool IsValidContract(IContract* contract);

			
			virtual void Register(IContract* contract);
		protected:
			GNC::GCS::IWindowLayoutContract* WindowLayoutContract;
		};
	}
}
