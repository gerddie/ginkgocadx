/*
 *  
 *  $Id: iwidgetsmanager.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "iwidgetsmanager.h"
#include "observers/iwidgetsobserver.h"

GNC::GCS::IWidgetsManager::IWidgetsManager(GNC::GCS::IVista* pView) :
m_pRendererActivo(NULL),
m_Modified(false),
m_pView(pView),
m_Enabled(true),
m_pDefaultEventListenerDelegate(NULL), 
m_cursor(GNC::GCS::Widgets::CUR_FLECHA)
{ }

GNC::GCS::IWidgetsManager::~IWidgetsManager() {
	m_pView = NULL;
}

GNC::GCS::IVista* GNC::GCS::IWidgetsManager::GetVista(){
	return m_pView;
}

void GNC::GCS::IWidgetsManager::MarcarModificado(bool modificado) {
	m_Modified = modificado;
}

bool GNC::GCS::IWidgetsManager::EstaModificado() {
	return m_Modified;
}

void GNC::GCS::IWidgetsManager::SetDefaultEventListenerDelegate(GNC::GCS::Events::ISubscriptorEventos* pDelegate) {
	this->m_pDefaultEventListenerDelegate = pDelegate;
}

void GNC::GCS::IWidgetsManager::RegisterBuilder(GNC::GCS::Widgets::IWidgetBuilder* pBuilder)
{
	m_pBuilders.push_back(pBuilder);
}

void GNC::GCS::IWidgetsManager::UnRegisterBuilder(GNC::GCS::Widgets::IWidgetBuilder* pBuilder)
{
	for (ListaBuilders::iterator it = m_pBuilders.begin(); it != m_pBuilders.end(); ++it) {
		if ( (*it) == pBuilder) {
			m_pBuilders.erase(it);
			break;
		}
	}
}

void GNC::GCS::IWidgetsManager::Enable(bool enabled)
{
	m_Enabled = enabled;
}

bool GNC::GCS::IWidgetsManager::IsEnabled()
{
	return m_Enabled;
}

//----------------------------------------------------------------------------------------------------
//region "Interfaz de registro del IWidgetsRenderer"

void GNC::GCS::IWidgetsManager::InsertarObservador(GNC::GCS::Widgets::IObservadorWidget* observador)
{
	m_Observadores.push_back(observador);
}

void GNC::GCS::IWidgetsManager::EliminarObservador(GNC::GCS::Widgets::IObservadorWidget* observador)
{
	IteradorListaObservadores it;
	
	for (it = m_Observadores.begin(); it != m_Observadores.end() && (*it) != observador; ++it);
	
	if (it != m_Observadores.end()) {
		m_Observadores.erase(it);
	}
}

//endregion

