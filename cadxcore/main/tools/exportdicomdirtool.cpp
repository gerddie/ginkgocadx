/*
 *  
 *  $Id: exportdicomdirtool.cpp $
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

#include <wx/msgdlg.h>

#include "exportdicomdirtool.h"

#include <api/imodelointegracion.h>

#include <api/controllers/icontroladorpermisos.h>
#include <main/gui/dicomdirexport/exportdicomdir.h>
#include <main/gui/history3/synchronizedialog.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>

#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::ExportDicomDirTool::ExportDicomDirTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Export, _Std("Export to DICOM Dir"), _Std("Export to DICOM Dir")+ " \tAlt+e", GinkgoResourcesManager::MenuIcons::GetIcoDicomDir()) 
{
}

GNC::ExportDicomDirTool::~ExportDicomDirTool()
{
}

bool GNC::ExportDicomDirTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","export");
}

void GNC::ExportDicomDirTool::Execute()
{
	GNC::GCS::IHistoryPanel* pHistory = GNC::HistoryToolsController::Instance()->GetHistoryPanel();
	std::list<long> listOfPks;
	pHistory->GetSelectedSeriesPk(listOfPks);
	Execute(listOfPks);
}

void GNC::ExportDicomDirTool::Execute(const std::list<long>& listOfPks)
{
	GNC::GCS::IHistoryPanel* pHistory = GNC::HistoryToolsController::Instance()->GetHistoryPanel();
	if (listOfPks.empty()) {
		wxMessageBox(_("Select at least one series"), _("Info"), wxICON_INFORMATION, pHistory->GetWxWindow());
		return;
	}

	if (!AreSeriesFullyDownloaded(listOfPks)) {
		GNC::GUI::SynchronizeDialog dlg(pHistory->GetWxWindow(), listOfPks, this);
		dlg.ShowModal();
	} else {
		GNC::GUI::ExportDicomDir dlg(pHistory->GetWxWindow(), listOfPks);
		dlg.ShowModal();
	}
}



