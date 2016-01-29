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

GNC::RuleTool::~RuleTool() {

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
	if (m_pRuleBuilder != NULL)
	{
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
	if (RuleContract->GetViewerActivo() != NULL)
	{
		RuleContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	}
	if (RuleContract->GetManager() != NULL)
	{
		RuleContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	}
	if (m_pRuleBuilder != NULL)
	{
		delete m_pRuleBuilder;
		m_pRuleBuilder = NULL;
	}
}

//endregion
#endif