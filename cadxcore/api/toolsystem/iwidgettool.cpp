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


