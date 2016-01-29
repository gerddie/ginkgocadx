/*
*
*  $Id: historytoolscontroller.cpp $
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

#include "historytoolscontroller.h"
#include <main/controllers/controladorlog.h>
#include <main/gui/history3/historypanel3.h>

#include <wx/event.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/dc.h>
#include <wx/wxginkgotoolbar.h>

GNC::HistoryToolsController::HistoryToolsController()
{
}

GNC::HistoryToolsController::~HistoryToolsController()
{
}

GNC::HistoryToolsController* GNC::HistoryToolsController::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new GNC::HistoryToolsController();
	}
	return m_pInstance;
}

void GNC::HistoryToolsController::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

GNC::HistoryToolsController* GNC::HistoryToolsController::m_pInstance = 0;

void GNC::HistoryToolsController::RegisterAll()
{
	for (ToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) {
		;
	}
}

void GNC::HistoryToolsController::UnRegisterAll()
{
	for (ToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) {
		delete (*it).second;
	}
	m_tools.clear();
}

/* Registra una herramienta */
void GNC::HistoryToolsController::RegisterTool(GNC::GCS::IHistoryTool* pTool)
{
	if (pTool == NULL) {
		LOG_ERROR("HistoryToolController", "null toool");
		return;
	}

	ToolMap::const_iterator it = m_tools.find(pTool->ID);

	if (it != m_tools.end()) {
		LOG_ERROR("HistoryToolController", "Duplicate tool " << pTool->ID);
		return;
	}
	m_tools[pTool->ID] = pTool;
}

/* Des-Registra una herramienta */
void GNC::HistoryToolsController::UnRegisterTool(GNC::GCS::IHistoryTool* pTool)
{
	//GTRACE("GNC::ControladorHerramientas::DesRegistrarHerramienta( Herramienta = " << pHerramienta << ")");
	for (ToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) {
		if ((*it).second == pTool) {
			delete (*it).second;
			m_tools.erase(it);
			break;
		}
	}
}

void GNC::HistoryToolsController::UnRegisterTool(GNC::GCS::IHistoryTool::UID id)
{
	//GTRACE("GNC::ControladorHerramientas::DesRegistrarHerramienta( Herramienta = " << pHerramienta << ")");
	for (ToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) {
		if ((*it).second->ID == id) {
			delete (*it).second;
			m_tools.erase(it);
			break;
		}
	}
}

GNC::GCS::IHistoryTool* GNC::HistoryToolsController::GetTool(GNC::GCS::IHistoryTool::UID id)
{
	GNC::GCS::IHistoryTool* pTool = NULL;
	ToolMap::iterator it = m_tools.find(id);
	if (it != m_tools.end()) {
		pTool = (*it).second;
	}
	return pTool;
}

GNC::GCS::IHistoryPanel* GNC::HistoryToolsController::GetHistoryPanel()
{
	return GNC::GUI::HistoryPanel3::Instance();
}

GNC::HistoryToolsController::ToolList GNC::HistoryToolsController::GetTools(){
	ToolList lista;
	for(ToolMap::iterator it = m_tools.begin(); it!= m_tools.end(); ++it){
		lista.push_back((*it).second);
	}
	return lista;
}

GNC::HistoryToolsController::ToolList GNC::HistoryToolsController::GetTools(GNC::GCS::IHistoryTool::TToolFamily family)
{
	ToolList lista;
	for(ToolMap::iterator it = m_tools.begin(); it!= m_tools.end(); ++it){
		if ((*it).second->Family == family) {
			lista.push_back((*it).second);
		}
	}
	return lista;
}

void GNC::HistoryToolsController::AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::IHistoryTool::TToolFamily family)
{
	GNC::HistoryToolsController::ToolList list = GetTools(family);
	for (GNC::HistoryToolsController::ToolList::const_iterator it = list.begin(); it != list.end(); ++it) {
		if ((*it)->Enabled()) {
			(*it)->AppendToolIn(pParent, pMenu);
		}
	}

}

void GNC::HistoryToolsController::AppendsToolInToolBar(wxGinkgoToolBar* pParent, GNC::GCS::IHistoryTool::TToolFamily family)
{
	GNC::HistoryToolsController::ToolList list = GetTools(family);
	for (GNC::HistoryToolsController::ToolList::const_iterator it = list.begin(); it != list.end(); ++it) {
		if ((*it)->Enabled()) {
			(*it)->AppendToolIn(pParent);
		}
	}
}

void GNC::HistoryToolsController::AppendsBigToolsInToolBar(wxGinkgoToolBar* pParent)
{
	for(ToolMap::iterator it = m_tools.begin(); it!= m_tools.end(); ++it){
		if ((*it).second->Enabled()) {
			(*it).second->AppendToolIn(pParent, true);
		}
	}
}


