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

namespace GNC {
	namespace GCS {

		class IComando;

		namespace Events {

			class EXTAPI EventoProgresoComando : public GNC::GCS::Events::IEvent
			{
			public:

				typedef enum
				{
					TEP_Iniciado,
					TEP_Progreso,
					TEP_Finalizado,
					TEP_Unknown
				} TipoEventoProgreso;

				EventoProgresoComando(IComando* pComando = NULL, TipoEventoProgreso tipo = TEP_Unknown, float progresoNormalizado = 0.0f, const std::string& texto = "") : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ProgresoComando, 0, 100, NULL)
				{
					m_Nombre = "ProgresoComando";
					m_pComando = pComando;
					m_Tipo = tipo;
					m_ProgresoNormalizado = progresoNormalizado;
					m_Texto = texto;
				}

				~EventoProgresoComando()
				{
				}

				virtual void pushInfo(std::ostream& out) const {
					out << "Tipo = ";
					switch (m_Tipo) {
						case TEP_Iniciado:
							out << "Iniciado";
							break;
						case TEP_Progreso:
							out << "Progreso";
							break;
						case TEP_Finalizado:
							out << "Finalizado";
							break;
						case TEP_Unknown:
						default:
							out << "Unknown";
							break;
					}
					out << ", Progreso = " << m_ProgresoNormalizado << ", Texto = " << m_Texto << ", Comando = " << m_pComando;
				}

				IComando* GetComando() const
				{
					return m_pComando;
				}

				TipoEventoProgreso GetTipo() const
				{
					return m_Tipo;
				}

				float GetProgresoNormalizado() const
				{
					return m_ProgresoNormalizado;
				}

				const std::string& GetTexto() const
				{
					return m_Texto;
				}

			protected:
				GNC::GCS::IComando*             m_pComando;
				TipoEventoProgreso              m_Tipo;
				float                           m_ProgresoNormalizado;
				std::string                     m_Texto;

			};

		}
	}
}
