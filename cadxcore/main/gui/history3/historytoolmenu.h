/*
 *  
 *  $Id: historytoolmenu.h $
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
