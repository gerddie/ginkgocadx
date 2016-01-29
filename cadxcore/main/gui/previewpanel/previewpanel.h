/*
 *  
 *  $Id: previewpanel.h $
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

