/*
 *  
 *  $Id: pointertool.h $
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
#if defined(ENABLE_POINTERTOOL)
#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include <export/tools/ipointertool.h>
#include <api/api.h>

/// Forward declarations

namespace GNC {
	namespace GCS {
		namespace Events {
			class EventoRaton;
			class EventoTeclado;
		}
		namespace Widgets {
			class WSeleccionBuilder;
		}
	}
}


namespace GNC {

	class EXTAPI PointerTool : public GNC::GCS::IPointerTool {
	public:
		typedef GNC::GCS::Events::EventoRaton       TEventoRaton;
		typedef GNC::GCS::Events::EventoTeclado     TEventoTeclado;
		typedef GNC::GCS::Widgets::WSeleccionBuilder TSeleccionBuilder;
		PointerTool();
		~PointerTool();

//region "Realización de la interfaz IHerramienta"

		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

//endregion


//region "Eventos refinados desde los paneles"

//endregion

	protected:

		TSeleccionBuilder*            m_pSeleccionBuilder;
	};
}
#endif
#endif