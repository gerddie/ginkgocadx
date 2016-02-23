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

#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>

#include "downloadagaindialog.h"
#include <wx/ginkgostyle/ginkgostyle.h>
#include <main/controllers/historycontroller.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/gui/history3/historypanel3.h>
#include <main/controllers/inactivitycontroller.h>
#include <main/entorno.h>

namespace GNC {
	namespace GUI {
		DownloadAgainDialog::DownloadAgainDialog(wxWindow* pParent, std::string uid, bool isSeries): DownloadAgainDialogBase(pParent),
                                                                                                             UID(uid),
                                                                                                             IsSeries(isSeries)
		{
			if (IsSeries) {
				m_pLabelMessage->SetLabel(_("This series has been already downloaded in the local history, Do you want to download it again?"));
			} else {
				m_pLabelMessage->SetLabel(_("This study has been already downloaded in the local history, Do you want to download it again?"));
			}
			m_pLabelMessage->Wrap(350);
			m_pFormPanel->Layout();
			m_pBody->Layout();
			Layout();
		}

		DownloadAgainDialog::~DownloadAgainDialog()
		{
		}
			
		void DownloadAgainDialog::OnOpenClick(wxCommandEvent &)
		{
			if (IsSeries) {
				GNC::GCS::HistoryController::SeriesModelList sml;
				GNC::GCS::HistoryController::Instance()->GetSeriesModelFromUID(UID, sml);
				if (!sml.empty()) {
					GNC::GUI::HistoryPanel3::Instance()->OpenSeriesOrStudy(sml.front().pk, true);
				}
			} else {
				GNC::GCS::HistoryController::StudyModelList sml;
				GNC::GCS::HistoryController::Instance()->GetStudyModelFromUID(UID, sml);
				if (!sml.empty()) {
					GNC::GUI::HistoryPanel3::Instance()->OpenSeriesOrStudy(sml.front().pk, false);
				}
			}
			EndModal(wxID_CANCEL);
		}
	}
}
