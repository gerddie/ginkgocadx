/*
 *  
 *  $Id: iobservadorchromacomplejo.h $
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
#include <api/ievento.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {
			namespace Observadores {

				/* Observador especifico de la herramienta lazo */
				class IObservadorChromaComplejo {
				public:
					virtual ~IObservadorChromaComplejo(){}

					virtual void OnKeyEvent(GNC::GCS::Events::EventoTeclado & evt) = 0;
				};
			}

			namespace Notificadores {
				class INotificadorChromaComplejo {

				protected:
					GNC::GCS::Widgets::Observadores::IObservadorChromaComplejo* m_pObservadorChroma;

				public:
					virtual ~INotificadorChromaComplejo() {}

					INotificadorChromaComplejo() {
						m_pObservadorChroma = NULL;
					}

					INotificadorChromaComplejo(GNC::GCS::Widgets::Observadores::IObservadorChromaComplejo* pObservador) {
						m_pObservadorChroma = pObservador;
					}

				public:
					void SetObservador(GNC::GCS::Widgets::Observadores::IObservadorChromaComplejo* pObservador)
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
