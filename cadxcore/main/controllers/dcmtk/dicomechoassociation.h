/*
 *  
 *  $Id: dicomechoassociation.h $
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
#include "dicomassociation.h"
#include <api/dicom/imodelodicom.h>

class EchoAssociation : public Association {
public:
	
	/**
	 Constructors
	 */
	EchoAssociation(const std::string& ambitolog);
	
	virtual ~EchoAssociation();
	
	virtual void OnAddPresentationContext(T_ASC_Parameters* params);
	
};
