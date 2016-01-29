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
#include "ishowhistorytool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IShowHistoryTool::ID = HCore_ShowHistory;

GNC::GCS::IShowHistoryTool::IShowHistoryTool() : ITool(ID, TMenuArchivo,  _Std("Show history \tAlt+h"), GinkgoResourcesManager::History::GetIcoHistorialOpen())
{
}
