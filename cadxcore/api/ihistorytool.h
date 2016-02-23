#pragma once
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

#ifndef IHISTORYTOOL_H
#define IHISTORYTOOL_H
#include <string>
#include <list>

#include <api/api.h>
#include <wx/bitmap.h>

class wxWindow;
class wxGinkgoToolBar;
class wxEvtHandler;
class wxMenu;

namespace GNC {
	namespace GCS {

		class EventHandlerHistoryTool;

		class EXTAPI IHistoryTool
		{
		public:
			typedef unsigned int UID;
			typedef enum {
					TFamily_Open,
					TFamily_Dicomize,
					TFamily_Delete,
					TFamily_ShowMetadata,
					TFamily_Q_R,
					TFamily_Anonymize,
					TFamily_Export,
					TFamily_Other,
					TFamily_Send
				} TToolFamily;

			IHistoryTool(UID id, TToolFamily family, const std::string& shortDescription, const std::string& description, const wxBitmap& icon, const wxBitmap& bigIcon = wxBitmap());

			virtual ~IHistoryTool();

			virtual void AppendToolIn(wxGinkgoToolBar* pParent, bool big = false);
			virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

			virtual void Execute() = 0;
			virtual void Execute(const std::list<long>& seriesPk);

			virtual bool Enabled() { return true;}
			virtual bool AreSeriesFullyDownloaded(const std::list<long>& seriesPk);

			UID ID;
			TToolFamily Family;
			std::string ShortDescription;
			std::string Description;
			wxBitmap Icon;
			wxBitmap BigIcon;
			EventHandlerHistoryTool* EventHandler;
		};
	}
}
#endif
