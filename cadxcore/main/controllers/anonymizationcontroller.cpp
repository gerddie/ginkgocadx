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

#include <wx/string.h>
#include "anonymizationcontroller.h"
#include "controladorlog.h"
#include "configurationcontroller.h"
#include <wx/xml/xml.h>
#include <wx/sstream.h>

namespace GNC 
{
	namespace GCS 
	{	
		AnonymizationController::TAnonymizationMap AnonymizationController::GetAnonymizationMap()
		{
			AnonymizationController::TAnonymizationMap map;
			std::string propertie;
			GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Anonymization", "ConfidentialityProfile", propertie);
			if (propertie.empty()) {
				map["0008|0014"] = TA_REPLACE_DEFAULT;
				map["0008|0018"] = TA_CREATE_NEW;
				map["0008|0050"] = TA_CREATE_NEW;
				map["0008|0080"] = TA_REPLACE_DEFAULT;
				map["0008|0081"] = TA_REPLACE_DEFAULT;
				map["0008|0090"] = TA_REPLACE_DEFAULT;
				map["0008|0092"] = TA_REPLACE_DEFAULT;
				map["0008|0094"] = TA_REPLACE_DEFAULT;
				map["0008|1010"] = TA_REPLACE_DEFAULT;
				map["0008|1030"] = TA_REPLACE_DEFAULT;
				map["0008|103e"] = TA_REPLACE_DEFAULT;
				map["0008|1040"] = TA_REPLACE_DEFAULT;
				map["0008|1048"] = TA_REPLACE_DEFAULT;
				map["0008|1060"] = TA_REPLACE_DEFAULT;
				map["0008|1070"] = TA_REPLACE_DEFAULT;
				map["0008|1080"] = TA_REPLACE_DEFAULT;
				map["0008|1155"] = TA_REPLACE_DEFAULT;
				map["0008|2111"] = TA_REPLACE_DEFAULT;
				map["0010|0010"] = TA_REPLACE_DEFAULT;
				map["0010|0020"] = TA_CREATE_NEW;
				map["0010|0030"] = TA_DELETE;
				map["0010|0032"] = TA_DELETE;
				map["0010|0040"] = TA_DELETE;
				map["0010|1000"] = TA_DELETE;
				map["0010|1001"] = TA_DELETE;
				map["0010|1010"] = TA_DELETE;
				map["0010|1020"] = TA_DELETE;
				map["0010|1030"] = TA_DELETE;
				map["0010|1090"] = TA_DELETE;
				map["0010|2160"] = TA_DELETE;
				map["0010|2180"] = TA_DELETE;
				map["0010|21b0"] = TA_DELETE;
				map["0010|4000"] = TA_DELETE;
				map["0018|1000"] = TA_DELETE;
				map["0018|1030"] = TA_DELETE;
				map["0020|000d"] = TA_CREATE_NEW;
				map["0020|000e"] = TA_CREATE_NEW;
				map["0020|0010"] = TA_REPLACE_DEFAULT;
				map["0020|0052"] = TA_CREATE_NEW;
				map["0020|0200"] = TA_CREATE_NEW;
				map["0020|4000"] = TA_REPLACE_DEFAULT;
				map["0040|0275"] = TA_DELETE;
				map["0040|a124"] = TA_CREATE_NEW;
				map["0040|a730"] = TA_DELETE;
				map["0088|0140"] = TA_CREATE_NEW;
				map["3006|0024"] = TA_CREATE_NEW;
				map["3006|00c2"] = TA_CREATE_NEW;
			}
			return map;
		}

		void AnonymizationController::ResetDefaultValues()
		{
			GNC::GCS::ConfigurationController::Instance()->deleteEntryUser("/GinkgoCore/Anonymization", "ConfidentialityProfile");
		}

		void AnonymizationController::SaveAnonymizationMap(const AnonymizationController::TAnonymizationMap& /*map*/)
		{
		}
	}
}

