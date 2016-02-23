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

#include <ostream>
#include <string>
#include <api/autoptr.h>
#include <api/dicom/idicom.h>
#include <api/dicom/idicomconformance.h>

namespace GIL {
	namespace DICOM {	
		class EXTAPI ICustomAssociation {
			
		protected:

			ICustomAssociation();

		public:
			virtual ~ICustomAssociation();

		public:

			virtual bool Connect(const std::string& serverId, const std::string& local_aet) = 0;

			virtual bool createRQ( const std::string& sopclassUID, const GIL::DICOM::DicomDataset& attributeListIn,  int& status, std::string& affectedSopinstanceUID) = 0;
			virtual bool setRQ(    const std::string& sopclassUID, const std::string& sopinstanceUID, const GIL::DICOM::DicomDataset& modificationList, int& status) = 0;
			virtual bool getRQ(    const std::string& sopclassUID, const std::string& sopinstanceUID, const int *attributeIdentifierList, unsigned long numShorts, int& status,  GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& attributeListOut) = 0;
			virtual bool actionRQ( const std::string& sopclassUID, const std::string& sopinstanceUID, int actionTypeID, const GIL::DICOM::DicomDataset& actionInformation, int& status,  GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& attributeListOut) = 0;
			virtual bool deleteRQ( const std::string& sopclassUID, const std::string& sopinstanceUID, int& status) = 0;

			virtual bool releaseAssociation() = 0;
			virtual bool abortAssociation() = 0;

		public:

			void SetStorageSOPClasses(const GIL::DICOM::SOPClassList& SOPClasses);
			const std::string& getErrorMessage() const;

		protected:

			std::string errorMsg;		
			GIL::DICOM::SOPClassList SOPClasses;
		};
	}
}
