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
#include "iconfigurationcontroller.h"
#include <string>
#include <api/autoptr.h>
#include <main/controllers/configurationcontroller.h>

GNC::GCS::IConfigurationController::IConfigurationController()
{
}

GNC::GCS::IConfigurationController::~IConfigurationController()
{
}

GNC::GCS::IConfigurationController* GNC::GCS::IConfigurationController::Instance()
{
	return GNC::GCS::ConfigurationController::Instance();
}

void GNC::GCS::IConfigurationController::FreeInstance()
{
	GNC::GCS::ConfigurationController::FreeInstance();
}

