#pragma once
/*
 *  
 *  $Id: toolpan.h $
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
#if defined(ENABLE_PANTOOL)
#ifndef PANTOOL_H
#define PANTOOL_H

#include <export/tools/itoolpan.h>
#include <widgets/wpan.h>
#include <api/api.h>

namespace GNC {

	class EXTAPI ToolPan : public GNC::GCS::IToolPan {
	public:
		typedef GNC::GCS::Events::EventoRaton       TEventoRaton;
		typedef GNC::GCS::Events::EventoTeclado     TEventoTeclado;
		typedef GNC::GCS::Widgets::WPanBuilder TBuilder;
		ToolPan();
		~ToolPan();

//region "Realización de la interfaz IHerramienta"

		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

//endregion


//region "Eventos refinados desde los paneles"

//endregion

	protected:

		TBuilder*            m_pBuilder;
	};
}
#endif
#endif