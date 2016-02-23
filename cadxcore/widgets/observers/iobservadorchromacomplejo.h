/*
 *  Ginkgo CADx Project
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
