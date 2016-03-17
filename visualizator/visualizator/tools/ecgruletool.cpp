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

#include "ecgruletool.h"

#include <export/contracts/iwidgetscontract.h>
#include <iostream>

#include <visualizator/widgets/wecgrule.h>
#include <resources/ginkgoresourcesmanager.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

GNKVisualizator::ECGRuleTool::ECGRuleTool() :
        m_pRuleBuilder(NULL)
{
        m_pRuleBuilder = NULL;
}

GNKVisualizator::ECGRuleTool::~ECGRuleTool()
{

}


//------------------------------------------------------------------------------
//region "ITool/IContract realization"

GNC::GCS::ITool* GNKVisualizator::ECGRuleTool::NewTool()
{
        return new GNKVisualizator::ECGRuleTool();
}

void GNKVisualizator::ECGRuleTool::Connect()
{
        if (WidgetsContract == NULL) {
                return;
        }
        if (m_pRuleBuilder != NULL) {
                delete m_pRuleBuilder;
                m_pRuleBuilder = NULL;
        }

        m_pRuleBuilder = new GNKVisualizator::Widgets::WECGRuleBuilder(WidgetsContract->GetManager(), GetTriggerButton());
        WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
        WidgetsContract->GetManager()->SetCursor(m_pRuleBuilder->GetCursor());
}

void GNKVisualizator::ECGRuleTool::Disconnect()
{
        if (WidgetsContract == NULL) {
                return;
        }
        WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
        WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
        if (m_pRuleBuilder != NULL) {
                delete m_pRuleBuilder;
                m_pRuleBuilder = NULL;
        }
}

//endregion
