/*
 *  
 *  $Id: dicomizetool.cpp $
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

#include "dicomizetool.h"

#include <api/controllers/icontroladorlog.h>
#include <api/controllers/icontroladorpermisos.h>

#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/gui/import/wxwizardimportacionginkgo.h>
#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::DicomizeTool::DicomizeTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Dicomize, _Std("Dicomize"), _Std("Dicomize files")+ " \tAlt+d", GinkgoResourcesManager::MenuIcons::GetIcoImportar(), GinkgoResourcesManager::BigIcons::GetIcoBigDicomize()) 
{
}

GNC::DicomizeTool::~DicomizeTool()
{
}

bool GNC::DicomizeTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","import");
}

void GNC::DicomizeTool::Execute()
{
	try{
		GNC::GUI::wxWizardImportacionGinkgo ib(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow(),NULL);

		ib.ShowModal();
	}
	catch(GIL::DICOM::I2DException& i){
		LOG_ERROR("DicomizeTool", "Error in dicomization: " << i.GetCause())
	}
}



