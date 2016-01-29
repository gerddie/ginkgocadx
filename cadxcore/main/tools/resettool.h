/*
 *  
 *  $Id: resettool.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include "enabled_tools.h"
#if defined(ENABLE_RESETTOOL)
#ifndef RESETTOOL_H
#define RESETTOOL_H

#include <export/tools/iresettool.h>
#include <api/api.h>

namespace GNC {

	class EXTAPI ResetTool : public GNC::GCS::IResetTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		ResetTool();
		~ResetTool();
		
		virtual bool ExecuteAction();

	protected:

	};
}
#endif
#endif