/*
 *  
 *  $Id: synchronizedialog.h $
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