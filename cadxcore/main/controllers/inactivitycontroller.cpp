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


#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>

#include "inactivitycontroller.h"
#include <api/ientorno.h>
#include <api/controllers/icontroladorpermisos.h>

#include <main/controllers/commandcontroller.h>
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/controladorpermisos.h>
#include <main/gui/countdownmessage/inactivitycountdownmessage.h>
#include <main/gui/mainwindow/ventanaprincipal.h>

#include <main/gui/mainwindow/detacheddialog.h>

//this timer is to controlate user inactivity
namespace GNC {
	namespace GCS {
		class TimerInactivity: public wxTimer {
		public:
			TimerInactivity() 
			{
				m_active = false;
				m_frozen = false;
				GNC::GCS::Permisos::EstadoPermiso estado = GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "inactivity_time");
				if (estado) {
					m_maxInactivity = estado.ObtenerValor<long>() * 1000;
					this->Start(m_maxInactivity, true);
					pCountDown = new GNC::GUI::InactivityCountdownMessage(GNC::GCS::IEntorno::Instance()->GetVentanaRaiz(), _("Ginkgo CADx has detected inactivity and it will be closed in %d seconds.\nDo you want to continue?"));
					m_active = true;
				} else {
					pCountDown = NULL;
					m_maxInactivity = -1;
				}

				estado = GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "auto_close");
				if (estado) {
					m_active = true;
				}
			}

			~TimerInactivity() 
			{
			}
			
			virtual void Notify()
			{
				if (GNC::GCS::ICommandController::Instance()->GetNumActiveCommands() == 0) {
					if (m_maxInactivity > 0) {
						if (!pCountDown->IsShown()) {
							pCountDown->Show();
						}
					}
				} else {
					Reset();
				}
			}

			virtual void Reset() 
			{
				if (!m_active || !wxThread::IsMain()) {
					return;
				}
				if (pCountDown != NULL && pCountDown->IsShown()) {
					pCountDown->Show(false);
				}
				if (IsRunning()) {
					Stop();
					if (m_maxInactivity > 0) {
						this->Start(m_maxInactivity, true);
					}
				}
			}

			virtual void KillNow()
			{
				if (!m_active || !wxThread::IsMain()) {
					return;
				}
				if (!m_frozen && GNC::GCS::ICommandController::Instance()->GetNumActiveCommands() == 0) {
					//kills now
					GNC::GCS::IEntorno::Instance()->GetVentanaPrincipal()->Close();
					//
				}
			}

			virtual void StopMonitoring() 
			{
				if (!m_active || !wxThread::IsMain()) {
					return;
				}
				m_frozen = true;
				if (pCountDown != NULL && pCountDown->IsShown()) {
					pCountDown->Show(false);
				}
				if (IsRunning()) {
					Stop();
				}
			}
			
			virtual void StartMonitoring() 
			{
				if (!m_active || !wxThread::IsMain()) {
					return;
				}
				m_frozen = false;
				if (m_maxInactivity > 0) {
					this->Start(m_maxInactivity, true);
				}
			}

			virtual void DetachForm()
			{
				pCountDown = NULL;
			}

		protected:
			long m_maxInactivity;
			bool m_active;
			bool m_frozen;
			GNC::GUI::InactivityCountdownMessage* pCountDown;
		};
	}
}
//


//region "Patron singleton"

GNC::GCS::InactivityController* GNC::GCS::InactivityController::m_pInstance = 0;

GNC::GCS::InactivityController::InactivityController()
{
	m_pTimerInactivity = new TimerInactivity;
}

GNC::GCS::InactivityController::~InactivityController()
{
	if (m_pTimerInactivity != NULL) {
		delete m_pTimerInactivity;
		m_pTimerInactivity = NULL;
	}
}

GNC::GCS::InactivityController* GNC::GCS::InactivityController::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new GNC::GCS::InactivityController();
	}
	return m_pInstance;
}

void GNC::GCS::InactivityController::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

//endregion

void GNC::GCS::InactivityController::ResetsInactivity()
{
	//resets inactivity
	m_pTimerInactivity->Reset();
	//
}

void GNC::GCS::InactivityController::RestartMonitoring()
{
	//resets inactivity
	m_pTimerInactivity->StartMonitoring();
	//
}

void GNC::GCS::InactivityController::StopsMonitoring()
{
	//resets inactivity
	m_pTimerInactivity->StopMonitoring();
	//
}
void GNC::GCS::InactivityController::DetachForm()
{
	m_pTimerInactivity->DetachForm();
}

void GNC::GCS::InactivityController::KillNow()
{
	//kills inmediatly
	m_pTimerInactivity->KillNow();
	//
}
