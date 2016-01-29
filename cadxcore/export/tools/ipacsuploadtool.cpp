/*
*  
*  $Id: ipacsuploadtool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "ipacsuploadtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IPACSUploadTool::ID = HCore_PACSUpload;

GNC::GCS::IPACSUploadTool::IPACSUploadTool() : ITool(ID, TMenuArchivo,  _Std("Upload study..."), GinkgoResourcesManager::MenuIcons::GetIcoSendToPACS(),4)
{
}
