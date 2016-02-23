/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
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
