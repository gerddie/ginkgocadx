/*
 *  
 *  $Id: istorecallback.h $
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

class DcmDataset;

class IStoreCallBack {
public:
    IStoreCallBack() {}
    virtual ~IStoreCallBack(){}

	 virtual void Store(const std::string& fileName, DcmDataset* dset) = 0;
};

