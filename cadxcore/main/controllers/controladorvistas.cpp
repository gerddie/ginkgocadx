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

#include <cmath>
#include <algorithm>

#include <wx/wx.h>
#include <wx/arrstr.h>
#include <wx/image.h>

#include "controladorvistas.h"


#include <api/ivista.h>
#include <eventos/modificacionfichero.h>

#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/entorno.h>

#include <main/controllers/commandcontroller.h>
#include <main/controllers/inactivitycontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/controladorpermisos.h>


#include <iostream>
#include <sstream>
#include <string>

#include <main/gui/mainwindow/detacheddialog.h>

//region "Patron singleton"

GNC::GCS::ControladorVistas* GNC::GCS::ControladorVistas::m_pInstance = 0;

GNC::GCS::ControladorVistas::ControladorVistas()
{
	m_pVistaActiva   = NULL;

	//se conecta el evento Ginkgo
	GNC::GCS::Events::EventoModificacionFichero evtModificado;
	GNC::Entorno::Instance()->GetControladorEventos()->Registrar(this, evtModificado);
}

GNC::GCS::ControladorVistas::~ControladorVistas()
{
}

GNC::GCS::ControladorVistas* GNC::GCS::ControladorVistas::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new GNC::GCS::ControladorVistas();
	}
	return m_pInstance;
}

void GNC::GCS::ControladorVistas::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

//endregion


wxWindow* GNC::GCS::ControladorVistas::GetRootWindow()
{
	return GNC::Entorno::Instance()->GetVentanaPrincipal()->GetRootWindow();
}

GNC::GCS::IVista* GNC::GCS::ControladorVistas::ObtenerVistaRegistrada(wxWindow* pVentana)
{
	GNC::GCS::IVista* pVista = NULL;

	MapaVentanas::iterator it = m_MapaVentanas.find(pVentana);
	if (it != m_MapaVentanas.end()) {
		pVista = (*it).second;
	}

	return pVista;
}

void GNC::GCS::ControladorVistas::Freeze()
{
	VentanaPrincipal* v = GNC::Entorno::Instance()->GetVentanaPrincipal();
	v->SuperFreeze();
}

void GNC::GCS::ControladorVistas::Thaw()
{
	VentanaPrincipal* v = GNC::Entorno::Instance()->GetVentanaPrincipal();
	v->SuperThaw();
}

