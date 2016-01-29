/*
 *  
 *  $Id: itoolcontroller.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <map>
#include "itoolcontroller.h"
#include <main/toolsystem/toolcontroller.h>


GNC::GCS::IToolController::IToolController()
{
}

GNC::GCS::IToolController::~IToolController()
{
}

GNC::GCS::IToolController* GNC::GCS::IToolController::New()
{
	return new GNC::ToolController();
}
