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

#include <vector>
#include <list>
#include <set>

#include <api/iwidgets.h>
#include <api/observers/ieventsobserver.h>
#include <api/observers/iwidgetsobserver.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

namespace GNC {
	namespace GCS {
		class IWidgetsRenderer;
	}
}


namespace GNC {
	namespace GCS {

		namespace Widgets {

			class WLupa;

			typedef enum ModoLupa {
				ML_Aumentar,
				ML_Disminuir
			} ModoLupa;


			class WLupaBuilder : public GNC::GCS::Widgets::IWidgetBuilder, public GNC::GCS::Widgets::IObservadorWidget {
			public:

				WLupaBuilder(GNC::GCS::IWidgetsManager* pManager, GNC::GCS::Widgets::ModoLupa modo, const GNC::GCS::TriggerButton& buttonMask, long gid);

				~WLupaBuilder();

				virtual void OnWidgetDestruido(IWidget*);

				virtual void SetModo(GNC::GCS::Widgets::ModoLupa modo);

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

			protected:
				GNC::GCS::Vector              m_PosicionCursor;
				GNC::GCS::Widgets::ModoLupa   m_ModoActivo;
				WLupa*                        m_pLupaActiva;

			};

			//----------------------------------------------------------------------------

			class WLupa : public GNC::GCS::Widgets::IWidget, public GNC::GCS::IEventsObserver {

			protected:


			public:
				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Matriz TMatriz;

				float               m_AnguloVertices;
				unsigned int        m_NumVertices;
				GNC::GCS::Vector*   m_VInteriores;
				GNC::GCS::Vector*   m_VExteriores;
				GNC::GCS::Vector*   m_VCoordsInteriores;
				GNC::GCS::Vector    m_Centro;

			//region "Constructor y destructor"

			public:

				WLupa(GNC::GCS::IWidgetsRenderer* pRenderer, const GNC::GCS::Vector& relacionImagenPantalla, GNC::GCS::Vector& puntoAnclaje, ModoLupa modo, IWidgetsManager* pManager, long vid, const char* nombre, long gid);

				~WLupa();

			//endregion

			//region "Interfaz especifica"
			private:
				GNC::GCS::Widgets::ModoLupa   m_ModoActivo;

			public:

				void SetRendererActivo( GNC::GCS::IWidgetsRenderer* r);

				void SetModo(GNC::GCS::Widgets::ModoLupa modo);

				void SetCenter(const GNC::GCS::Vector& punto)
				{
					m_Centro = punto;
				}

				const GNC::GCS::Vector& GetCenter() const
				{
					return m_Centro;
				}

				void SetPosCursor(const GNC::GCS::Vector& punto)
				{
					m_PosCursor = punto;
				}

				const GNC::GCS::Vector& GetPosCursor()
				{
					return m_PosCursor;
				}

			//endregion

			//region "Interfaz generica"

				virtual void Modificar(bool modificar);

				virtual bool SoportaAutoBorrado() const {
					return true;
				}

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

			//endregion

			//region "Estado interno"

			private:
				bool              m_MouseDown;
				bool              m_LDown;
				bool              m_RDown;
				GNC::GCS::Vector  m_PosicionCursor;
			//endregion

				bool              m_Dentro;

			public:
				void SetMouseDown(bool estado) {
					m_MouseDown = estado;
				}


			//region "Propiedades geometricas"
			private:
				GNC::GCS::Vector  m_RelacionImagenPantalla;

				float             m_RadioLente;
				float             m_RadioLentePantalla;
				float             m_GrosorLupa;
				float             m_GrosorLupaPantalla;
				float             m_GrosorMango;
				float             m_GrosorMangoPantalla;

				float             m_AnguloPosAsa;
				float             m_AnguloAsa;

			public:

				void Desplazar(const GNC::GCS::Vector& delta);
				void DesplazarA(const GNC::GCS::Vector& posicion);

				void Recalcular();

			public:

			//endregion

			//region Interfaz de eventos ginkgo

				virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

			//endregion

			};
		}
	}
}
