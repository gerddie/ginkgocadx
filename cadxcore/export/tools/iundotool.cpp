/*
*  
*  $Id: iundotool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "iundotool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IUndoTool::ID = HCore_Deshacer;

GNC::GCS::IUndoTool::IUndoTool() : ITool(ID, TMenuEdicion, _Std("Undo"), GinkgoResourcesManager::AngleBar::GetIcoAnguloNormal())
{
}
