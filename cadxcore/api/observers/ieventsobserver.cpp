/*
 *  
 *  $Id: ieventsobserver.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "ieventsobserver.h"
#include <api/controllers/ieventscontroller.h>

GNC::GCS::IEventsObserver::IEventsObserver()
{
	//m_pControlador = NULL;
}

GNC::GCS::IEventsObserver::~IEventsObserver()
{
	GNC::GCS::IEventsController::Instance()->DesRegistrar(this);
}

/*
void GNC::GCS::IEventsObserver::OnRegistrar (IControladorEventos* pControlador)
{
	m_pControlador = pControlador;
}
*/

void GNC::GCS::IEventsObserver::PreProcesarEvento(GNC::GCS::Events::IEvent*, std::list<GNC::GCS::Events::IEvent*>& )
{

}

bool GNC::GCS::IEventsObserver::FiltrarEvento(GNC::GCS::Events::IEvent *)
{
	return false;
}
