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


std::string GIL::DICOM::MakeUID(GinkgoUIDType uidType, int counter)
{
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
