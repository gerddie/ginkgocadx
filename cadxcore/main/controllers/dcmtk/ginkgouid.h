/*
 *  
 *  $Id: ginkgouid.h $
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
#include <string>
#include <api/api.h>

namespace GIL {
	namespace DICOM {

		typedef enum GinkgoUIDType {
			GUID_StudyRoot,
			GUID_SeriesRoot,
			GUID_InstanceRoot,
			GUID_PrivateGenericFileSOPClass
		} GinkgoUIDType;

		/***
		* Generates a unique UID on given root. If counter != -1 adds a level with those counter value
		**/
		std::string EXTAPI MakeUID(GinkgoUIDType uidType, int counter = -1);
	}
}