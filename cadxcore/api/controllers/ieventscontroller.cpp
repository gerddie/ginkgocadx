/*
 *  
 *  $Id: ieventscontroller.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "ieventscontroller.h"
#include <main/controllers/controladoreventos.h>


GNC::GCS::IEventsController::IEventsController()
{
}

GNC::GCS::IEventsController::~IEventsController()
{
}

GNC::GCS::IEventsController* GNC::GCS::IEventsController::Instance()
{
	return GNC::GCS::ControladorEventos::Instance();
}

void GNC::GCS::IEventsController::FreeInstance()
{
	GNC::GCS::ControladorEventos::FreeInstance();
}
