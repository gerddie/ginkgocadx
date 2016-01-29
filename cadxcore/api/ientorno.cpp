/*
 *  
 *  $Id: ientorno.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <api/ientorno.h>
#include <main/entorno.h>

GNC::GCS::IEntorno::IEntorno()
{
}

GNC::GCS::IEntorno::~IEntorno()
{
}

GNC::GCS::IEntorno* GNC::GCS::IEntorno::Instance()
{
	return GNC::Entorno::Instance();
}

void GNC::GCS::IEntorno::FreeInstance()
{
	return GNC::Entorno::FreeInstance();
}