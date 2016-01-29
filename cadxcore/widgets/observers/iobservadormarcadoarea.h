/*
 *  
 *  $Id: iobservadormarcadoarea.h $
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
#include <stddef.h>
#include <api/ievento.h>
namespace GNC {
	namespace GCS {
		namespace Widgets {
			class WMarcadoArea;

			namespace Observadores {

				/* Observador especifico de la herramienta lazo */
				class IObservadorMarcadoArea {
				public:

					virtual ~IObservadorMarcadoArea(){}

					virtual void OnPoligonoCreado(GNC::GCS::Widgets::WMarcadoArea* pPoligono) = 0;
					virtual void OnPoligonoModificado(GNC::GCS::Widgets::WMarcadoArea* pPoligono) = 0;
					virtual void OnPoligonoDestruido(GNC::GCS::Widgets::WMarcadoArea* pPoligono) = 0;
					virtual void OnKeyEvent(GNC::GCS::Events::EventoTeclado & evt) = 0;
				};
			}

			namespace Notificadores {
				class INotificadorMarcadoArea {

				protected:
					GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* m_pObservadorMarcado;

				public:


					INotificadorMarcadoArea() {
						m_pObservadorMarcado = NULL;
					}

					INotificadorMarcadoArea(GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador) {
						m_pObservadorMarcado = pObservador;
					}

					virtual ~INotificadorMarcadoArea(){}

				public:
					void SetObservador(GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador)
					{
						m_pObservadorMarcado = pObservador;
					}

					virtual void NotificarCreacion(WMarcadoArea* pWidget) {
						if (m_pObservadorMarcado != NULL) {
							m_pObservadorMarcado->OnPoligonoCreado(pWidget);
						}
					}

					virtual void NotificarModificacion(WMarcadoArea* pWidget) {
						if (m_pObservadorMarcado != NULL) {
							m_pObservadorMarcado->OnPoligonoModificado(pWidget);
						}
					}

					virtual void NotificarDestruccion(WMarcadoArea* pWidget) {
						if (m_pObservadorMarcado != NULL) {
							m_pObservadorMarcado->OnPoligonoDestruido(pWidget);
						}
					}

					virtual void NotificarKeyEvent(GNC::GCS::Events::EventoTeclado& evt) {
						if (m_pObservadorMarcado != NULL) {
							m_pObservadorMarcado->OnKeyEvent(evt);
						}
					}
				};
			}
		}
	}
}
