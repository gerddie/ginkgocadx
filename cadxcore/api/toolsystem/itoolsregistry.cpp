/*
 *  
 *  $Id: itoolsregistry.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "itoolsregistry.h"
#include <main/toolsystem/toolsregistry.h>

GNC::GCS::ToolsRegistryException::ToolsRegistryException(const std::string& msg) throw() : GNC::GCS::IException(msg, "GNC", false)
{
}


GNC::GCS::IToolsRegistry::IToolsRegistry()
{
}

GNC::GCS::IToolsRegistry::~IToolsRegistry()
{
}


void GNC::GCS::IToolsRegistry::UnRegisterAll()
{
}

//------------------------------------------------------------------------------
//region "Singleton pattern"

/** Singleton getter **/
GNC::GCS::IToolsRegistry* GNC::GCS::IToolsRegistry::Instance()
{
	return GNC::ToolsRegistry::Instance();

}

/** Singleton free **/
void GNC::GCS::IToolsRegistry::FreeInstance()
{
	GNC::ToolsRegistry::FreeInstance();
}

//endregion
