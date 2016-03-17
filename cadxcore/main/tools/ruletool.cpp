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

#include "ruletool.h"

#if defined(ENABLE_RULETOOL)
#include <export/contracts/irulecontract.h>
#include <iostream>

#include <widgets/wrule.h>
#include <resources/ginkgoresourcesmanager.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

GNC::RuleTool::RuleTool() :
        m_pRuleBuilder(NULL)
{
        m_pRuleBuilder = NULL;
}

GNC::RuleTool::~RuleTool()
{

}


//------------------------------------------------------------------------------
//region "ITool/IContract realization"

GNC::GCS::ITool* GNC::RuleTool::NewTool()
{
        return new GNC::RuleTool();
}

void GNC::RuleTool::Connect()
{
        if (RuleContract == NULL || RuleContract->GetManager()==NULL) {
                return;
        }
        if (m_pRuleBuilder != NULL) {
                delete m_pRuleBuilder;
                m_pRuleBuilder = NULL;
        }

        m_pRuleBuilder = new GNC::GCS::Widgets::WRuleBuilder(RuleContract->GetManager(), GetTriggerButton(), RuleContract->SupportsCallibration());
        RuleContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
        RuleContract->GetManager()->SetCursor(m_pRuleBuilder->GetCursor());
}

void GNC::RuleTool::Disconnect()
{
        if (RuleContract == NULL) {
                return;
        }
        if (RuleContract->GetViewerActivo() != NULL) {
                RuleContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
        }
        if (RuleContract->GetManager() != NULL) {
                RuleContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
        }
        if (m_pRuleBuilder != NULL) {
                delete m_pRuleBuilder;
                m_pRuleBuilder = NULL;
        }
}

//endregion
#endif
