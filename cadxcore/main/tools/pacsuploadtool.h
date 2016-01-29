#pragma once
/*
 *  
 *  $Id: pacsuploadtool.h $
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
#include <export/tools/ipacsuploadtool.h>
#include <api/api.h>

#include <map>


namespace GNC {

	class EXTAPI PACSUploadTool : public GNC::GCS::IPACSUploadTool {

	protected:

	public:
		virtual GNC::GCS::ITool* NewTool();
		PACSUploadTool();
		~PACSUploadTool();
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

	protected:

	};
}