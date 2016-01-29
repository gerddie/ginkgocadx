/*
*  
*  $Id: progresocomando.h $
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
