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

#include "pacsuploadtool.h"
#include <wx/dialog.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>

#include <api/internationalization/internationalization.h>
#include <export/contracts/iviewcontract.h>
#include <api/ivista.h>
#include <api/controllers/icontroladorlog.h>
#include <main/gui/pacsupload/pacsuploaddialog.h>
#include <main/entorno.h>
#include <api/istudycontext.h>
#include <commands/sendpacscommand.h>
#include <main/controllers/commandcontroller.h>

//----------------------------------------------------------------------

GNC::GCS::ITool* GNC::PACSUploadTool::NewTool()
{
        return new GNC::PACSUploadTool();
}

GNC::PACSUploadTool::PACSUploadTool()
{
}
GNC::PACSUploadTool::~PACSUploadTool()
{
}

bool GNC::PACSUploadTool::ExecuteAction()
{
        std::list<long> viewSeries;
        viewSeries.insert(viewSeries.begin(), ViewContract->GetView()->GetEstudio()->OpenedSeries.begin(), ViewContract->GetView()->GetEstudio()->OpenedSeries.end());

        GNC::GUI::PACSUploadDialog dlg(ViewContract->GetView()->GetWindow(), viewSeries);

        if (dlg.ShowModal() == wxID_OK) {
                std::list<long> listOfSeries;
                listOfSeries = dlg.GetSelectedSeries();
                if (!listOfSeries.empty()) {
                        GADAPI::SendPACSCommand* pCmd = new GADAPI::SendPACSCommand(dlg.GetSelectedServer(), "", listOfSeries);
                        GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Send to PACS..."),pCmd, NULL);
                }
        }

        return true;
}

void GNC::PACSUploadTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
        if (AppendsInMenu()) {
                GNC::GUI::wxDefaultEvtHandlerTool* evtHandler = new GNC::GUI::wxDefaultEvtHandlerTool(pParent, this);
                pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
        }
}

