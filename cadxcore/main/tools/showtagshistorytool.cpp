/*
 *  
 *  $Id: showtagshistorytool.cpp $
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


#include "showtagshistorytool.h"

#if defined(ENABLE_SHOWTAGSHISTORYTOOL)
#include <wx/msgdlg.h>
#include <main/gui/paneltags/paneltags.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::ShowTagsHistoryTool::ShowTagsHistoryTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_ShowMetadata, _Std("DICOM inspector"), _Std("DICOM inspector"), GinkgoResourcesManager::ToolIcons::GetIcoMostrarOcultarTags()) 
{
}

GNC::ShowTagsHistoryTool::~ShowTagsHistoryTool()
{
}

void GNC::ShowTagsHistoryTool::Execute()
{
	GNC::GCS::IHistoryPanel* pHistory = GNC::HistoryToolsController::Instance()->GetHistoryPanel();
	std::list<long> listOfPks;
	pHistory->GetSelectedSeriesPk(listOfPks);

	if (listOfPks.empty()) {
		wxMessageBox(_("Select at least one series"), _("Info"), wxICON_INFORMATION, pHistory->GetWxWindow());
		return;
	}
	int i = 0;
	for (std::list<long>::iterator it = listOfPks.begin(); it != listOfPks.end(); ++it) {
		GNC::GUI::PanelTags* m_pPanel = new GNC::GUI::PanelTags(pHistory->GetWxWindow(), false);
		if (m_pPanel->SetSeriesPk((*it))) {
			m_pPanel->Show(true);
			wxPoint position = m_pPanel->GetPosition();
			position.x += i;
			position.y += i;
			m_pPanel->SetPosition(position);
			i += 10;
		} else {
			m_pPanel->Destroy();
		}
	}
}

#endif
