/*
 *
 *  $Id: InactivityCountdownMessage.cpp $
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
			IsSeries(isSeries),
			UID(uid)
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
