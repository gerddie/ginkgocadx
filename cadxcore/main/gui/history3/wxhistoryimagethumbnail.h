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

#pragma once

#include <wx/thumbnailctrl/thumbnailctrl.h>
#include <api/controllers/ihistorycontroller.h>

namespace GNC {
	namespace GUI {
		class wxHistoryImageThumbnailItem: public wxImageThumbnailItem
		{
			public:
			typedef enum TItemType {
				TI_Series,
				TI_Image
			}TItemType;

			wxHistoryImageThumbnailItem(const wxString& filename, TItemType type, long series_pk, long file_pk, GNC::GCS::IHistoryController::TLocation source);

			TItemType GetType() 
			{
				return m_type;
			}

			long GetFilePk()
			{
				return m_filepk;
			}

			long GetSeriesPk()
			{
				return m_seriespk;
			}

			virtual bool Draw(wxDC& dc, wxThumbnailCtrl* ctrl, const wxRect& rect, int style);

		protected:
			TItemType m_type;
			GNC::GCS::IHistoryController::TLocation m_source;
			long m_filepk;
			long m_seriespk;
		};
	}
}
