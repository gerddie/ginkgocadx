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
#include <map>
#include <list>

#include <api/autoptr.h>
#include <api/observers/ieventsobserver.h>
#include "ipreviewpanel.h"
#include "previewpanelmodel.h"
#include "previewpanelbase.h"

#include <wx/scrolwin.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>

class IModuleController;
class PanelPreview;
class SubTitledPanel;

namespace GNC {
	namespace GUI {		
		class EXTAPI PreviewPanel : public PreviewPanelBase , public GNC::GCS::IEventsObserver, public INodoHistorial {

		public:

			PreviewPanel(GNC::GCS::IVista* pView, wxWindow* parent);
			~PreviewPanel();

			void LoadModel(IPreviewNode* pNode);
			wxWindow* GetWxWindow();

		protected:
			virtual void OnChar(wxKeyEvent &event);
			virtual void OnHidePreviewsClick(wxCommandEvent& event);
			virtual void OnShowPreviewsClick(wxMouseEvent& event);
			virtual void FoldUp(bool foldup);
			virtual void PreProcesarEvento(GNC::GCS::Events::IEvent* evt, std::list<GNC::GCS::Events::IEvent*>& lista);
			virtual void ProcesarEvento(GNC::GCS::Events::IEvent* evt) ;
			virtual void OnEraseBackground( wxEraseEvent& event );
			
			GNC::GUI::ISeleccionablePreview* m_pSelected;
			GNC::GCS::IVista* m_pView;
			bool m_dirty;
		};
	}
}

