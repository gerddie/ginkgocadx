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

#include "thumbnailcontroller.h"
#include <main/entorno.h>
#include <wx/thread.h>
#include <eventos/progresocomando.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladoreventos.h>

#define TAMANIO_MAX_PILA 10
namespace GNC {
	namespace GCS {

		ThumbnailController* ThumbnailController::Instance()
		{
			if (m_pInstance == NULL) {
				m_pInstance = new ThumbnailController();
			}
			return m_pInstance;
		}

		void ThumbnailController::FreeInstance()
		{
			if(m_pInstance!=NULL){
				delete m_pInstance;
				m_pInstance = NULL;
			}
		}

		ThumbnailController* ThumbnailController::m_pInstance = NULL;

		ThumbnailController::ThumbnailController()
		{
			GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
			m_pCriticalSection = new wxCriticalSection();
			//m_pContinueTimer = 
			m_pCmd = NULL;			
		}
		ThumbnailController::~ThumbnailController()
		{
			if(m_pCriticalSection != NULL)
			{
				delete m_pCriticalSection;
				m_pCriticalSection = NULL;
			}
			if (m_pCmd != NULL) {
				GNC::GCS::ICommandController::Instance()->AbortarComando(m_pCmd);
			}
		}

		void ThumbnailController::requestThumbnail(long file_pk, GADAPI::ThumbnailsNotifier* pNotifier)
		{
			m_pCriticalSection->Enter();
			m_petitions.push_back(TPetition(pNotifier, file_pk));
			if (m_pCmd == NULL) {
				m_pCmd = new GADAPI::ThumbnailsCommand(file_pk, pNotifier);
				GNC::GCS::ICommandController::Instance()->ProcessAsync("Generating thumbnails...", m_pCmd, this);
			}
			m_pCriticalSection->Leave();
		}

		void ThumbnailController::removeRequest(GADAPI::ThumbnailsNotifier* pNotifier)
		{
			m_pCriticalSection->Enter();
			if (!m_petitions.empty()) {
				if (m_petitions.front().m_pNotifier == pNotifier && m_pCmd != NULL) {
					GNC::GCS::ICommandController::Instance()->AbortarComandosDeOwner(this);
					m_pCmd = NULL;
					//to start with next it has to be synchronized with gui so call timer...
					if(IsRunning()) {
						Stop();
						Start(300, true);
					}
				}
			}
			for (TListPetitions::iterator it = m_petitions.begin(); it != m_petitions.end(); ++it) {
				if ((*it).m_pNotifier == pNotifier) {
					m_petitions.erase(it);
					break;
				}
			}
			m_pCriticalSection->Leave();
		}

		void ThumbnailController::Notify()
		{
			m_pCriticalSection->Enter();
			if (m_pCmd == NULL) {
				//run front if exists...
				TListPetitions::iterator it = m_petitions.begin();
				if (it != m_petitions.end()) {
					m_pCmd = new GADAPI::ThumbnailsCommand((*it).m_file_pk, (*it).m_pNotifier);
					GNC::GCS::ICommandController::Instance()->ProcessAsync("Generating thumbnails...", m_pCmd, this);
				} 
			}
			m_pCriticalSection->Leave();
		}

		void ThumbnailController::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
		{
			GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);
			if (pEvt == NULL  || pEvt->GetComando() == NULL || pEvt->GetComando() != m_pCmd) {
				return;
			}
			switch (pEvt->GetTipo()) {
				case GNC::GCS::Events::EventoProgresoComando::TEP_Iniciado:
					break;
				case GNC::GCS::Events::EventoProgresoComando::TEP_Progreso:
					break;
				case GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado:
					{
						m_pCriticalSection->Enter();
						m_petitions.pop_front();
						TListPetitions::iterator it = m_petitions.begin();
						if (it != m_petitions.end() && !m_pCmd->EstaAbortado()) {
							m_pCmd = new GADAPI::ThumbnailsCommand((*it).m_file_pk, (*it).m_pNotifier);
							GNC::GCS::ICommandController::Instance()->ProcessAsync("Generating thumbnails...", m_pCmd, this);
						} else {
							m_pCmd = NULL;
						}
						m_pCriticalSection->Leave();
					}
					break;
				case GNC::GCS::Events::EventoProgresoComando::TEP_Unknown:
			break;
			}	
		}
	}
}
