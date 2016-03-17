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

#include "icontroladoracciones.h"
#include <string>
#include <api/autoptr.h>
#include <main/controllers/controladoracciones.h>

GNC::GCS::Accion::Accion(const std::string& nombre, bool puedeRehacerse, const int peso) : m_nombre(nombre), m_peso(peso), m_puedeRehacerse(puedeRehacerse)
{
}

GNC::GCS::Accion::~Accion()
{
}

const std::string& GNC::GCS::Accion::GetNombre()
{
        return m_nombre;
}

bool GNC::GCS::Accion::PuedeRehacer()
{
        return m_puedeRehacerse;
}

void GNC::GCS::Accion::SetPuedeRehacerse(bool puedeRehacerse)
{
        m_puedeRehacerse = puedeRehacerse;
}

void GNC::GCS::Accion::Hacer()
{
}

GNC::GCS::IControladorAcciones::IControladorAcciones()
{
}

GNC::GCS::IControladorAcciones::~IControladorAcciones()
{
}

GNC::GCS::IControladorAcciones* GNC::GCS::IControladorAcciones::Instance()
{
        return GNC::GCS::ControladorAcciones::Instance();
}

void GNC::GCS::IControladorAcciones::FreeInstance()
{
        GNC::GCS::ControladorAcciones::FreeInstance();
}
