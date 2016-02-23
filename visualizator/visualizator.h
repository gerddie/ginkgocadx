/*
 *  Ginkgo CADx Project
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


#pragma once
#include <prvext/prvext.h>

#include <wx/dynlib.h>
#include <api/imodulo.h>

class GNKVisualizatorExtension : public PrivateExtension
{
public :
	GNKVisualizatorExtension();
	~GNKVisualizatorExtension();
	virtual PrivateExtension::TListControllers InitializeLibrary(GNC::GCS::IEntorno* pEntorno);
	virtual bool IsActive();
	virtual void ShowActivationDialog(wxWindow* pParent);
};

PrivateExtension* init_VisualizatorExtension();
