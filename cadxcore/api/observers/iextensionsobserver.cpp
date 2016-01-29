/*
 *  
 *  $Id: iextensionsobserver.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "iextensionsobserver.h"

#include <api/controllers/imodulecontroller.h>



GNC::GCS::IExtensionsObserver::IExtensionsObserver()
{
}

GNC::GCS::IExtensionsObserver::~IExtensionsObserver()
{
}

void GNC::GCS::IExtensionsObserver::OnModuleActivated(GNC::GCS::IModuleController* )
{
}

void GNC::GCS::IExtensionsObserver::OnModuleDeactivated(GNC::GCS::IModuleController* )
{
}
