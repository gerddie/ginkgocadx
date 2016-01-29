/*
*  $Id: gnkprocontroller.cpp $
*  Proyecto Ginkgo
*
*  Copyright 2008 MetaEmotion S.L. All rights reserved.
*
*/

#include "anonusagestats.h"

//singleton
GNC::GCS::AnonUsageStats* GNC::GCS::AnonUsageStats::m_pInstance = NULL;

GNC::GCS::AnonUsageStats * GNC::GCS::AnonUsageStats::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new AnonUsageStats();
	}
	return m_pInstance;
}
void GNC::GCS::AnonUsageStats::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
//

GNC::GCS::IAnonUsageStatsDelegate::~IAnonUsageStatsDelegate()
{
}

GNC::GCS::AnonUsageStats::AnonUsageStats()
{
}

GNC::GCS::AnonUsageStats::~AnonUsageStats()
{
}

void GNC::GCS::AnonUsageStats::log(const std::string& operation, const std::string& description)
{
	try {
		if (Delegate.IsValid()) {
			Delegate->log(operation, description);
		}
	} 
	catch(...)
	{
	}
}

void GNC::GCS::AnonUsageStats::setDelegate(const GNC::GCS::Ptr<IAnonUsageStatsDelegate>& delegate)
{
	Delegate = delegate;
}

void GNC::GCS::AnonUsageStats::resetSettings()
{
	if (Delegate.IsValid()) {
		Delegate->resetSettings();
	}
}

