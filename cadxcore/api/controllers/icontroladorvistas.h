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

#include <string>
#include <list>
#include <api/api.h>
#include <api/observers/iviewsobserver.h>

class wxWindow;

namespace GNC {
	namespace GCS {

		class EXTAPI IControladorVistas {
		public:
			typedef std::list<IVista*> TipoListaVistas;

			static IControladorVistas* Instance();
			static void FreeInstance();

			virtual wxWindow* GetRootWindow() = 0;

			virtual void Freeze() = 0;
			virtual void Thaw() = 0;

			virtual void SetTitulo(GNC::GCS::IVista* pVista, const std::string& titulo) = 0;
			virtual std::string GetTitulo(GNC::GCS::IVista* pVista) = 0;

			virtual void Registrar(GNC::GCS::IVista*  pVista) = 0;
			virtual void Destruir(GNC::GCS::IVista*   pVista) = 0;
			virtual void Destruir(wxWindow* pVentana) = 0;

//region "Getters de vistas activas"

			virtual GNC::GCS::IVista* GetVistaActiva() = 0;

//endregion

//region "Propagacion de eventos de cambios de configuracion
			virtual void PropagarConfiguracionCambiada() = 0;
//endregion

//region "Gestion de carga y descarga de vistas"
			virtual void SolicitarActivarVista(GNC::GCS::IVista* pVista) = 0;
//endregion

			virtual TipoListaVistas GetVistas() = 0;

		protected:

			virtual ~IControladorVistas();
			IControladorVistas();

		};
	}
}
