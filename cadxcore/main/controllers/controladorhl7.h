/*
 *  
 *  $Id: controladorhl7.h $
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
#include <api/ientorno.h>
#include <api/imodelohl7.h>

namespace GIL {
	namespace HL7 {
		class HiloEnviaMensajesHl7;

		class ControladorHL7 {
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
