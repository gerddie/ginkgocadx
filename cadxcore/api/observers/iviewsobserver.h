/*
 *  
 *  $Id: iviewsobserver.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GNC {
	namespace GCS {
		/* Interfaz de observacion de vistas */
		class EXTAPI IObservadorVistas {
		public:

			IObservadorVistas() {}

			virtual ~IObservadorVistas() {}

			//region "Interfaz de observacion de vistas"

			virtual void OnVistaActivada(GNC::GCS::IVista* /*pVista*/) {}

			// Opcionales
			virtual void OnVistaCreada(GNC::GCS::IVista* ) {}

			virtual void OnVistaDestruida(GNC::GCS::IVista* ) {}
			//endregion

		};
	}
}