void GNC::GCS::ControladorVistas::SetTitulo(GNC::GCS::IVista* pVista, const std::string& tituloNuevo) {
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//
	wxWindow* pVentana = NULL;
	int numConteo = 0;
	bool cambia = true;

	pVentana = pVista->GetWindow();
	if (pVentana == NULL) {
		LOG_WARN("Core/ControladorVistas", "No se pudo obtener la ventana asociada a la vista. Inconsistencia interna");
		return;
	}
	else {
		// Buscamos el titulo actual que tiene asociado la vista compuesta, de haberlo.
		MapaTitulosRegistrados::iterator it = m_MapaTitulosRegistrados.find(pVista);
		if (it != m_MapaTitulosRegistrados.end()) { // Ya esta registrado.
			const std::string& tituloActual = (*it).second;
			if (tituloNuevo != tituloActual) { // El titulo ha cambiado

				// Buscamos el titulo antiguo del registro y lo borramos.

				MapaConteoTitulos::iterator it2 = m_MapaConteoTitulos.find(tituloActual);
				if (it2 != m_MapaConteoTitulos.end()) {
					MapaConteoRegistros& registrosConteo = (*it2).second;
					MapaConteoRegistros::iterator it3 = registrosConteo.find(pVista);
					if (it3 != registrosConteo.end()) {
						registrosConteo.erase(it3); // Eliminamos la entrada de la vista para ese titulo
						if (registrosConteo.empty()) { // Si no hay mas entradas de vista para ese titulo, eliminamos el registro de vistas asociadas a los titulos.
							m_MapaConteoTitulos.erase(it2);
						}
					}
					else {
						LOG_WARN("Core/ControladorVistas", "Se ha detectado una inconsistencia interna en el registro de conteos de titulos.");
					}
				}
				else {
					LOG_WARN("Core/ControladorVistas", "Se ha detectado una inconsistencia interna en el registro de titulos de ventanas.");
				}

				m_MapaTitulosRegistrados.erase(it); // Borramos el registro de titulo de la vista
			}
			else { // El titulo no cambia
				cambia = false;
			}
		}
	}

	if (cambia) {

		// Buscamos el titulo nuevo que tiene asociado la vista compuesta, de haberlo.
		MapaTitulosRegistrados::iterator it = m_MapaTitulosRegistrados.find(pVista);
		if (it == m_MapaTitulosRegistrados.end()) { // No esta registrado.
			m_MapaTitulosRegistrados[pVista] = tituloNuevo;
		}

		MapaConteoTitulos::iterator it2 = m_MapaConteoTitulos.find(tituloNuevo);
		if (it2 != m_MapaConteoTitulos.end()) { // Ya hay ventanas registradas con este titulo
			MapaConteoRegistros& registrosConteo = (*it2).second;
			MapaConteoRegistros::iterator it3 = registrosConteo.find(pVista);
			if (it3 != registrosConteo.end()) {
				LOG_WARN("Core/ControladorVistas", "Entrada duplicada en el registro de conteos de titulos. Inconsistencia interna");
				registrosConteo.erase(it3);
			}
			for (MapaConteoRegistros::iterator it3 = registrosConteo.begin(); it3 != registrosConteo.end(); ++it3)
			{
				const int num = (*it3).second + 1;
				numConteo = numConteo > num ? numConteo : num;
			}
			registrosConteo[pVista] = numConteo;
		}
		else {
			MapaConteoRegistros registroConteo;
			registroConteo[pVista] = 0;
			m_MapaConteoTitulos[tituloNuevo] = registroConteo;
		}
	}

	std::ostringstream os;
	if (pVista->SoportaGuardar() && pVista->EstaModificada()) {
		os << "* ";
	}
	os << tituloNuevo;
	if (numConteo > 0) {
		os << " (" << numConteo + 1 << ")";
	}

	wxAuiNotebook* nb = dynamic_cast<wxAuiNotebook*> (pVentana->GetParent());
	if (nb != NULL) {
		int idx = nb->GetPageIndex(pVentana);
		if (idx != wxNOT_FOUND) {
			nb->SetPageText(idx, wxString::FromUTF8(os.str().c_str()));
		}
		else {
			LOG_WARN("Core/ControladorVistas", "No se encontro el tab asociado. Inconsistencia interna");
		}
	} else {
		
		GNC::GUI::DetachedDialog* dd = dynamic_cast<GNC::GUI::DetachedDialog*> (pVentana->GetParent());
		if(dd != NULL)
		{
			dd->SetTitulo(wxString::FromUTF8(os.str().c_str()));
		}
		else {
			//it's startup view
			//LOG_WARN("Core/ControladorVistas", "No se encontro el panel o dialogo desencajado asociado a la vista. Inconsistencia interna");
		}
	}

}

std::string GNC::GCS::ControladorVistas::GetTitulo(GNC::GCS::IVista* pVista)
{
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	wxWindow* pVentana = NULL;

	pVentana = pVista->GetWindow();
	if (pVentana == NULL) {
		LOG_WARN("Core/ControladorVistas", "No se pudo obtener la ventana asociada a la vista. Inconsistencia interna");
		return std::string("");
	}

	wxAuiNotebook* nb = dynamic_cast<wxAuiNotebook*> (pVentana->GetParent());
	if (nb != NULL) {
		int idx = nb->GetPageIndex(pVentana);
		if (idx != wxNOT_FOUND) {
			return std::string(nb->GetPageText(idx).ToUTF8());
		}
		else {
			LOG_WARN("Core/ControladorVistas", "No se pudo obtener el tab asociado a la vista. Inconsistencia interna");
			return std::string("");
		}
	}
	else {
		GNC::GUI::DetachedDialog* dd = dynamic_cast<GNC::GUI::DetachedDialog*> (pVentana->GetParent());
		if(dd!=NULL)
		{
			return std::string(dd->GetTitulo().ToUTF8());
		}
		else {
			LOG_WARN("Core/ControladorVistas", "No se encontro el panel o dialogo desencajado asociado a la vista. Inconsistencia interna");
			return std::string("");
		}
	}
}

void GNC::GCS::ControladorVistas::PropagarConfiguracionCambiada()
{
	GNC::GCS::ILocker lock(this);
	for (MapaVentanas::iterator it = m_MapaVentanas.begin(); it != m_MapaVentanas.end(); ++it) {
		GNC::GCS::IVista* pVista = (*it).second;
		pVista->OnConfiguracionCambiada();
	}
}

