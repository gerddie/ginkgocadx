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

#ifndef WXGINKGOTOOLBAR_H
#define WXGINKGOTOOLBAR_H

#include <api/toolsystem/itool.h>
#include <api/observers/ieventsobserver.h>

#include <wx/dc.h>
#include <wx/wxginkgotoolbar.h>

namespace GNC {
	namespace GCS {
		class IToolController;
	}
	namespace GUI {
		class GinkgoToolSystemBar : public wxGinkgoToolBar, public GNC::GCS::IEventsObserver {
		public:
			GinkgoToolSystemBar(wxWindow* pParent, GNC::GCS::IToolController* pToolBar, const GNC::GCS::ITool::TToolFamily& family);
			~GinkgoToolSystemBar();

			virtual void ProcesarEvento(GNC::GCS::Events::IEvent* evt);

		protected:
			virtual wxRect GetToolRect(int tool_id) const;
			virtual void OnRightUp(wxMouseEvent& evt);
			virtual void OnLeftUp(wxMouseEvent& evt);
			virtual void OnMiddleUp(wxMouseEvent& evt);
			virtual void OnDropDownTool(wxAuiToolBarEvent& evt);
			virtual void OnKeyEvent(wxKeyEvent& event);

			GNC::GCS::IToolController* m_pToolBar;
		};
	}
}

#endif
