#pragma once
/*
 *  
 *  $Id: wxginkgotoolbar.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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