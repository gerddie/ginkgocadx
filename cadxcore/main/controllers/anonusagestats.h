/*
*  $Id: auditcontrollerdelegate.h $
*  Proyecto Ginkgo
*
*  Copyright 2008 MetaEmotion S.L. All rights reserved.
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


