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

#ifndef TRACKBALLTOOL_H
#define TRACKBALLTOOL_H

#include <export/tools/itrackballtool.h>
#include <api/api.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {
			class WTrackballBuilder;
		}
		namespace Events {
			class EventoRaton;
			class EventoTeclado;
		}
	}

	class EXTAPI TrackballTool : public GNC::GCS::ITrackballTool {
	public:
		typedef GNC::GCS::Events::EventoRaton       TEventoRaton;
		typedef GNC::GCS::Events::EventoTeclado     TEventoTeclado;
		typedef GNC::GCS::Widgets::WTrackballBuilder TBuilder;
		TrackballTool();
		~TrackballTool();

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
