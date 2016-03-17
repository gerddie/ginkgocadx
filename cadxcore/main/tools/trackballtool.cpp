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

#include "trackballtool.h"


#include <export/contracts/iwidgetscontract.h>

#include <api/globals.h>
#include <api/iwidgetsmanager.h>
#include <resources/ginkgoresourcesmanager.h>
#include <widgets/wtrackball.h>

#include <iostream>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>


GNC::TrackballTool::TrackballTool()
{
        m_pBuilder = NULL;
}

GNC::TrackballTool::~TrackballTool()
{
        if (m_pBuilder != NULL) {
                delete m_pBuilder;
                m_pBuilder = NULL;
        }
}

GNC::GCS::ITool* GNC::TrackballTool::NewTool()
{
        return new GNC::TrackballTool();
}

void GNC::TrackballTool::Connect()
{
        if (m_pBuilder != NULL) {
                delete m_pBuilder;
                m_pBuilder = NULL;
        }
        m_pBuilder = new TBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
        WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
        WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
}

void GNC::TrackballTool::Disconnect()
{
        WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
        WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
        // DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
        if (m_pBuilder != NULL) {
                delete m_pBuilder;
                m_pBuilder = NULL;
        }
}
