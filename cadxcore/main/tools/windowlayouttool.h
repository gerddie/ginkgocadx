/*
 *  
 *  $Id: windowlayouttool.h $
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
#if defined(ENABLE_WINDOWLAYOUTTOOL)
#ifndef WINDOWLAYOUTTOOL_H
#define WINDOWLAYOUTTOOL_H

#include <export/tools/iwindowlayouttool.h>
#include <api/api.h>

namespace GNC {

	class EXTAPI WindowLayoutTool : public GNC::GCS::IWindowLayoutTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		WindowLayoutTool();
		~WindowLayoutTool();

		virtual bool HasDropDownMenu();
		void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
		
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		
		virtual void SetWindowLayout(int filas, int columnas);
		virtual void AddRow();
		virtual void AddColumn();
		virtual void DelRow();
		virtual void DelColumn();
		
		virtual bool AddAllowed();
		virtual bool Supports(int filas, int columnas);
		virtual bool DeleteRowAllowed();
		virtual bool DeleteColumnAllowed();
		
		virtual bool SupportsSplit();
		virtual void SplitActive(bool vertically);

	protected:

	};
}
#endif
#endif