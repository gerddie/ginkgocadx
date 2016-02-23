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

#ifndef WXMENUITEMTOOL_H
#define WXMENUITEMTOOL_H
#include <api/api.h>
#include <wx/menu.h>
#include <wx/menuitem.h>

namespace GNC {
	namespace GCS {
		class ITool;
	}
	namespace GUI {
		class EXTAPI wxMenuItemTool : public wxMenuItem {
			public:
				wxMenuItemTool(wxMenu* pMenuPadre, long id, const wxString& name, const wxBitmap& bitmap, wxEvtHandler* pHandler, int type = wxITEM_NORMAL);

				virtual ~wxMenuItemTool();

				wxEvtHandler* m_pHandler;
			};

		class EXTAPI wxDefaultEvtHandlerTool: public wxEvtHandler
		{
			public:
				wxDefaultEvtHandlerTool(wxEvtHandler* pParent, GNC::GCS::ITool* pTool);
				virtual ~wxDefaultEvtHandlerTool();

				virtual void OnToolClick(wxCommandEvent& event);
				virtual void OnUpdateUI(wxUpdateUIEvent& event);
			protected:
				GNC::GCS::ITool* m_pTool;
				wxEvtHandler* m_pParent;
		};

		class EXTAPI wxMenuTool : public wxMenu {
		public:
			wxMenuTool(wxEvtHandler* pHandler);

			virtual ~wxMenuTool();

			wxEvtHandler* m_pHandler;
		};
	}
}

#endif
