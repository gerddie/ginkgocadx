/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
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
