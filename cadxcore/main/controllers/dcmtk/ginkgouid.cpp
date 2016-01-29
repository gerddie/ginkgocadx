/*
 *  
 *  $Id: ginkgouid.cpp $
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
#include "ginkgouid.h"
#include <sstream>
#include <api/ientorno.h>
#include <dcmtk/dcmdata/dcuid.h>

// MetaEmotion.Healthcate.GinkgoCADx.MayorVersion.MinorVersion

#define GINKGO_UID_ROOT                           "1.3.6.1.4.1.39470.1.1"

#define GINKGO_STUDY_UID_ROOT                     ".1.2"
#define GINKGO_SERIES_UID_ROOT                    ".1.3"
#define GINKGO_INSTANCE_UID_ROOT                  ".1.4"
#define GINKGO_UID_PRIVATE_GENERIC_FILE_SOP_CLASS ".1.0.1"


std::string GIL::DICOM::MakeUID(GinkgoUIDType uidType, int counter) {
	char newUID[128];
	std::ostringstream  buf;
	buf << GINKGO_UID_ROOT << "." << GNC::GCS::IEntorno::Instance()->GetGinkgoMayorVersionNumber() << "." << GNC::GCS::IEntorno::Instance()->GetGinkgoMinorVersionNumber();

	switch(uidType) {
		case GUID_StudyRoot:
			buf << GINKGO_STUDY_UID_ROOT;
			break;
		case GUID_SeriesRoot:
			buf << GINKGO_SERIES_UID_ROOT;
			break;
		case GUID_InstanceRoot:
			buf << GINKGO_INSTANCE_UID_ROOT;
			break;
		case GUID_PrivateGenericFileSOPClass:
			buf << GINKGO_UID_PRIVATE_GENERIC_FILE_SOP_CLASS;
			break;
		default:
			break;
	}

	if (counter >= 0 ) {
		buf << counter;
	}

	dcmGenerateUniqueIdentifier(newUID, buf.str().c_str());
	return std::string(newUID);
}