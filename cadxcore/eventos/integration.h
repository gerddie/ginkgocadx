/*
*  
*  $Id: integration.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#pragma once

#include <api/ievento.h>
#include "eventosginkgo.h"

#include <api/imodelointegracion.h>
#include <api/autoptr.h>


namespace GNC {
	namespace GCS {
		namespace Events {

			class EXTAPI HL7SendRequestEvent : public GNC::GCS::Events::IEvent
			{
			private:
				std::string m_Message;

			public:
				HL7SendRequestEvent() : GNC::GCS::Events::IEvent(ginkgoEVT_Core_HL7SendRequest, 0, 100, NULL)
				{
					m_Nombre = "HL7SendRequest";					
				}

				HL7SendRequestEvent(const std::string& msg): GNC::GCS::Events::IEvent(ginkgoEVT_Core_HL7SendRequest, 0, 100, NULL){
					this->m_Nombre  = "HL7SendRequest";
					this->m_Message = msg;

				}

				~HL7SendRequestEvent()
				{
				}

				const std::string& GetMessage() const
				{
					return m_Message;
				}

				virtual void pushInfo(std::ostream& out) const {
					out << "HL7SendRequest ()";
				}
			};
		}
	}
}
