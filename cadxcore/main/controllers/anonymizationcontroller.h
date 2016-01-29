/*
 *  
 *  $Id: anonymizationcontroller.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <api/api.h>
#include <map>
#include <string>

namespace GNC {
	namespace GCS {

		class EXTAPI AnonymizationController {
		public:
			typedef enum TAction {
				TA_DELETE,
				TA_REPLACE_DEFAULT,
				TA_CREATE_NEW
			} TAction;
			typedef std::map<std::string, TAction> TAnonymizationMap;

			static TAnonymizationMap GetAnonymizationMap();
			static void ResetDefaultValues();
			static void SaveAnonymizationMap(const TAnonymizationMap& map);
		};
	}
}
