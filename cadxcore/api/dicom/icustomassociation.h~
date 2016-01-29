/*
 *  
 *  $Id: icustomassociation.h $
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

#include <ostream>
#include <string>
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