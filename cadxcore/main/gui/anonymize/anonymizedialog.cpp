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

#include "anonymizedialog.h"
#include "anonymizepanel.h"
#include <api/dicom/idicommanager.h>
#include <main/entorno.h>
#include <api/ientorno.h>
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/ipacscontroller.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/historycontroller.h>
#include <commands/comandoexportacion.h>
#include <wx/ginkgostyle/ginkgostyle.h>

GNC::GUI::AnonymizeDialog::AnonymizeDialog(wxWindow* pParent, const std::list<long>& seriesPk): GNC::GUI::AnonymizeDialogBase(pParent)
{
	m_pDatosPersistentes = new TipoWizardExportacion();
	m_pDatosPersistentes->ListOfSeriesPks = seriesPk;

	m_pAnonymizePanel = new AnonymizePanel(m_pBody, seriesPk);
	m_pBodySizer->Add(m_pAnonymizePanel, 1, wxEXPAND);
	m_pBody->Layout();
	Layout();
}

GNC::GUI::AnonymizeDialog::~AnonymizeDialog()
{
	
}

void GNC::GUI::AnonymizeDialog::OnOverwriteClick(wxCommandEvent &)
{
	LaunchCommand(true);
}

void GNC::GUI::AnonymizeDialog::OnCreateNewClick(wxCommandEvent &)
{
	LaunchCommand(false);
}

void GNC::GUI::AnonymizeDialog::LaunchCommand(bool overwrite)
{
	if (overwrite) {
		m_pDatosPersistentes->m_target = GNC::GUI::TipoWizardExportacion::OVERWRITE;
	} else {
		m_pDatosPersistentes->m_target = GNC::GUI::TipoWizardExportacion::NEW_STUDY;
	}
	m_pAnonymizePanel->GetTagsAnonymized(m_pDatosPersistentes->m_base);
	m_pDatosPersistentes->m_incluirTagsGinkgo = !m_pAnonymizePanel->AnonymizePrivateTags();
	
	GADAPI::ComandoExportacionParams* pParams = new GADAPI::ComandoExportacionParams(m_pDatosPersistentes);
	GADAPI::ComandoExportacion* pCmd = new GADAPI::ComandoExportacion(pParams);
	GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Exporting images ..."), pCmd, NULL);
	this->Close();
}

