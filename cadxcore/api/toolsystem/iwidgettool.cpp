/*
*  
*  $Id: iwidgettool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include <iostream>

#include "iwidgettool.h"
#include "itoolcontroller.h"
#include <api/controllers/icontroladorlog.h>

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/wxginkgotoolbar.h>

GNC::GCS::IWidgetTool::IWidgetTool(const ITool::UID& uid, const ITool::TToolFamily& uidFamilia, const std::string& nombre, const wxBitmap& icon, const int subFamilia, const int& prioridad): GNC::GCS::ITool(uid, uidFamilia, nombre, icon, subFamilia, prioridad)
{
}

GNC::GCS::IWidgetTool::~IWidgetTool()
{
}

bool GNC::GCS::IWidgetTool::ExecuteAction()
{
	//none
	LOG_ERROR("IWidgetTool","trying to execute a widget tool");
	return false;
}

void GNC::GCS::IWidgetTool::AppendToolIn(wxGinkgoToolBar* pParent)
{
	if (AppendsInToolBar()) {
		pParent->AddTool(this->ID, wxString::FromUTF8(Name.c_str()), Icon, wxString::FromUTF8(Name.c_str()), wxITEM_NORMAL);
		pParent->ForceSetToolDropDown(this->ID, HasDropDownMenu());
		pParent->EnableTool(this->ID, IsEnabled());
	}
}


