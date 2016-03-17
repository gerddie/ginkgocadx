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
#include <api/icommand.h>
#include <main/controllers/controladorbbddhl7.h>

namespace GADAPI
{
class SendHL7CommandParams : public GNC::GCS::IComandoParams
{

public:
        SendHL7CommandParams(const std::list<GIL::HL7::MensajeHL7>& messages);

        virtual ~SendHL7CommandParams();

public:
        std::list<GIL::HL7::MensajeHL7> messages;
        wxSemaphore* m_pSemaphore;
        int m_idLastSock;
};


class SendHL7Command : public GNC::GCS::IComando
{
public:
        SendHL7Command(SendHL7CommandParams* pParams);

protected:
        virtual void Execute();
        virtual void Update();
        virtual void OnAbort();

protected:
        /* Envia el mensaje por MLLP */
        void EnviarMensajeMLLP(const std::string& msg, const std::string& url, bool procesarACK=true, const std::string& msgControlId="") ;

        SendHL7CommandParams* m_pSendParams;
};
}
