/*
 *  
 *  $Id: sendpacstool.cpp $
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

#include "sendpacstool.h"

#include <api/internationalization/internationalization.h>
#include <api/controllers/icontroladorpermisos.h>
#include <main/controllers/commandcontroller.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/gui/selectpacsserver/selectpacsserver.h>
#include <main/gui/history3/synchronizedialog.h>
#include <main/controllers/historycontroller.h>
#include <commands/sendpacscommand.h>



//----------------------------------------------------------------------

GNC::SendPACSTool::SendPACSTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Send, _Std("Send to PACS"), _Std("Send to PACS"), GinkgoResourcesManager::MenuIcons::GetIcoSendToPACS()) 
{
}

GNC::SendPACSTool::~SendPACSTool()
{
}

bool GNC::SendPACSTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_upload");
}

void GNC::SendPACSTool::Execute()
{
	GNC::GCS::IHistoryPanel* pHistory = GNC::HistoryToolsController::Instance()->GetHistoryPanel();
	std::list<long> listOfPks;
	pHistory->GetSelectedSeriesPk(listOfPks);
	Execute(listOfPks);
}

void GNC::SendPACSTool::Execute(const std::list<long>& listOfPks)
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
		GNC::GUI::SelectPacsServer dlg(pHistory->GetWxWindow());
		if (dlg.ShowModal() == wxID_OK) {
			GADAPI::SendPACSCommand* pCmd = new GADAPI::SendPACSCommand(dlg.GetSelectedServer(), "", listOfPks);
			GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Sending to PACS..."),pCmd, NULL);
		}
	}
}



