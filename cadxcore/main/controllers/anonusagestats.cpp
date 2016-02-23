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

