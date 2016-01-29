#pragma once
/*
*
*  $Id: iwindowlevelcontract.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include <list>
#include <vector>

#include <api/api.h>
#include <export/contracts/iwidgetscontract.h>

namespace GIL {
	namespace DICOM {
		class DicomDataset;
	}
}

namespace GNC {
	namespace GCS {
		class EXTAPI IPixelsBluringContract : public IWidgetsContract {
		public:
			IPixelsBluringContract();

			~IPixelsBluringContract();

			virtual void Setup(const std::string& modality);
		};
	}
}

