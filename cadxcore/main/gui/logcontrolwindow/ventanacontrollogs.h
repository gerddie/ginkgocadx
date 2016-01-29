/*
 *  
 *  $Id: ventanacontrollogs.h $
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
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/log4cplus/logdefines.h>
#include <wx/event.h>
#include "ventanacontrollogsbase.h"


namespace GNC {
	namespace GUI {

		class VentanaControlLogs : public VentanaControlLogsBase, public GNC::GCS::Logging::ILogger
		{
		public:
			//singleton
			static VentanaControlLogs* Instance();
			static void CerrarSiAbierta();
			void Refrescar();

			//region Interfaz ILogger
			virtual void Append(const GNC::GCS::Logging::LogEvent& le);

			//endregion

		protected:
			static VentanaControlLogs*  m_pInstance;
			GNC::GCS::IControladorLog::MapaLogLevels m_MapaLogLevels;
			bool m_needRefresh;

			VentanaControlLogs();
			~VentanaControlLogs();

			virtual void OnClose(wxCloseEvent& event);

			virtual void OnInternalIdle();

			virtual void OnLimpiarClick( wxCommandEvent& /*event*/ ) { LimpiarRegistros(); }
			virtual void OnLimpiarRegistros( wxCommandEvent& /*event*/ ) { LimpiarRegistros(); }

			virtual void OnGuardarRegistros( wxCommandEvent& event );
			virtual void OnNivelChoice( wxCommandEvent& event );

			virtual void OnSalirClick( wxCommandEvent& event );
			virtual void OnKeyDown( wxKeyEvent& event );
			void LimpiarRegistros();

			wxArrayString pendingLines;

		};
	}
}
