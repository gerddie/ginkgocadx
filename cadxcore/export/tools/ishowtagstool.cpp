/*
*  
*  $Id: ishowtagstool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "ishowtagstool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IShowTagsTool::ID = HCore_ShowDicomTags;

GNC::GCS::IShowTagsTool::IShowTagsTool() : ITool(ID, TMenuHerramientas,  _Std("DICOM inspector"), GinkgoResourcesManager::ToolIcons::GetIcoMostrarOcultarTags())
{
}
