#pragma once
/*
*
*  $Id: islidercontract.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef ISLIDERCONTRACT_H
#define ISLIDERCONTRACT_H

#include <list>

#include <api/api.h>
#include <export/contracts/iwidgetscontract.h>

namespace GNC {
	namespace GCS {

		class EXTAPI ISliderContract : public IWidgetsContract {
		public:

			ISliderContract();

			~ISliderContract();
			
			virtual void SetSlice(int slicenum) = 0;
			virtual int GetSlice() const = 0;
			virtual int GetNumberOfSlices() const = 0;

			virtual void GoToNextSeries() const;
			virtual void GoToPrevSeries() const;
		};		
	}
}

#endif