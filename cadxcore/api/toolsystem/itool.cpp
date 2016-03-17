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

#include "itool.h"
#include "itoolcontroller.h"
#include <api/toolsystem/itoolsregistry.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/wxginkgotoolbar.h>

//revisado
void GNC::GCS::ITool::RegisterTool()
{
        GNC::GCS::IToolsRegistry::Instance()->RegisterTool(this);
}

/** Sets the Owner module of the tool (the module providing the tool). NULL if CADxCore **/
void GNC::GCS::ITool::SetOwnerModule(GNC::GCS::IModuleController* pOwner)
{
        OwnerModule = pOwner;
}

/** Gets the Owner module of the tool (the module providing the tool). NULL if CADxCore **/
GNC::GCS::IModuleController* GNC::GCS::ITool::GetOwnerModule()
{
        return OwnerModule;
}

GNC::GCS::ITool::ITool(const ITool::UID& uid, const ITool::TToolFamily& uidFamilia, const std::string& nombre, const wxBitmap& icon, const int subFamilia, const int& prioridad) :
        ID(uid),
        FamilyId(uidFamilia),
        SubFamilyId(subFamilia),
        Priority(prioridad),
        Name(nombre),
        Icon(icon),
        m_pParent(NULL),
        OwnerModule(NULL),
        KeyCode(0),
        Enabled(true)
{
}

GNC::GCS::ITool::~ITool()
{
}

void GNC::GCS::ITool::AppendToolIn(wxGinkgoToolBar* pParent)
{
        if (AppendsInToolBar()) {
                if (HasDropDownMenu()) {
                        pParent->AddTool(this->ID, wxString::FromUTF8(Name.c_str()), Icon, wxString::FromUTF8(Name.c_str()), wxITEM_NORMAL);
                        pParent->ForceSetToolDropDown(this->ID, true);
                } else {
                        pParent->AddTool(this->ID, wxString::FromUTF8(Name.c_str()), Icon, wxString::FromUTF8(Name.c_str()), wxITEM_CHECK);
                        pParent->ForceSetToolDropDown(this->ID, false);
                }
                pParent->EnableTool(this->ID, IsEnabled());
        }
}

void GNC::GCS::ITool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
        if (AppendsInMenu()) {
                GNC::GUI::wxDefaultEvtHandlerTool* evtHandler = new GNC::GUI::wxDefaultEvtHandlerTool(pParent, this);
                pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
        }
}

void GNC::GCS::ITool::AppendDropDownMenu(wxEvtHandler* /*pParent*/, wxMenu* /*pMenu*/)
{
}

void GNC::GCS::ITool::SetToolBar(GNC::GCS::IToolController* pParent)
{
        m_pParent = pParent;
}

GNC::GCS::IToolController* GNC::GCS::ITool::GetToolBar()
{
        return m_pParent;
}

void GNC::GCS::ITool::ConnectKeyEvents(wxWindow* /*pParent*/)
{
        //implement in child
}

int GNC::GCS::ITool::GetKeyCode()
{
        return KeyCode;
}

bool GNC::GCS::ITool::IsEnabled()
{
        return Enabled;
}

/*If a tool is not active it wont be attached to any menu*/
bool GNC::GCS::ITool::IsReady()
{
        return true;
}

void GNC::GCS::ITool::Enable(bool enabled)
{
        Enabled = enabled;
}
