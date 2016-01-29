/*
*  
*  $Id: iopenstudytool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "iaddfiletohistorytool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IAddFileToHistory::ID = HCore_AddFileToHistory;

GNC::GCS::IAddFileToHistory::IAddFileToHistory() : ITool(ID, TMenuArchivo,  _Std("Open study"), GinkgoResourcesManager::MenuIcons::GetIcoAbrir())
{
}
