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


#include "synchronizedialog.h"
#include <main/controllers/commandcontroller.h>
#include <commands/synchronizationcommand.h>
#include <main/controllers/hangingprotocolcontroller.h>


GNC::GUI::SynchronizeDialog::SynchronizeDialog(wxWindow* parent, const std::list<long>& seriesPk, GNC::GCS::IHistoryTool* pTool): GNC::GUI::SynchronizeDialogBase(parent),
        m_seriesPk(seriesPk),
        m_pTool(pTool),
        m_pCtrlMod(NULL),
        m_mode(-1),
        m_pHangingProtocol(NULL),
        m_isToolMode(true)
{
}

GNC::GUI::SynchronizeDialog::SynchronizeDialog(wxWindow* parent, const std::list<long>& seriesPk, GNC::GCS::IModuleController* pCtrlMod, int mode, const GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& pHangingProtocol): GNC::GUI::SynchronizeDialogBase(parent),
        m_seriesPk(seriesPk),
        m_pTool(NULL),
        m_pCtrlMod(pCtrlMod),
        m_mode(mode),
        m_pHangingProtocol(pHangingProtocol),
        m_isToolMode(false)
{
}

GNC::GUI::SynchronizeDialog::~SynchronizeDialog()
{
}

void GNC::GUI::SynchronizeDialog::OnSynchronize(wxCommandEvent &/*evt*/)
{
        if (m_isToolMode) {
                GADAPI::SynchronizationCommandParameters* pParams = new GADAPI::SynchronizationCommandParameters(m_seriesPk, m_pTool);
                GADAPI::SynchronizationCommand* pCmd = new GADAPI::SynchronizationCommand(pParams);
                GNC::GCS::ICommandController::Instance()->ProcessAsync("syncronizing...", pCmd, NULL);
        } else {
                GADAPI::SynchronizationCommandParameters* pParams = new GADAPI::SynchronizationCommandParameters(m_seriesPk, m_pCtrlMod, m_mode, m_pHangingProtocol);
                GADAPI::SynchronizationCommand* pCmd = new GADAPI::SynchronizationCommand(pParams);
                GNC::GCS::ICommandController::Instance()->ProcessAsync("syncronizing...", pCmd, NULL);
        }

        EndModal(wxOK);
}
