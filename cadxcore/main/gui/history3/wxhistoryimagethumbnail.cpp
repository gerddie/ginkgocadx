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
