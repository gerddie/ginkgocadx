/*
 *  
 *  $Id: showhistorytool.cpp $
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
#include "showhistorytool.h"
#include <export/contracts/iviewcontract.h>
#include <api/ivista.h>
#include <api/controllers/icontroladorlog.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <main/entorno.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/gui/history3/historypanel3.h>

//----------------------------------------------------------------------

GNC::GCS::ITool* GNC::ShowHistoryTool::NewTool()
{
	return new GNC::ShowHistoryTool();
}

GNC::ShowHistoryTool::ShowHistoryTool()
{
}
GNC::ShowHistoryTool::~ShowHistoryTool()
{
}
		
bool GNC::ShowHistoryTool::ExecuteAction()
{	
	if (GNC::GUI::HistoryPanel3::Instance()->IsIconized()) {
		GNC::GUI::HistoryPanel3::Instance()->Restore();
	}
	if (!GNC::GUI::HistoryPanel3::Instance()->IsShown())
	{		
		GNC::GUI::HistoryPanel3::Instance()->Show(true);
	} else {
		GNC::GUI::HistoryPanel3::Instance()->Raise();
	}
	
	return true;
}

void GNC::ShowHistoryTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		GNC::GUI::wxDefaultEvtHandlerTool* evtHandler = new GNC::GUI::wxDefaultEvtHandlerTool(pParent, this);
		pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
	}
}

bool GNC::ShowHistoryTool::IsShown()
{
	return GNC::GUI::HistoryPanel3::Instance()->IsShown();
}
