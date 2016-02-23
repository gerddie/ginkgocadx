/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <string>
#include <api/autoptr.h>
#include <api/controllers/anonusagestatsids.h>

//macro definition
#define LOG_STAT(action) \
{ \
	GNC::GCS::AnonUsageStats* cA = GNC::GCS::AnonUsageStats::Instance(); \
	if (cA != NULL) { \
		cA->log(action);\
	} \
}

#define LOG_STAT_WITH_DESCRIPTION(action, description) \
{ \
	GNC::GCS::AnonUsageStats* cA = GNC::GCS::AnonUsageStats::Instance(); \
	if (cA != NULL) { \
		cA->log(action, description);\
	} \
}

namespace GNC {	
	namespace GCS {

		class IAnonUsageStatsDelegate
		{
		public:
			virtual ~IAnonUsageStatsDelegate();
			virtual void log(const std::string& operation, const std::string& description) = 0;
			virtual void resetSettings() = 0;
		};

		class AnonUsageStats
		{
		public:
			static AnonUsageStats *Instance();
			static void FreeInstance();

			void log(const std::string& operation, const std::string& description = "");
			void setDelegate(const GNC::GCS::Ptr<IAnonUsageStatsDelegate>& deleg);
			void resetSettings();
		protected:
			AnonUsageStats();
			~AnonUsageStats();

			GNC::GCS::Ptr<IAnonUsageStatsDelegate> Delegate;
			//singleton
			static AnonUsageStats* m_pInstance;
		};
	}
}


