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


#include "addfiletohistorytool.h"
#include <export/contracts/iviewcontract.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/gui/open/abrir.h>
#include <commands/openremovableunit.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/gui/ginkgocloud/openginkgoclouddialog.h>



#define OPEN_FILE  1866
#define OPEN_FOLDER  1867
#define OPEN_REMOVABLE  1868
#define OPEN_GINKGO_CLOUD  1869

namespace GNC
{
namespace GUI
{
class wxEventHandlerOpenStudy : public wxEvtHandler
{
public:
        wxEventHandlerOpenStudy(wxEvtHandler* pParent, GNC::AddFileToHistory* pHerramienta):wxEvtHandler()
        {
                m_pEvtParent = pParent;
                m_pHerramienta=pHerramienta;

                m_pEvtParent->Connect(OPEN_FILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerOpenStudy::OnOpenFile),NULL,this);
                m_pEvtParent->Connect(OPEN_FOLDER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerOpenStudy::OnOpenFolder),NULL,this);
                m_pEvtParent->Connect(OPEN_REMOVABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerOpenStudy::OnOpenRemovable),NULL,this);
                m_pEvtParent->Connect(OPEN_GINKGO_CLOUD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerOpenStudy::OnOpenGinkgoCloud),NULL,this);
        }

        ~wxEventHandlerOpenStudy()
        {

                m_pEvtParent->Disconnect(OPEN_FILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerOpenStudy::OnOpenFile),NULL,this);
                m_pEvtParent->Disconnect(OPEN_FOLDER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerOpenStudy::OnOpenFolder),NULL,this);
                m_pEvtParent->Disconnect(OPEN_REMOVABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerOpenStudy::OnOpenRemovable),NULL,this);
                m_pEvtParent->Disconnect(OPEN_GINKGO_CLOUD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerOpenStudy::OnOpenGinkgoCloud),NULL,this);
                m_pEvtParent = NULL;
                m_pHerramienta = NULL;
        }

        void OnOpenFile( wxCommandEvent& event )
        {
                m_pHerramienta->OpenFile();
                event.Skip(false);
        }

        void OnOpenFolder( wxCommandEvent& event )
        {
                m_pHerramienta->OpenFolder();
                event.Skip(false);
        }

        void OnOpenRemovable( wxCommandEvent& event )
        {
                m_pHerramienta->OpenRemovable();
                event.Skip(false);
        }

        void OnOpenGinkgoCloud( wxCommandEvent& event )
        {
                m_pHerramienta->OpenGinkgoCloud();
                event.Skip(false);
        }

        GNC::AddFileToHistory* m_pHerramienta;
        wxEvtHandler* m_pEvtParent;
};
}
}

GNC::GCS::ITool* GNC::AddFileToHistory::NewTool()
{
        return new GNC::AddFileToHistory();
}

GNC::AddFileToHistory::AddFileToHistory()
{
        Enable(GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire"));
}
GNC::AddFileToHistory::~AddFileToHistory()
{
}

bool GNC::AddFileToHistory::ExecuteAction()
{
        return false;
}

bool GNC::AddFileToHistory::HasDropDownMenu()
{
        return true;
}

void GNC::AddFileToHistory::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
        if (pMenu->GetMenuItemCount() > 0 && !pMenu->GetMenuItems().back()->IsSeparator()) {
                pMenu->AppendSeparator();
        }
        pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, OPEN_FILE, wxString(_("Open file")) + wxT(" \tAlt+f"), GinkgoResourcesManager::MenuIcons::GetIcoOpenFile(), new GNC::GUI::wxEventHandlerOpenStudy( pParent, this)));
        pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, OPEN_FOLDER, wxString(_("Open folder")) + wxT(" \tAlt+d"), GinkgoResourcesManager::MenuIcons::GetIcoOpenDir(), new GNC::GUI::wxEventHandlerOpenStudy( pParent, this)));
        pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, OPEN_REMOVABLE, _("Open removable unit"), GinkgoResourcesManager::MenuIcons::GetIcoRemovableUnit(), new GNC::GUI::wxEventHandlerOpenStudy( pParent, this)));
        pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, OPEN_GINKGO_CLOUD, _("Open Ginkgo Cloud link"), GinkgoResourcesManager::ToolIcons::GetIcoDownloadCloud(), new GNC::GUI::wxEventHandlerOpenStudy( pParent, this)));
}

void GNC::AddFileToHistory::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
        if (AppendsInMenu()) {
                AppendDropDownMenu(pParent, pMenu);
        }
}

void GNC::AddFileToHistory::OpenFile()
{
        GNC::GUI::Abrir::AbrirFichero(ViewContract->GetView()->GetWindow());

}

void GNC::AddFileToHistory::OpenFolder()
{
        GNC::GUI::Abrir::AbrirDirectorio(ViewContract->GetView()->GetWindow());
}

void GNC::AddFileToHistory::OpenRemovable()
{
        GADAPI::OpenRemovableUnitCommandParams* pParams = new GADAPI::OpenRemovableUnitCommandParams(ViewContract->GetView()->GetWindow());
        GADAPI::OpenRemovableUnitCommand* pCmd = new GADAPI::OpenRemovableUnitCommand(pParams);
        GNC::GCS::ICommandController::Instance()->ProcessAsync("Listing units...", pCmd, NULL);
}

void GNC::AddFileToHistory::OpenGinkgoCloud()
{
        GNC::GUI::OpenGinkgoCloudDialog* dlg = new GNC::GUI::OpenGinkgoCloudDialog(ViewContract->GetView()->GetWindow());
        dlg->Show();
        dlg->Raise();
}
