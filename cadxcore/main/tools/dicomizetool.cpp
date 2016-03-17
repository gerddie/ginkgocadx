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

//#define _GINKGO_TRACE

#include "dicomizetool.h"

#include <api/controllers/icontroladorlog.h>
#include <api/controllers/icontroladorpermisos.h>

#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/gui/import/wxwizardimportacionginkgo.h>
#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::DicomizeTool::DicomizeTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Dicomize, _Std("Dicomize"), _Std("Dicomize files")+ " \tAlt+d", GinkgoResourcesManager::MenuIcons::GetIcoImportar(), GinkgoResourcesManager::BigIcons::GetIcoBigDicomize())
{
}

GNC::DicomizeTool::~DicomizeTool()
{
}

bool GNC::DicomizeTool::Enabled()
{
        return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","import");
}

void GNC::DicomizeTool::Execute()
{
        try {
                GNC::GUI::wxWizardImportacionGinkgo ib(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow(),NULL);

                ib.ShowModal();
        } catch(GIL::DICOM::I2DException& i) {
                LOG_ERROR("DicomizeTool", "Error in dicomization: " << i.GetCause())
        }
}



