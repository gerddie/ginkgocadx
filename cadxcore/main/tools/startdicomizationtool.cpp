/*
 *  
 *  $Id: startdicomizationtool.cpp $
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
#include "startdicomizationtool.h"
#include <export/contracts/iviewcontract.h>
#include <api/ivista.h>
#include <api/controllers/icontroladorlog.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <main/entorno.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/gui/history3/historypanel3.h>

#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/gui/import/wxwizardimportacionginkgo.h>
#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::GCS::ITool* GNC::StartDicomizationTool::NewTool()
{
	return new GNC::StartDicomizationTool();
}

GNC::StartDicomizationTool::StartDicomizationTool()
{
}
GNC::StartDicomizationTool::~StartDicomizationTool()
{
}
		
bool GNC::StartDicomizationTool::ExecuteAction()
{	
	try{
		GNC::GUI::wxWizardImportacionGinkgo ib(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow(),NULL);

		ib.ShowModal();
	}
	catch(GIL::DICOM::I2DException& i){
		LOG_ERROR("DicomizeTool", "Error in dicomization: " << i.GetCause())
	}
	
	return true;
}

void GNC::StartDicomizationTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		GNC::GUI::wxDefaultEvtHandlerTool* evtHandler = new GNC::GUI::wxDefaultEvtHandlerTool(pParent, this);
		pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
	}
}

bool GNC::StartDicomizationTool::IsShown()
{
	return GNC::GUI::HistoryPanel3::Instance()->IsShown();
}
