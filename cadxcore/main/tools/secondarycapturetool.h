/*
 *  
 *  $Id: secondarycapturetool.h $
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
#ifndef SECONDARYCAPTURETOOL_H
#define SECONDARYCAPTURETOOL_H

#include <export/tools/isecondarycapturetool.h>
#include <api/api.h>

namespace GNC {

	class EXTAPI SecondaryCaptureTool : public GNC::GCS::ISecondaryCaptureTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		SecondaryCaptureTool();
		~SecondaryCaptureTool();
		
		virtual bool ExecuteAction();

	protected:

	};
}
#endif