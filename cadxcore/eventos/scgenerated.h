/*
*  
*  $Id: views.h $
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

namespace GNC {
	namespace GCS {
		class IVista;
		namespace Events {

			class EXTAPI SCGeneratedEvent : public GNC::GCS::Events::IEvent
			{		

			public:
				SCGeneratedEvent(GNC::GCS::IVista* pView = NULL, const std::list<long>& newSeriesId = std::list<long>()) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_SCGenerated, 0, 100, NULL),
					m_pView(pView),
					m_seriesId(newSeriesId)
				{
					m_Nombre = "SCGeneratedEvent";	
				}

				~SCGeneratedEvent()
				{
				}

				const std::list<long>& GetSeriesId() {
					return m_seriesId;
				}

				GNC::GCS::IVista* GetView()
				{
					return m_pView;
				}				

				virtual void pushInfo(std::ostream& /*out*/) const {
				}

				private:
					GNC::GCS::IVista* m_pView;
					std::list<long> m_seriesId;
			};
		}
	}
}
