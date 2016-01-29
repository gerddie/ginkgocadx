/*
 *  
 *  $Id: visualizator.h $
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
