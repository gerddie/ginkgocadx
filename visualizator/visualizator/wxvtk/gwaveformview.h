/*
 *  
 *  $Id: gwaveformview.h $
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

#include <api/observers/ieventsobserver.h>
#include <export/contracts/iwidgetscontract.h>

#include "visualizatorviews.h"

class vtkGinkgoImageViewer;
class wxGinkgoToolBar;
namespace GNKVisualizator{
	namespace Widgets {
		class WECG;
	}
	class WaveFormView;
	namespace GUI {
		class GWaveformViewDelegate;
		class GWaveformAnnotatorDelegate;
			
		class GWaveformView : public GWaveformViewBase, public GNC::GCS::IWidgetsContract, public GNC::GCS::IEventsObserver
		{
			friend class GWaveformViewDelegate;
			friend class GWaveformAnnotatorDelegate;
		public:
			/* Constructor */
			GWaveformView(GNKVisualizator::WaveFormView* pView);

			/* Destructor */
			~GWaveformView();

			void InitPipeline();

			void StopPipeline();

			void OnFinishLoad();
						
			virtual GNC::GCS::IWidgetsManager* GetManager();

			void OnPaint(wxPaintEvent& event);
			virtual void OnComboGroup(wxCommandEvent &event);
			virtual void OnShowHideMetadata(wxCommandEvent &event);
			virtual void ShowMetaData(bool show);
			virtual void OnShowHideCornerAnnotations(wxCommandEvent &event);

			

		protected:
			void OnFocus(wxChildFocusEvent &event);
			
			void PreProcesarEvento(GNC::GCS::Events::IEvent* evt, std::list<GNC::GCS::Events::IEvent*>& lista);
			void ProcesarEvento(GNC::GCS::Events::IEvent *evt) ;

			GNKVisualizator::WaveFormView* View;
			GNKVisualizator::Widgets::WECG* WECG;
			wxGinkgoToolBar* m_pButtonBar;
			
			GNC::GCS::IWidgetsManager* m_pManager;
			GNC::GCS::Ptr<GWaveformViewDelegate> Delegate;
			GNC::GCS::Ptr<GWaveformAnnotatorDelegate> AnnotatorDelegate;
		};
	}
}

