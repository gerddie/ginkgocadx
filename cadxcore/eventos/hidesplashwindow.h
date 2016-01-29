/*
*  
*  $Id: hidesplashwindow.h 5047 2012-07-25 13:14:57Z carlos $
*  Ginkgo CADx Project
*
*  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
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
		namespace Events {

			class EXTAPI EventHideSplashWindow : public GNC::GCS::Events::IEvent
			{
			public:

				EventHideSplashWindow(GNC::GCS::IVista* pVista = NULL) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_HideSplashWindow, 0, 100, pVista)
				{
					m_Nombre = "EventHideSplashWindow";
				}

				~EventHideSplashWindow()
				{
				}

				virtual void pushInfo(std::ostream& /*out*/) const {
				}

			protected:
			};

		}
	}
}
