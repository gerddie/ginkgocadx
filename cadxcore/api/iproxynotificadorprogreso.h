/*
 *  
 *  $Id: iproxynotificadorprogreso.h $
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
#include <string>
#include <api/api.h>

namespace GNC {
	class EXTAPI IProxyNotificadorProgreso {
	public:
		IProxyNotificadorProgreso() {}
		virtual ~IProxyNotificadorProgreso() {}


		/** Notifica el progreso. Este metodo se puede ejecutar de manera asincrona, y su desencadenante resulta en una llamada sincrona con la interfaz.
		Devuelve false si el comando se ejecutara, de manera que el invocador (tarea que ejecuta la accion real) pueda saber si debe terminar.**/
		virtual bool NotificarProgreso(float progresoNormalizado, const std::string& texto) = 0;
	};
}
