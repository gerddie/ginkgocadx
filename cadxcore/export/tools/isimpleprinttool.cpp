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
#include "isimpleprinttool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::ISimplePrintTool::ID = HCore_SimplePrint;

GNC::GCS::ISimplePrintTool::ISimplePrintTool() : ITool(ID, TMenuArchivo,  _Std("Print") + "...", GinkgoResourcesManager::MenuIcons::GetIcoImpresora(),5)
{
}
