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
#include <api/iwidgets.h>
#include <api/observers/iwidgetsobserver.h>
#include <api/controllers/ieventscontroller.h>
#include <vector>
#include <list>
#include <set>

namespace GNC {
	namespace GCS {

		namespace Widgets {

			class WRejilla;

			class WRejillaBuilder : public GNC::GCS::Widgets::IWidgetBuilder, public GNC::GCS::Widgets::IObservadorWidget {
			public:

				WRejillaBuilder(GNC::GCS::IWidgetsManager* pManager, long gid, double escalaX = 50, double escalaY = 50);

				~WRejillaBuilder();

				virtual void OnWidgetDestruido(IWidget*);

				virtual bool IsRejillaVisible();

				virtual void ActivarRejilla(bool activar);

				virtual void SetEscalaX(double escala);
				virtual void SetEscalaY(double escala);

				virtual double GetEscalaX();
				virtual double GetEscalaY();

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

			protected:
				GNC::GCS::Vector              m_PosicionCursor;
				WRejilla*                     m_pRejillaActiva;
				double									m_EscalaX;
				double									m_EscalaY;

			};

			//----------------------------------------------------------------------------

			class WRejilla : public GNC::GCS::Widgets::IWidget {

			protected:


			public:

			//region "Constructor y destructor"

			public:

				WRejilla(IWidgetsManager* pManager, double escalaX, double escalaY, long vid, const char* nombre, long gid);

				~WRejilla();

			//endregion

			//region "Interfaz generica"

				virtual bool SoportaAutoBorrado() const {
					return false;
				}

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual void Seleccionar(bool seleccionado);

				virtual void Iluminar(bool iluminado);

				virtual void Ocultar(bool oculto);

				virtual void SetEscalaX(double escala);
				virtual void SetEscalaY(double escala);

				virtual double GetEscalaX();
				virtual double GetEscalaY();

			//endregion

			//region "Estado interno"

			private:


			//region "Propiedades geometricas"
			private:
				double m_EscalaX;
				double m_EscalaY;

			public:

			//endregion

			//endregion

			};
		}
	}
}

