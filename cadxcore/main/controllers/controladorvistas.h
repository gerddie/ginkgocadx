/*
 *  
 *  $Id: controladorvistas.h $
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

#include <string>
#include <map>

#include <api/globals.h>
#include <api/ilock.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/observers/ieventsobserver.h>

class wxAuiNotebook;

namespace GNC {

	namespace GCS {
		class ControladorVistas : public IControladorVistas, public GNC::GCS::IEventsObserver, public GNC::GCS::ILockable {

//region "Patron singleton"
		public:
			static ControladorVistas* Instance();
			static void FreeInstance();

			static ControladorVistas* m_pInstance;
//endregion

//region "Interfaz de controlador de vistas"
		public:

			virtual void Freeze();
			virtual void Thaw();

			/* Obtiene la ventana raiz (NoteBook) que es padre de las ventanas al inicio. */
			virtual wxWindow* GetRootWindow();

			/* Obtiene la vista registrada asociada a una ventana */
			GNC::GCS::IVista* ObtenerVistaRegistrada(wxWindow* pVentana);

			virtual void SetTitulo(GNC::GCS::IVista* pVista, const std::string& titulo);
			virtual std::string GetTitulo(GNC::GCS::IVista* pVista);

			virtual void Registrar(GNC::GCS::IVista*  pVista);
			virtual void Destruir(GNC::GCS::IVista*   pVista);
			virtual void Destruir(wxWindow* pVentana);

			virtual void PropagarConfiguracionCambiada();
//endregion

//region "Getters de vistas activas"

			virtual GNC::GCS::IVista* GetVistaActiva();

//endregion


//region "Gestion de carga y descarga de vistas"

			virtual void SolicitarActivarVista(GNC::GCS::IVista* pVista);

//endregion

//region "Propagación de eventos"
			virtual void PropagarVistaCreada(GNC::GCS::IVista* pVista);
			virtual void PropagarVistaDestruida(GNC::GCS::IVista* pVista);
//endregion

//region "Gestión de paneles extra"

			virtual void MostrarPanelFlotante(wxWindow* win, int x = -1, int y = -1);

			virtual bool EsVisible(wxWindow* win);

			virtual void OcultarPanel(wxWindow* win);

//endregion

			//region "interfaz de eventos"
			void ProcesarEvento(GNC::GCS::Events::IEvent *evt);
			//endregion

			virtual TipoListaVistas GetVistas();

			typedef std::map<wxWindow*, GNC::GCS::IVista*> MapaVentanas;
			typedef std::map<GNC::GCS::IVista*, int> MapaConteoRegistros; // Tupla de registro de repeticion por vista
			typedef std::map<std::string, MapaConteoRegistros > MapaConteoTitulos;
			typedef std::map<GNC::GCS::IVista*, std::string> MapaTitulosRegistrados;

			MapaVentanas           m_MapaVentanas;
			MapaConteoTitulos      m_MapaConteoTitulos;
			MapaTitulosRegistrados m_MapaTitulosRegistrados;

		protected:
			/* Constructor. Se mantiene una referencia a la ventana principal por ser ella la responsable de crear "tabs" */
			ControladorVistas();
			~ControladorVistas();

		protected:
			GNC::GCS::IVista*                   m_pVistaActiva;
		};
	}
}
