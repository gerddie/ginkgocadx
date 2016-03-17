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
#include <api/ientorno.h>
#include <api/imodelohl7.h>

namespace GIL
{
namespace HL7
{
class HiloEnviaMensajesHl7;

class ControladorHL7
{
public:
public:

        typedef enum TipoProtocolo {
                TP_MLLP
        } TipoProtocolo;


        ControladorHL7(TipoProtocolo protocolo = TP_MLLP);
        virtual ~ControladorHL7();

        //region "Interfaz generica"

        /* Envia el mensaje por el protocolo especificado */
        virtual void EnviarMensaje(const GIL::HL7::Message& msg, bool procesarACK=true) const;

        //endregion

        //region "Helpers"

        //endregion

        void InsertarMensajeBBDD(const GIL::HL7::Message& msg, const std::string& url,const bool procesarACK,const int protocolo,const std::string& msgControlId) const;

protected:
        TipoProtocolo      m_Protocolo;
};
}
}
