/*
 *  
 *  $Id: visualizator.cpp $
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

