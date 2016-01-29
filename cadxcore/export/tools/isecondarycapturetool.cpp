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
#include "isecondarycapturetool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::ISecondaryCaptureTool::ID = HCore_SecondaryCapture;

GNC::GCS::ISecondaryCaptureTool::ISecondaryCaptureTool() : ITool(ID, TMenuArchivo,  _Std("Save snapshot as new series..."), GinkgoResourcesManager::MenuIcons::GetIcoGuardar(),3)
{
}
