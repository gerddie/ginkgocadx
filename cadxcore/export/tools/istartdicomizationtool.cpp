/*
*  
*  $Id: istartdicomizationtool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "istartdicomizationtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IStartDicomizationTool::ID = HCore_DicomizeFromView;

GNC::GCS::IStartDicomizationTool::IStartDicomizationTool() : ITool(ID, TMenuArchivo,  _Std("Dicomize..."), GinkgoResourcesManager::MenuIcons::GetIcoImportar(), 2)
{
}
