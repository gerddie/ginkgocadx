/*
 *  
 *  $Id: icontroladorvistas.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "icontroladorvistas.h"
#include <main/controllers/controladorvistas.h>


GNC::GCS::IControladorVistas::IControladorVistas()
{
}

GNC::GCS::IControladorVistas::~IControladorVistas()
{
}

GNC::GCS::IControladorVistas* GNC::GCS::IControladorVistas::Instance()
{
	return GNC::GCS::ControladorVistas::Instance();
}

void GNC::GCS::IControladorVistas::FreeInstance()
{
	GNC::GCS::ControladorVistas::FreeInstance();
}
