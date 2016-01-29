/*
 *  
 *  $Id: ipacscontroller.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <list>
#include <map>

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif
#include <api/controllers/ipacscontroller.h>

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

GIL::DICOM::PACSException::PACSException(const std::string& msg, const std::string& component, bool fatal) throw() : GNC::GCS::IException(msg, component, fatal)
{
}

GIL::DICOM::IPACSController::IPACSController()
{
}

GIL::DICOM::IPACSController::~IPACSController()
{
}

