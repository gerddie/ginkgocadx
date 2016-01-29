/*
*  
*  $Id: ishowhistorytool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "iimageexporttool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IImageExportTool::ID = HCore_ImageExport;

GNC::GCS::IImageExportTool::IImageExportTool() : ITool(ID, TMenuArchivo,  _Std("Export image..."), GinkgoResourcesManager::MenuIcons::GetIcoExportar(), 4)
{
}
