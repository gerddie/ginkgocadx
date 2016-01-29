#pragma once
/*
*
*  $Id: irulecontract.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IRULECONTRACT_H
#define IRULECONTRACT_H

#include <list>

#include <api/api.h>
#include <export/contracts/iwidgetscontract.h>

namespace GNC {
	namespace GCS {

		class EXTAPI IRuleContract : public IWidgetsContract {
		public:

			IRuleContract();

			~IRuleContract();
			
			virtual bool SupportsCallibration() = 0;
		};		
	}
}

#endif