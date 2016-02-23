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
#include <string>
#include <list>
#include <api/autoptr.h>

#include <main/controllers/historytoolscontroller.h>
#include <api/controllers/ieventscontroller.h>

#include <wx/wxginkgotoolbar.h>

namespace GNC {
	namespace GUI {
		class HistoryPanel3;

		class HistoryToolMenu: public wxGinkgoToolBar
		{
			public:
				HistoryToolMenu(HistoryPanel3* pParent);
				~HistoryToolMenu();

			public:
				void Reload();
				
			protected:
				//MENUS...
				//drop downs
				void OnDropDownOpenFile(wxAuiToolBarEvent& event);
				void OnDropDownRemove(wxAuiToolBarEvent& event);
				//file actions...
				//remove
				void AppendEditTools(wxMenu* pEditMenu, wxEvtHandler* pHandler);
				void OnRemove(wxCommandEvent& event);
				void OnRemoveAll(wxCommandEvent& event);
				void OnSelectAll(wxCommandEvent& event);
				void OnDeselectAll(wxCommandEvent& event);
				void OnInvertSelection(wxCommandEvent& event);
				void OnCloseWindow(wxCommandEvent& event);
				//query/retrieve
				void OnQueryRetrieve(wxCommandEvent& event);
				

			public:
				HistoryPanel3* HistoryPanel;
		};
	}
}
