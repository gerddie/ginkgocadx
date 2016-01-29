/*
 *  
 *  $Id: anonymizetool.cpp $
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
#include <wx/msgdlg.h>
#include "anonymizetool.h"

#if defined(ENABLE_ANONYMIZETOOL)

#include <main/gui/anonymize/anonymizedialog.h>
#include <main/gui/history3/synchronizedialog.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>

//----------------------------------------------------------------------

GNC::AnonymizeTool::AnonymizeTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Anonymize, _Std("Anonymize"), _Std("Anonymize"), GinkgoResourcesManager::ToolIcons::GetIcoAnonymize()) 
{
}

GNC::AnonymizeTool::~AnonymizeTool()
{
}

void GNC::AnonymizeTool::Execute()
{
	GNC::GCS::IHistoryPanel* pHistory = GNC::HistoryToolsController::Instance()->GetHistoryPanel();
	std::list<long> listOfPks;
	pHistory->GetSelectedSeriesPk(listOfPks);
	Execute (listOfPks);
}

void GNC::AnonymizeTool::Execute(const std::list<long>& seriesPk) 
{
	GNC::GCS::IHistoryPanel* pHistory = GNC::HistoryToolsController::Instance()->GetHistoryPanel();
	if (seriesPk.empty()) {
		wxMessageBox(_("Select at least one series"), _("Info"), wxICON_INFORMATION, pHistory->GetWxWindow());
		return;
	}

	if (!AreSeriesFullyDownloaded(seriesPk)) {
		GNC::GUI::SynchronizeDialog dlg(pHistory->GetWxWindow(), seriesPk, this);
		dlg.ShowModal();
	} else {
		GNC::GUI::AnonymizeDialog dlg(pHistory->GetWxWindow(), seriesPk);
		dlg.ShowModal();
	}

}

#endif
