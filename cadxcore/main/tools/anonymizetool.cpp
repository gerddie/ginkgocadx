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
