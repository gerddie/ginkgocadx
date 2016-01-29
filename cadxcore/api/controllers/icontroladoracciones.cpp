/*
 *  
 *  $Id: icontroladoracciones.cpp 5053 2012-07-26 07:38:15Z tovar $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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
