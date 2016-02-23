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

#include "queryretrievehistorytool.h"
#include <api/controllers/icontroladorpermisos.h>

#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/gui/acquisition/dialogoadquisicion.h>

//----------------------------------------------------------------------

GNC::QueryRetrieveHistoryTool::QueryRetrieveHistoryTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Q_R, _Std("PACS Q/R"), _Std("Acquire a DICOM study from PACS")+ " \tAlt+a", GinkgoResourcesManager::Acquisition::GetIcoAdquirePACS(), GinkgoResourcesManager::BigIcons::GetIcoBigQR()) 
{
}

GNC::QueryRetrieveHistoryTool::~QueryRetrieveHistoryTool()
{
}

bool GNC::QueryRetrieveHistoryTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && 
		GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_acquisition");
}

void GNC::QueryRetrieveHistoryTool::Execute()
{
	GNC::GUI::DialogoAdquisicion::Instance()->Show();
}



