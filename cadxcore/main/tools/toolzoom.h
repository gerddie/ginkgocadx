/*
 *  
 *  $Id: toolzoom.h $
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
#if defined(ENABLE_ZOOMTOOL)
#ifndef ZOOMTOOL_H
#define ZOOMTOOL_H
#include <export/tools/itoolzoom.h>
#include <api/api.h>
#include <widgets/wzoom.h>

namespace GNC {
	class wxToolZoomKeyHandler;
	class EXTAPI ToolZoom : public GNC::GCS::IToolZoom {
	public:
		typedef GNC::GCS::Events::EventoRaton       TEventoRaton;
		typedef GNC::GCS::Events::EventoTeclado     TEventoTeclado;
		typedef GNC::GCS::Widgets::WZoomBuilder      TBuilder;
		ToolZoom();
		~ToolZoom();

		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();
		virtual bool HasDropDownMenu();
		virtual void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);

		virtual void ZoomOneOne();
		virtual void ConnectKeyEvents(wxWindow* pWindow);

	protected:
		friend class wxToolZoomKeyHandler;
		TBuilder*            m_pBuilder;
		wxToolZoomKeyHandler* m_pHandler;
	};
}
#endif
#endif