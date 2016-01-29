/*
 *  $Id: wxOftalImageThumbnail.h $
 *  Proyecto Ginkgo
 *
 *  Copyright 2008 MetaEmotion S.L. All rights reserved.
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