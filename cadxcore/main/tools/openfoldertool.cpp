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


#include "openfoldertool.h"
#include <api/internationalization/internationalization.h>

#if defined(ENABLE_OPENFOLDERTOOL)

#include <main/gui/open/abrir.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/controllers/controladorpermisos.h>

//----------------------------------------------------------------------

GNC::OpenFolderTool::OpenFolderTool(): GNC::GCS::IHistoryTool(ID,GNC::GCS::IHistoryTool::TFamily_Open, _Std("Open folder"), _Std("Open folder")+ " \tAlt+o", GinkgoResourcesManager::MenuIcons::GetIcoOpenDir())
{
}

GNC::OpenFolderTool::~OpenFolderTool()
{
}

bool GNC::OpenFolderTool::Enabled()
{
        return GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire");
}

void GNC::OpenFolderTool::Execute()
{
        GNC::GUI::Abrir::AbrirDirectorio(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow());
}



#endif
