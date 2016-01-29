/*
 *  
 *  $Id$
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
#include <list>

#include "ventanacontrolhl7base.h"


class wxSQLite3Database;
namespace GNC{
	namespace GUI {
		class TimerControlHL7;
		class VentanaControlHL7 : public VentanaControlHL7Base
		{
		public:
			//singleton
			static VentanaControlHL7* Instance();
			static void CerrarSiAbierta();
			void Refrescar();

		protected:
			static VentanaControlHL7*  m_pInstance;
			TimerControlHL7* m_pTimerRefresh;

			VentanaControlHL7();
			~VentanaControlHL7();

			void OnActivate(wxActivateEvent &event);
			void OnMensajeMenu(wxDataViewEvent& event);
			void OnMensajeDClick(wxDataViewEvent& event);
			void OnMostrarMensaje(wxCommandEvent &event);
			void OnEditarMensaje(wxCommandEvent &event);
			void OnListaKey(wxKeyEvent &event);
			void OnEliminarMensaje(wxCommandEvent &event);
			void OnPurgarMensajes(wxCommandEvent& event);
			void OnActualizar(wxCommandEvent &event);
			void OnArrancarPararClick(wxCommandEvent &event);
			void RefrescarMensajes();
			void ActualizarEstadoControlador();
			void MostrarVentanaMensaje();
			void EliminarMensaje();

			virtual void OnMenuSalir( wxCommandEvent& event );
			virtual void OnKeyDown( wxKeyEvent& event );

		//endregion

		};
	};
};