void GNC::GCS::ControladorVistas::Registrar(GNC::GCS::IVista* pVista)
{
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	GNC::GCS::ILocker lock(this);
	if (pVista == NULL) {
		LOG_WARN("Core/ControladorVistas", "No se puede registrar una vista nula");
		return;
	}
	wxWindow* pVentana = pVista->GetWindow();
	if (pVentana == NULL) {
		LOG_WARN("Core/ControladorVistas", "No se puede registrar una vista con ventana nula");
		return;
	}

	MapaVentanas::iterator it = m_MapaVentanas.find(pVentana);
	if (it != m_MapaVentanas.end()) {
		LOG_WARN("Core/ControladorVistas", "La vista ya estaba registrada. Posible inconsistencia interna");
	}
	else {
		m_MapaVentanas[pVentana] = pVista;
	}
	GNC::Entorno::Instance()->GetVentanaPrincipal()->InsertarVentana(pVentana);
}

void GNC::GCS::ControladorVistas::Destruir(GNC::GCS::IVista* pVista)
{
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	Lock();
	GTRACE("ControladorVistas::DestruirVista(GNC::GCS::IVista* pVista = " << pVista << ")");

	if (pVista != NULL) {
		wxWindow* pVentana = pVista->GetWindow();
		MapaVentanas::iterator it = m_MapaVentanas.find(pVentana);
		if (it != m_MapaVentanas.end()) {
			m_MapaVentanas.erase(it);
			PropagarVistaDestruida(pVista);
		}
		else {
			LOG_WARN("Core/ControladorVistas", "Error al destruir vista: La ventana asociada a la vista no estaba registrada. Posible inconsistencia interna");
		}
	}
	UnLock();
}


void GNC::GCS::ControladorVistas::Destruir(wxWindow* pVentana)
{

    if (pVentana)
        pVentana->Hide();

	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	Lock();
	GTRACE("ControladorVistas::DestruirVista(wxWindow* pVentana = " << pVentana << ")");

	if (pVentana != NULL) {

		MapaVentanas::iterator it = m_MapaVentanas.find(pVentana);
		if (it != m_MapaVentanas.end()) {
			GNC::GCS::IVista* pVista = (*it).second;
			m_MapaVentanas.erase(it);
			PropagarVistaDestruida(pVista);
		}
		else {
			//LOG_WARN("Core/ControladorVistas", "Error al destruir ventana: La ventana no estaba registrada. Posible inconsistencia interna");
		}
	}
	UnLock();
}


//region "Gestion de carga y descarga de vistas"

void GNC::GCS::ControladorVistas::SolicitarActivarVista(GNC::GCS::IVista* pVista) {
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	if (pVista == m_pVistaActiva) {
		return;
	}

	Freeze();

	if (m_pVistaActiva != NULL) {
		m_pVistaActiva->Desactivar();
		m_pVistaActiva->GetWindow()->Refresh();
	}

	m_pVistaActiva = pVista;

	GTRACE("GNC::GCS::ControladorVistas::SolicitarActivarVista(" << pVista << ")");


	if (pVista != NULL) {
		pVista->Activar();
		m_pVistaActiva->GetWindow()->Refresh();
	}

	GNC::Entorno* pEntorno = GNC::Entorno::Instance();
	Entorno::ViewsObserversList::const_iterator it;
	for (it = pEntorno->ViewsObservers.begin(); it != pEntorno->ViewsObservers.end(); ++it) {
		IObservadorVistas* oe = *it;
		oe->OnVistaActivada(pVista);
	}

	Thaw();
}


//endregion

//region "Propagación de eventos"

void GNC::GCS::ControladorVistas::PropagarVistaCreada(GNC::GCS::IVista* pVista)
{
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	GNC::Entorno* pEntorno = GNC::Entorno::Instance();
	Entorno::ViewsObserversList::const_iterator it;
	for (it = pEntorno->ViewsObservers.begin(); it != pEntorno->ViewsObservers.end(); ++it) {
		IObservadorVistas* ov = *it;
		ov->OnVistaCreada(pVista);
	}
}

inline bool my_isnan(double x)
{
	return x != x;
}

