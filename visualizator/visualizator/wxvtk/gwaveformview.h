/*
 *  Ginkgo CADx Project
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

#include <api/observers/ieventsobserver.h>
#include <export/contracts/iwidgetscontract.h>

#include "visualizatorviews.h"

class vtkGinkgoImageViewer;
class wxGinkgoToolBar;
namespace GNKVisualizator
{
namespace Widgets
{
class WECG;
}
class WaveFormView;
namespace GUI
{
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

