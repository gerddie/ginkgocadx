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
