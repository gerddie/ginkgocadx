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
#include <api/autoptr.h>
#include "icustomassociation.h"

GIL::DICOM::ICustomAssociation::ICustomAssociation() { }

GIL::DICOM::ICustomAssociation::~ICustomAssociation() { }

void GIL::DICOM::ICustomAssociation::SetStorageSOPClasses(const GIL::DICOM::SOPClassList& SOPClasses)
{
	this->SOPClasses = SOPClasses;
}

const std::string& GIL::DICOM::ICustomAssociation::getErrorMessage() const
{
	return errorMsg;
}