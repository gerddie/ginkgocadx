/*
*
*  $Id: droptargetmainwindow.cpp $
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

#include <wx/dir.h>
#include <api/globals.h>
#include <api/internationalization/internationalization.h>

#include "droptargetmainwindow.h"
#include <commands/comandoincluirhistorial.h>
#include <main/controllers/commandcontroller.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/entorno.h>

GNC::GUI::DropTargetMainWindow::DropTargetMainWindow():wxFileDropTarget()
{
}
GNC::GUI::DropTargetMainWindow::~DropTargetMainWindow()
{
}
/**
When a file is dropped on the window, it will be processes as it was passed as first parameter to Ginkgo CADx
*/
bool GNC::GUI::DropTargetMainWindow::OnDropFiles(wxCoord /*x*/, wxCoord /*y*/, const wxArrayString& filenames)
{
	std::list<std::string> listaPaths;
	for(wxArrayString::const_iterator it = filenames.begin(); it!= filenames.end(); ++it)
	{
		GNC::Entorno::Instance()->GetVentanaPrincipal()->EjecutarParametro(*it, false);
	}
	return true;
}