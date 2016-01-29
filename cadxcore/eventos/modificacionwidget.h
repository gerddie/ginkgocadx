/*
*  
*  $Id: modificacionwidget.h $
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

		namespace Widgets {
			class IWidget;
		}

		namespace Events {

			class EXTAPI EventoModificacionWidget : public GNC::GCS::Events::IEvent
			{
			public:

				typedef enum {
					WidgetCreado,
					WidgetModificado,
					WidgetDestruido
				} TipoModificacion;

				EventoModificacionWidget(GNC::GCS::IVista* pVista = NULL, TipoModificacion tipo = WidgetModificado, GNC::GCS::Widgets::IWidget* w = NULL) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ModificacionWidget,0,100,pVista)
				{
					m_Nombre = "ModificacionWidget";
					m_pWidget = w;
					m_Tipo = tipo;
				}

				~EventoModificacionWidget()
				{
				}

				virtual void pushInfo(std::ostream& out) const {
					out << "Tipo = ";
					switch (m_Tipo) {
						case WidgetCreado:
							out << "WidgetCreado";
							break;
						case WidgetModificado:
							out << "WidgetModificado";
							break;
						case WidgetDestruido:
							out << "WidgetDestruido";
							break;
					}
				}

				GNC::GCS::Widgets::IWidget* GetWidget()
				{
					return m_pWidget;
				}

				TipoModificacion GetTipo()
				{
					return m_Tipo;
				}

			protected:
				TipoModificacion            m_Tipo;
				GNC::GCS::Widgets::IWidget* m_pWidget;
			};
		}
	}
}
