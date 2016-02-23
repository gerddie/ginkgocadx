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
