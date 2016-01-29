/*
*  $Id: wxOftalImageThumbnail.cpp $
*  Proyecto Ginkgo
*
*  Copyright 2008 MetaEmotion S.L. All rights reserved.
*
*/

#include <wx/filename.h>
#include <wx/dc.h>

#include "wxhistoryimagethumbnail.h"
#include <resources/ginkgoresourcesmanager.h>
namespace GNC {
	namespace GUI {
		wxHistoryImageThumbnailItem::wxHistoryImageThumbnailItem(const wxString& filename, TItemType type, long series_pk, long file_pk, GNC::GCS::IHistoryController::TLocation source):wxImageThumbnailItem(filename)
		{
			m_type = type;
			m_filepk = file_pk;
			m_seriespk = series_pk;
			m_source = source;
		}

		bool wxHistoryImageThumbnailItem::Draw(wxDC& dc, wxThumbnailCtrl* ctrl, const wxRect& rect, int style)
		{
			wxImageThumbnailItem::Draw(dc, ctrl, rect, style);
			wxBitmap bmp;
			switch (m_source) {
				case GNC::GCS::IHistoryController::TL_LocalDatabase:
					bmp = GinkgoResourcesManager::History::GetIcoLocalFile();
					break;
				case GNC::GCS::IHistoryController::TL_LocalLinked:
					bmp = GinkgoResourcesManager::History::GetIcoLocalLinked();
					break;
				case GNC::GCS::IHistoryController::TL_WadoLinked:
					bmp = GinkgoResourcesManager::History::GetIcoWadoLinked();
					break;
				default:
					;
			}
			if (bmp.IsOk()) {
				int x = rect.x + ctrl->GetThumbnailMargin();
				int y = rect.y + ctrl->GetThumbnailMargin();
				dc.DrawBitmap(bmp, x, y, true);
			}
			return true;
		}
	}
}
