/*
 *  
 *  $Id: helpercompresion.h $
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
#include <string>

const bool DecompressJPEG2000(const std::string& fileIn, const std::string& fOut);

const bool CompressJPEG2000(const std::string& fileIn, const std::string& fOut);

