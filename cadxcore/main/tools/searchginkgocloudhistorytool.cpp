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

#include "searchginkgocloudhistorytool.h"

#include <api/controllers/icontroladorpermisos.h>
#include <main/gui/ginkgocloud/ginkgocloudsearchdialog.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>


#include <api/internationalization/internationalization.h>

//----------------------------------------------------------------------

GNC::SearchGinkgoCloudHistoryTool::SearchGinkgoCloudHistoryTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Q_R, _Std("Search Ginkgo Cloud"), _Std("Search Ginkgo Cloud"), GinkgoResourcesManager::ToolIcons::GetIcoSearchCloud(),GinkgoResourcesManager::BigIcons::GetIcoBigGinkgoCloudSearch()) 
{
}

GNC::SearchGinkgoCloudHistoryTool::~SearchGinkgoCloudHistoryTool()
{
}

bool GNC::SearchGinkgoCloudHistoryTool::Enabled() 
{
	return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && GNC::GCS::IControladorPermisos::Instance()->Get("ginkgocloud","download");
}

void GNC::SearchGinkgoCloudHistoryTool::Execute()
{
	GNC::GUI::GinkgoCloudSearchDialog::Instance()->Show();
}



