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

			class EXTAPI EventView : public GNC::GCS::Events::IEvent
			{		

			public:
				typedef enum {
					Creation,
					Initialized,
					Destruction
				} EventViewType;

				EventView(GNC::GCS::IVista* pView = NULL, EventViewType type = Creation) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_Views, 0, 100, NULL)
				{
					m_Nombre = "EventView";	
					m_pView = pView;
					m_type = type;
				}

				~EventView()
				{
				}

				const EventViewType GetType() {
					return m_type;
				}

				GNC::GCS::IVista* GetView()
				{
					return m_pView;
				}				

				virtual void pushInfo(std::ostream& /*out*/) const {
				}

				private:
					GNC::GCS::IVista* m_pView;
					EventViewType m_type;

			};
		}
	}
}
