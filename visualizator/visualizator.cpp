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

#include <api/ientorno.h>
#include <api/internationalization/internationalization.h>
#include <api/controllers/icontroladorlog.h>

#include <visualizator/license.h>
#include "visualizator/controllers/visualizatorcontroller.h"

#include <resources/visualizatorresourcesmanager.h>

#include "visualizator.h"

#include <wx/msgdlg.h>

#if defined(_WIN32) && defined(_GINKGO_DEBUG) && !defined(_WIN32_WINNT_WIN7)
//#include <vld.h>
#endif

#if defined(MONOLITIC)
PrivateExtension* init_VisualizatorExtension()
{
	return new GNKVisualizatorExtension();
}
#else
#error UNSUPPORTED
IMPLEMENT_PrivateExtension(GNKVisualizatorExtension)
#endif

GNKVisualizatorExtension::GNKVisualizatorExtension() : PrivateExtension(EXT_SID, EXT_PROVIDER, EXT_DESCRIPTION, EXT_UPDATEURL)
{
}

GNKVisualizatorExtension::~GNKVisualizatorExtension()
{
}
PrivateExtension::TListControllers GNKVisualizatorExtension::InitializeLibrary(GNC::GCS::IEntorno* pEntorno)
{
	PrivateExtension::TListControllers list;
	list.push_back(new GNKVisualizator::ControladorAtencionPrimaria(pEntorno));
	return list;
}
bool GNKVisualizatorExtension::IsActive()
{
	return true;
}
void GNKVisualizatorExtension::ShowActivationDialog(wxWindow* /*pParent*/)
{
	wxMessageBox(_("Ginkgo CADx visualizator is Open Source"), _("Info"));
}

