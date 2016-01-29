#pragma once
/*
 *  
 *  $Id: showhistory.h $
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
#include "enabled_tools.h"
#ifndef IMAGEEXPORTTTOOL_H
#define IMAGEEXPORTTTOOL_H
#include <export/tools/iimageexporttool.h>
#include <api/api.h>

#include <map>


namespace GNC {

	class EXTAPI ImageExportTool : public GNC::GCS::IImageExportTool {

	protected:

	public:
		virtual GNC::GCS::ITool* NewTool();
		ImageExportTool();
		~ImageExportTool();
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

	protected:

	};
}
#endif