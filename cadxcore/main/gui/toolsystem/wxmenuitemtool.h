#pragma once
/*
 *  
 *  $Id: wxmenuitemtool.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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
