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

//#define _GINKGO_TRACE

#include <wx/string.h>
#include <wx/intl.h>
#include <wx/thread.h>

#include <api/internationalization/internationalization.h>

#include <endpoint/endpoint.h>
#include "sendhl7command.h"
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/controladorhl7.h>
#include <api/imodelohl7.h>
#include <api/ihl7parser.h>

#define IDC_SEND_HL7								117

// Singleton de persistencia
namespace GADAPI
{
SendHL7CommandParams::SendHL7CommandParams(const std::list<GIL::HL7::MensajeHL7>& messages)
{
        this->messages = messages;
        m_idLastSock = -1;
        m_pSemaphore = new wxSemaphore;
}

SendHL7CommandParams::~SendHL7CommandParams()
{
        delete m_pSemaphore;
}

SendHL7Command::SendHL7Command(SendHL7CommandParams* pParams) : IComando(pParams)
{
        m_pSendParams = pParams;

        SetId(IDC_SEND_HL7);
        EsperaA(IDC_SEND_HL7);
}

void SendHL7Command::Execute()
{
        int current = 1;
        for(std::list<GIL::HL7::MensajeHL7>::iterator it = m_pSendParams->messages.begin(); it!=m_pSendParams->messages.end(); ++it) {
                bool sent = false;
                while (!sent && !EstaAbortado()) {
                        float currentProgress = (float)(current-1)/m_pSendParams->messages.size();
                        {
                                wxString strTmp = wxString::Format(_("Sending HL7 message %d of %d"), (int)(current), (int)(m_pSendParams->messages.size()));
                                NotificarProgreso(currentProgress, std::string(strTmp.ToUTF8()));
                                LOG_DEBUG("Core/HL7", "Comienza el procesamiento del mensaje " << (*it).m_id)
                        }

                        try {
                                switch((*it).m_protocolo) {
                                case GIL::HL7::ControladorHL7::TP_MLLP:
                                        EnviarMensajeMLLP((*it).m_mensaje,(*it).m_destino,(*it).m_procesarACK,(*it).m_msgControlId);
                                        GIL::HL7::ControladorBBDDHl7::Instance()->ActualizarEstadoMensaje((*it).m_id,GIL::HL7::MensajeHL7::TE_Enviado,"");
                                        LOG_DEBUG("Core/HL7", _Std("Message successfully sent"))
                                        sent = true;
                                        break;
                                default:
                                        throw GIL::HL7::HL7Exception(_Std("Unknown protocol"));
                                }
                        } catch(GIL::HL7::HL7Exception& ex) {
                                LOG_ERROR("Core/HL7", _Std("Error sending message:") << ex.str())
                                //ha ocurrido un error

                                //al primer error se para
                                if (ex.IsFatal()) {
                                        GIL::HL7::ControladorBBDDHl7::Instance()->ActualizarEstadoMensaje((*it).m_id,GIL::HL7::MensajeHL7::TE_Error,ex.GetCause().c_str());
                                } else {
                                        GIL::HL7::ControladorBBDDHl7::Instance()->ActualizarEstadoMensaje((*it).m_id,GIL::HL7::MensajeHL7::TE_Warning,ex.GetCause().c_str());
                                }
                                NotificarProgreso(currentProgress, _Std("Error sending HL7 message see log for more details"));
                        } catch(...) {
                                LOG_DEBUG("Core/HL7", _Std("Error sending message:") << " " <<_Std("Unknown error"));
                                GIL::HL7::ControladorBBDDHl7::Instance()->ActualizarEstadoMensaje((*it).m_id, GIL::HL7::MensajeHL7::TE_Error, _Std("Unknown error"));
                        }
                        if (!sent) {
                                //wait one minute
                                m_pSendParams->m_pSemaphore->WaitTimeout(60000);
                        }
                } //while !sent
        }

}

void SendHL7Command::Update()
{
        if (EstaAbortado()) {
                return;
        }

}

void SendHL7Command::OnAbort()
{
        if (m_pSendParams->m_idLastSock != -1) {
                Endpoint::ShutdownSocket(m_pSendParams->m_idLastSock, Endpoint::BOTH);
        }
        m_pSendParams->m_pSemaphore->Post();
}



/* Envia el mensaje por MLLP */
void SendHL7Command::EnviarMensajeMLLP(const std::string& msg, const std::string& url, bool procesarACK, const std::string& msgControlId)
{
        const std::string START_BLOCK = "\x0b";
        const std::string END_BLOCK   = "\x1C";
        const std::string END_MSG     = "\x0D";

        std::cout << "inicializo..." << std::endl;
        Endpoint ep(TCP | CLIENT, url);

        if (!ep) {
                throw GIL::HL7::HL7Exception(_Std("Failed to start connection") + ": " + ep.m_error_str, "COMM");
        }

        m_pSendParams->m_idLastSock = ep.m_sockfd;

        std::stringstream outputStream;
        outputStream << START_BLOCK << msg << END_BLOCK << END_MSG;
        long nbytes = ep.Write(outputStream.str());
        if (nbytes != (long)outputStream.str().size()) {
                std::stringstream errmsg;
                if (nbytes == -1) {
                        errmsg << _Std("Error sending message:") << ep.m_error_str;
                } else {
                        errmsg << _Std("Error sending message. It was sent ") << nbytes << _Std(" bytes from a total of ") << outputStream.str();
                }
                throw GIL::HL7::HL7Exception(errmsg.str(), "COMM");
        }

        if (!procesarACK) {
                return;
        }

        ep.Shutdown(Endpoint::OUTPUT);

        //std::cout << "Message Control ID is = " << msgControlId << std::endl;

        outputStream.str("");

        //std::cout << "Connected to: " << std::string(ep.m_remote) << std::endl;

        unsigned int msgNum = 0;

        std::list<std::string> mStrings;
        std::stringstream      inputStream;

        std::string buff;

        bool started = false;
        bool pending = true;

        while( pending && ( ep.Read(-1024, buff) > 0 ) ) {
                if (!started) {

                        std::string::size_type sb_loc = buff.find(START_BLOCK, 0);

                        if (sb_loc != std::string::npos) {

                                started = true;

                                mStrings.push_back(std::string());

                                msgNum++;

                                std::string::size_type eb_loc = buff.find(END_BLOCK, sb_loc+1);

                                if (eb_loc != std::string::npos) {
                                        inputStream << buff.substr(sb_loc + 1, eb_loc - 1 - sb_loc);

                                        mStrings.back() = inputStream.str();
                                        inputStream.str("");

                                        started = false;

                                        std::string::size_type em_loc = buff.find(END_MSG, eb_loc+1);
                                        if (em_loc != std::string::npos) {
                                                pending = false;
                                        } else {
                                                inputStream << buff.substr(eb_loc+1);
                                        }

                                } else {
                                        inputStream << buff.substr(sb_loc + 1);
                                }
                        }

                } else {
                        std::string::size_type eb_loc = buff.find(END_BLOCK, 0);

                        if (eb_loc != std::string::npos) {
                                inputStream << buff.substr(0, eb_loc - 1);
                                started = false;

                                mStrings.back() = inputStream.str();
                                inputStream.str("");

                                std::string::size_type em_loc = buff.find(END_MSG, eb_loc+1);
                                if (em_loc != std::string::npos) {
                                        pending = false;
                                } else {
                                        inputStream << buff.substr(eb_loc+1);
                                }
                        } else {
                                inputStream << buff;
                        }
                }
        }
        bool confirmado = false;
        std::list<std::string>::iterator it;
        unsigned int i;
        for ( i = 1, it = mStrings.begin(); it != mStrings.end(); i++, ++it) {
                std::string& s = *it;

                if (i != msgNum || !pending) {
                        //std::cout << "Mensaje :\n" << s << std::endl;
                        try {
                                GIL::Hl7Parser::Messages::ACKInterpreter ai(GIL::HL7::Message::FromER7(s));
                                if (msgControlId != ai.GetReferralMessageControlId()) {
                                        throw GIL::HL7::HL7Exception(_Std("Error in the logic of confirmation. The control of the message ID does not match recognized"));
                                }
                                if (!ai.IsSuccess()) {
                                        std::stringstream errmsg;
                                        errmsg << _Std("NACK received:") << ai.ERR.GetErrorCodeDescription();
                                        throw GIL::HL7::HL7Exception(errmsg.str());
                                }
                                confirmado = true;
                        } catch (GIL::HL7::HL7Exception& e) {
                                throw GIL::HL7::HL7Exception(_Std("Failed delivery confirmation:") + e.GetCause(), "COMM");
                        }
                        break;
                }
        }
        if (!confirmado) {
                throw GIL::HL7::HL7Exception(_Std("Error in the confirmation of delivery: Delivery is not recognized by the remote end"), "COMM");
        }
        //std::cout << "Total: " << ep.GetTotalBytesReaded() << std::endl;*/
}

}
