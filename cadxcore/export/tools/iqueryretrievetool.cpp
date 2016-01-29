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
#include "iqueryretrievetool.h"
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

GNC::GCS::ITool::UID GNC::GCS::IQueryRetrieveTool::ID = HCore_QueryRetrieve;

GNC::GCS::IQueryRetrieveTool::IQueryRetrieveTool() : ITool(ID, TMenuArchivo,  _Std("Acquire a DICOM study from PACS"), GinkgoResourcesManager::Acquisition::GetIcoAdquirePACS())
{
}
