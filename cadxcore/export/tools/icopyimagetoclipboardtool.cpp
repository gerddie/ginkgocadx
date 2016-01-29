/*
*  
*  $Id: isecondarycapturetool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "icopyimagetoclipboardtool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::ICopyImageToClipboardTool::ID = HCore_CopyImageToClipboard;

GNC::GCS::ICopyImageToClipboardTool::ICopyImageToClipboardTool() : ITool(ID, TMenuEdicion,  _Std("Copy") + " \tCtrl+c", GinkgoResourcesManager::ToolIcons::GetIcoCopyToClipboard(),3)
{
}
