/*
 *  
 *  $Id: ruletool.cpp $
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

GNKVisualizator::ECGRuleTool::~ECGRuleTool() {

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
	if (m_pRuleBuilder != NULL)
	{
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
	if (m_pRuleBuilder != NULL)
	{
		delete m_pRuleBuilder;
		m_pRuleBuilder = NULL;
	}
}

//endregion