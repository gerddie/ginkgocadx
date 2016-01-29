/*
 *  
 *  $Id: iobservadorchroma.h $
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
				class IObservadorChroma {
				public:
					virtual ~IObservadorChroma(){};

					virtual void OnKeyEvent(GNC::GCS::Events::EventoTeclado & evt) = 0;
				};
			}

			namespace Notificadores {
				class INotificadorChroma {

				protected:
					GNC::GCS::Widgets::Observadores::IObservadorChroma* m_pObservadorChroma;

				public:
					INotificadorChroma() {
						m_pObservadorChroma = NULL;
					}

					INotificadorChroma(GNC::GCS::Widgets::Observadores::IObservadorChroma* pObservador) {
						m_pObservadorChroma = pObservador;
					}

					virtual ~INotificadorChroma() {};

				public:
					void SetObservador(GNC::GCS::Widgets::Observadores::IObservadorChroma* pObservador)
					{
						m_pObservadorChroma = pObservador;
					}

					virtual void NotificarKeyEvent(GNC::GCS::Events::EventoTeclado& evt) {
						if (m_pObservadorChroma != NULL) {
							m_pObservadorChroma->OnKeyEvent(evt);
						}
					}
				};
			}
		}
	}
}
