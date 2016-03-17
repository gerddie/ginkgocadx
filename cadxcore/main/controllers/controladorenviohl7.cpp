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


#define LOGGER "ControladorEnvioHl7"
#include <api/internationalization/internationalization.h>
#include <endpoint/endpoint.h>
#include "controladorenviohl7.h"
#include "controladorbbddhl7.h"
#include <main/controllers/commandcontroller.h>
#include <api/controllers/icontroladorlog.h>
#include "controladoreventos.h"
#include "configurationcontroller.h"
#include <api/globals.h>
#include <api/threads/thread.h>
#include <api/icommand.h>
#include <api/imodelohl7.h>
#include <eventos/progresocomando.h>

#include "../entorno.h"
#include <commands/sendhl7command.h>



void GIL::HL7::ControladorEnvioHl7::FreeInstance()
{
        wxCriticalSectionLocker locker(m_criticalSection);
        if(m_pInstance != NULL) {
                LOG_DEBUG("Core/HL7", _Std("Stopping HL7 sender"))
                //if(!m_pInstance->EstaAbortado()) {
                m_pInstance->Abortar();
                m_pInstance->Wait();
                delete m_pInstance;
                m_pInstance = NULL;
                //}
        }
}

void GIL::HL7::ControladorEnvioHl7::Arrancar()
{
        bool boolValue;
        GNC::GCS::ConfigurationController::Instance()->readBoolGeneral("/GinkgoCore/HCE", "HL7MonitorEnabled", boolValue, true);

        if (boolValue) {
                wxCriticalSectionLocker locker(m_criticalSection);
                if (m_pInstance == NULL) {
                        m_pInstance = new GIL::HL7::ControladorEnvioHl7();
                        m_pInstance->Create();
                        GNC::GCS::Threading::SetThreadName( m_pInstance->GetId(), "EnvioHL7");
                        wxThreadError error = m_pInstance->Run();
                        if(error != wxTHREAD_NO_ERROR) {
                                LOG_ERROR("Core/HL7", _Std("Error starting HL7 sender") << ": " << error )
                        } else {
                                LOG_INFO("Core/HL7", _Std("HL7 sender started"))
                        }
                } else {
                        if(!m_pInstance->IsRunning()) {
                                m_pInstance->Abortar();
                                m_pInstance->Wait();
                                delete m_pInstance;
                                m_pInstance = new GIL::HL7::ControladorEnvioHl7();
                                m_pInstance->Create();
                                wxThreadError error = m_pInstance->Run();
                                if(error != wxTHREAD_NO_ERROR) {
                                        LOG_ERROR("Core/HL7", _Std("Error starting HL7 sender") << ": " << error )
                                } else {
                                        LOG_INFO("Core/HL7", _Std("HL7 sender started"))
                                }
                        }
                }
        }
}

bool GIL::HL7::ControladorEnvioHl7::EstaArrancado()
{
        wxCriticalSectionLocker locker(m_criticalSection);
        if(m_pInstance == NULL) {
                return false;
        } else {
                return m_pInstance->IsRunning();
        }
}

GIL::HL7::ControladorEnvioHl7* GIL::HL7::ControladorEnvioHl7::m_pInstance = NULL;
wxCriticalSection GIL::HL7::ControladorEnvioHl7::m_criticalSection;

//proceso que envia mensajes
GIL::HL7::ControladorEnvioHl7::ControladorEnvioHl7() : wxThread(wxTHREAD_JOINABLE)
{
        GNC::GCS::Events::EventoProgresoComando evt;
        GNC::GCS::ControladorEventos::Instance()->Registrar(this,evt);
        m_Abortado = false;
}

GIL::HL7::ControladorEnvioHl7::~ControladorEnvioHl7()
{
        LOG_DEBUG("Core/HL7", _Std("HL7 controller destroyed"))
}

void GIL::HL7::ControladorEnvioHl7::Abortar()
{
        m_Abortado = true;
        GNC::GCS::ICommandController::Instance()->AbortarComandosDeOwner(this);
        m_semaphore.Post();
        m_semaphore.Post();
}

bool GIL::HL7::ControladorEnvioHl7::EstaAbortado()
{
        return m_Abortado;
}

void* GIL::HL7::ControladorEnvioHl7::Entry()
{
        m_Abortado = false;

        GNC::GCS::Threading::SetThreadName( GetId(), "HL7 Sender");

        while(!TestDestroy() && !m_Abortado) {
                if(TestDestroy() || m_Abortado) {
                        break;
                }
                try {
                        //esperamos a que arranque todo correctamente
                        m_semaphore.WaitTimeout(5000);
                        TListaMensajes listaMensajes;

                        GIL::HL7::ControladorBBDDHl7::Instance()->GetMensajesHL7(listaMensajes,true);

                        if (!TestDestroy() && !m_Abortado && listaMensajes.size() > 0) {
                                //launch command...
                                GADAPI::SendHL7CommandParams* pParams = new GADAPI::SendHL7CommandParams(listaMensajes);
                                GADAPI::SendHL7Command* pCmd = new GADAPI::SendHL7Command(pParams);
                                GNC::GCS::ICommandController::Instance()->ProcessAsync("Send HL7", pCmd, this);
                                m_semaphore.Wait();
                        }
                } catch (HL7Exception& ex) {
                        LOG_ERROR("Core/HL7", _Std("Error processing message") << ": " << ex.str())
                }
        }
        return NULL;
}

void GIL::HL7::ControladorEnvioHl7::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
        GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);

        if (pEvt == NULL  || pEvt->GetComando() == NULL) {
                return;
        }

        if (pEvt->GetTipo() == GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado) {
                GADAPI::SendHL7Command* pCmdSend = dynamic_cast< GADAPI::SendHL7Command * > (pEvt->GetComando());
                if (pCmdSend != NULL) {
                        m_semaphore.Post();
                }
        }
}

