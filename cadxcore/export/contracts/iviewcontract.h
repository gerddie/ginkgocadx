#pragma once
/*
*
*  $Id: iviewcontract.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IVIEWCONTRACT_H
#define IVIEWCONTRACT_H

#include <api/icontract.h>
namespace GNC {
	namespace GCS {
		class IVista;

		class EXTAPI IViewContract : public IContract {
		public:
			IViewContract();
			~IViewContract();
			
			virtual GNC::GCS::IVista* GetView() = 0;
		protected:
		};
	}
}
#endif