void GNC::GCS::ControladorVistas::PropagarVistaDestruida(GNC::GCS::IVista* pVista)
{
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	if (pVista == NULL) {
		LOG_WARN("Core/ControladorVistas", "Error al propagar destruccion de vista. Vista nula. Inconsistencia interna");
		return;
	}

	GNC::Entorno* pEntorno = GNC::Entorno::Instance();

	MapaTitulosRegistrados::iterator it = m_MapaTitulosRegistrados.find(pVista);
	if (it != m_MapaTitulosRegistrados.end()) { // Esta registrado.
		const std::string& titulo = (*it).second;
		MapaConteoTitulos::iterator it2 = m_MapaConteoTitulos.find(titulo);
		if (it2 != m_MapaConteoTitulos.end()) {
			MapaConteoRegistros& registrosConteo = (*it2).second;
			MapaConteoRegistros::iterator it3 = registrosConteo.find(pVista);
			if (it3 != registrosConteo.end()) {
				registrosConteo.erase(it3); // Eliminamos la entrada de la vista para ese titulo
				if (registrosConteo.empty()) { // Si no hay mas entradas de vista para ese titulo, eliminamos el registro de vistas asociadas a los titulos.
					m_MapaConteoTitulos.erase(it2);
				}
			}
			else {
				LOG_WARN("Core/ControladorVistas", "Se ha detectado una inconsistencia interna en el registro de conteos de titulos.");
			}
		}
		else {
			LOG_WARN("Core/ControladorVistas", "Se ha detectado una inconsistencia interna en el registro de titulos de ventanas.");
		}
		m_MapaTitulosRegistrados.erase(it); // Borramos el registro de titulo de la vista
	}
	else {
		LOG_WARN("Core/ControladorVistas", "Destruccion de vista sin titulo registrado.");
	}

	Entorno::ViewsObserversList::const_iterator ito;
	for (ito = pEntorno->ViewsObservers.begin(); ito != pEntorno->ViewsObservers.end(); ++ito) {
		IObservadorVistas* ov = *ito;
		ov->OnVistaDestruida(pVista);
	}

	//se desactiva la vista y se desreferencia si es la activa
	pVista->Desactivar();
	if (pVista == m_pVistaActiva)
	{
		SolicitarActivarVista(NULL);
	}
}

//endregion

//region "Getters de vistas activas"
GNC::GCS::IVista* GNC::GCS::ControladorVistas::GetVistaActiva() {
	return m_pVistaActiva;
}
//endregion

//region "Gestión de paneles extra"

void GNC::GCS::ControladorVistas::MostrarPanelFlotante(wxWindow* win, int x, int y)
{
	GTRACE("ControladorVistas::MostrarPanel(" << win << ")")
	VentanaPrincipal* vp = GNC::Entorno::Instance()->GetVentanaPrincipal();
	if (vp == NULL) {
		throw std::exception();
	}
	vp->Freeze();
	//std::cout << "Mostrando panel " << win << std::endl;
	vp->m_mgr.GetPane(win).Show(true);
	//se muestra abajo
	wxAuiPaneInfo& pi = vp->m_mgr.GetPane(win);
	if (!pi.IsFloating()) {
		pi.Float();
	}
	pi.FloatingSize(pi.best_size);
	pi.FloatingPosition(x,y);
	vp->m_mgr.Update();
	vp->Layout();
	vp->Thaw();
}

bool GNC::GCS::ControladorVistas::EsVisible(wxWindow* win)
{
	VentanaPrincipal* vp = GNC::Entorno::Instance()->GetVentanaPrincipal();
	if (vp == NULL) {
		throw std::exception();
	}
	wxAuiPaneInfo pi = vp->m_mgr.GetPane(win);
	return pi.IsShown();
}

void GNC::GCS::ControladorVistas::OcultarPanel(wxWindow* win)
{
	GTRACE("ControladorVistas::OcultarPanel(" << win << ")")
	VentanaPrincipal* vp = GNC::Entorno::Instance()->GetVentanaPrincipal();
	if (vp == NULL) {
		throw std::exception();
	}
	vp->SuperFreeze();
	//std::cout << "Ocultando panel " << win << std::endl;
	vp->m_mgr.GetPane(win).Hide();
	vp->m_mgr.Update();
	vp->Layout();
	vp->SuperThaw();
}

GNC::GCS::ControladorVistas::TipoListaVistas GNC::GCS::ControladorVistas::GetVistas(){
	//resets inactivity
	GNC::GCS::InactivityController::Instance()->ResetsInactivity();
	//

	GNC::GCS::ControladorVistas::TipoListaVistas result;

	for(MapaVentanas::iterator it = m_MapaVentanas.begin(); it!= m_MapaVentanas.end(); ++it){
		result.push_back((*it).second);
	}

	return result;
}


void GNC::GCS::ControladorVistas::ProcesarEvento(GNC::GCS::Events::IEvent *evt){
	GNC::GCS::Events::EventoModificacionFichero* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionFichero*>(evt);

	GNC::GCS::IVista* pVista = NULL;

	if(pEvt != NULL && pEvt->GetVistaModificada()!=NULL){
		if(pEvt->GetTipoEvento() == GNC::GCS::Events::EventoModificacionFichero::FicheroGuardado){
			pVista = pEvt->GetVistaModificada();
		}
		else if(pEvt->GetTipoEvento() == GNC::GCS::Events::EventoModificacionFichero::FicheroModificado){
			pVista = pEvt->GetVistaModificada();
		}
	}

	if (pVista != NULL)
	{
		SetTitulo(pVista, pVista->GetTitulo());
	}

}
