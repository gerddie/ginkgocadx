/*
 *  
 *  $Id: queryretrievetool.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
//#define _GINKGO_TRACE

#include "queryretrievehistorytool.h"
#include <api/controllers/icontroladorpermisos.h>

#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/gui/acquisition/dialogoadquisicion.h>

//----------------------------------------------------------------------

GNC::QueryRetrieveHistoryTool::QueryRetrieveHistoryTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Q_R, _Std("PACS Q/R"), _Std("Acquire a DICOM study from PACS")+ " \tAlt+a", GinkgoResourcesManager::Acquisition::GetIcoAdquirePACS(), GinkgoResourcesManager::BigIcons::GetIcoBigQR()) 
{
}

GNC::QueryRetrieveHistoryTool::~QueryRetrieveHistoryTool()
{
}

bool GNC::QueryRetrieveHistoryTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && 
		GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_acquisition");
}

void GNC::QueryRetrieveHistoryTool::Execute()
{
	GNC::GUI::DialogoAdquisicion::Instance()->Show();
}



