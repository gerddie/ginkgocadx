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

//#define _GINKGO_TRACE
#include <iostream>
#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/controllers/imodulecontroller.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/ientorno.h>
#include <api/istudycontext.h>
#include <api/toolsystem/itoolcontroller.h>
#include <api/ientorno.h>
#include <eventos/views.h>
#include <main/controllers/controladoreventos.h>


#include "ivista.h"

GNC::GCS::VistaException::VistaException(const std::string& msg, const std::string& component, bool fatal) throw() : GNC::GCS::IException(msg ,component, fatal)
{
}

/* Constructor */
GNC::GCS::IVista::IVista(const GNC::GCS::Ptr<GNC::GCS::IStudyContext>& estudio) :
		Estudio(new GNC::GCS::Ptr<GNC::GCS::IStudyContext>(estudio) ),
		m_Titulo(_Std("Loading...")),
		m_toolController(GNC::GCS::IToolController::New()),
		m_Activada(false),
		m_Cargada(false),
		m_Guardada(true)
{
	(*Estudio)->View  = this;
	GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventView(this, GNC::GCS::Events::EventView::Creation));
}

/* Destructor */
GNC::GCS::IVista::~IVista()
{
	if (GNC::GCS::IEntorno::Instance()->GetVentanaPrincipal() != NULL) {
		GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventView(this, GNC::GCS::Events::EventView::Destruction));
	}
	delete m_toolController;
	GTRACE(">> IVista::~IVista() " << this)
	(*Estudio)->View = NULL;
	if (this->IsLocked()) {
		this->UnLock();
	}
	// Estudio is a pointer to GNC::GCS::Ptr to avoid circular dependencies.
	delete Estudio;
	GTRACE("<< IVista::~IVista()" << this)
}

std::string& GNC::GCS::IVista::GetTitulo()
{
	return m_Titulo;
}

bool GNC::GCS::IVista::SoportaHerramienta(GNC::GCS::ITool* )
{
	return true;
}

/* Notificacion de cambio en la configuracion */
void GNC::GCS::IVista::OnConfiguracionCambiada()
{

}

void GNC::GCS::IVista::Activar()
{
	m_Activada = true;
}

void GNC::GCS::IVista::Desactivar()
{
	m_Activada = false;
}

/* Obtiene el identificador unico del módulo al que pertenece */
const std::string& GNC::GCS::IVista::GetUIDModulo()
{
	return (*Estudio)->Module->GetUID();
}

/* Obtiene el modulo al que pertenece */
GNC::GCS::IModuleController* GNC::GCS::IVista::GetModulo()
{
	return (*Estudio)->Module;
}

GNC::GCS::Ptr<GNC::GCS::IStudyContext>& GNC::GCS::IVista::GetEstudio() {
	return (*Estudio);
}


bool GNC::GCS::IVista::EstaCargada()
{
	return m_Cargada;
}

bool GNC::GCS::IVista::EstaModificada()
{
	return false;
}

void GNC::GCS::IVista::SetCargada(bool cargada)
{
	m_Cargada = cargada;
}

bool GNC::GCS::IVista::Guardar()
{
	return true;
}

const std::list<std::string> GNC::GCS::IVista::GetImagePaths() const
{
	if ((*Estudio)) {
		return (*Estudio)->GetImagePaths();
	}
	else {
		throw VistaException("Vista no inicializada");
	}
}


/**Gets dicom is currently shown**/
std::string GNC::GCS::IVista::GetCurrentDICOM()
{
	if ((*Estudio)) {
		return std::string((*Estudio)->GetImagePath((*Estudio)->ActiveFileIndex));
	}
	else {
		throw VistaException("Vista no inicializada");
	}
}

GNC::GCS::IToolController* GNC::GCS::IVista::GetToolController()
{
	return m_toolController;
}

void GNC::GCS::IVista::OnFocus()
{
	if (!m_Activada) {
		GNC::GCS::IControladorVistas::Instance()->SolicitarActivarVista(this);
	}
}
