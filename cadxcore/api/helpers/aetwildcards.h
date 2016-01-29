/*
 *  
 *  $Id: aetwildcards.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

#include <string>
#include <api/api.h>
namespace GNC {
	namespace GCS {
		class EXTAPI AETWildcards {
		public:
			static std::string Parse(const std::string& AET);
		};
	}
}
