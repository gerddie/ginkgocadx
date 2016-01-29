/*
 *  
 *  $Id: icontroladorpermisos.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include "icontroladorpermisos.h"
#include <main/controllers/controladorpermisos.h>

GNC::GCS::ControladorPermisosException::ControladorPermisosException(const std::string& msg, bool fatal) throw() : GNC::GCS::IException(msg, "GNC", fatal)
{
}				


GNC::GCS::IControladorPermisos::IControladorPermisos()
{
}

GNC::GCS::IControladorPermisos::~IControladorPermisos()
{
}
GNC::GCS::IControladorPermisos* GNC::GCS::IControladorPermisos::Instance()
{
	return GNC::ControladorPermisos::Instance();
}

void GNC::GCS::IControladorPermisos::FreeInstance()
{
	GNC::ControladorPermisos::FreeInstance();

}
