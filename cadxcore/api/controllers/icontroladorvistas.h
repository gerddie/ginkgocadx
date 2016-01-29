/*
 *  
 *  $Id: icontroladorvistas.h $
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
