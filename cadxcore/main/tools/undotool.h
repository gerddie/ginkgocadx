#pragma once
/*
 *  
 *  $Id: undotool.h $
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
#if defined(ENABLE_UNDOTOOL)
#ifndef UNDOTOOL_H
#define UNDOTOOL_H
#include <export/tools/iundotool.h>
#include <api/api.h>

#include <map>

namespace GNC {

	class EXTAPI UndoTool : public GNC::GCS::IUndoTool {

	protected:

	public:
		virtual GNC::GCS::ITool* NewTool();
		UndoTool();
		~UndoTool();
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

		//deshacer
		virtual void Deshacer();
		//rehacer
		virtual void Rehacer();
		virtual bool PuedeDeshacer(std::string& nombreAccion);
		virtual bool PuedeRehacer(std::string& nombreAccion);
	protected:

	};
}
#endif
#endif