/*
 *  
 *  $Id: firstlevelpanel.h $
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

#include <api/autoptr.h>
#include "previewpanelbase.h"
#include "ipreviewpanel.h"
#include "previewpanelmodel.h"

namespace GNC {
	namespace GUI {
		class FirstLevelPanel: public FirstLevelPanelBase, public INodoHistorial {
		public:
			FirstLevelPanel(GNC::GCS::IVista* pView, INodoHistorial* pPadre,  GNC::GUI::IPreviewNode* pNode);
			~FirstLevelPanel();

			wxWindow* GetWxWindow();
			virtual void GetNodeSize(int &x, int &y);


		protected:
			virtual void OnChar(wxKeyEvent &event);
			virtual void OnPaint(wxPaintEvent& event);
			virtual void OnSize(wxSizeEvent &event);
			virtual void OnMouseWheel(wxMouseEvent &event);
			virtual void OnEraseBackground( wxEraseEvent& event );

			GNC::GUI::IPreviewNode* m_pNode;
			GNC::GCS::IVista* m_pView;
		};
	}
}
