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
