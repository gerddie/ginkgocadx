/*
*  
*  $Id: render.h $
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
		namespace Events {

			class EXTAPI EventoRender : public GNC::GCS::Events::IEvent
			{
			public:

				EventoRender(GNC::GCS::IVista* pVista, int sliceAfectado = -1) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_Render,0,100,pVista)
				{
					m_Nombre = "Render";
					m_SliceAfectado = sliceAfectado;
				}

				~EventoRender()
				{
				}

				virtual void pushInfo(std::ostream& out) const {
					out << "Slice = " << m_SliceAfectado;
				}

			protected:
				int              m_SliceAfectado;
			};

		}
	}
}
