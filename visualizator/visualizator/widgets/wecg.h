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
#include <api/observers/iwidgetsobserver.h>
#include <api/controllers/ieventscontroller.h>
#include <visualizator/estudios/ecgstudy.h>

namespace GNC {
	namespace GCS {
		class IContextoEstudio;
		class TexturaCairo;
	}
}

namespace GNKVisualizator {

	namespace Widgets {

		class WECG;
		class Data;

		class WECGBuilder : public GNC::GCS::Widgets::IWidgetBuilder, public GNC::GCS::Widgets::IObservadorWidget {
		public:

			WECGBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

			~WECGBuilder();

			virtual void OnWidgetDestruido(GNC::GCS::Widgets::IWidget*);

			virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

			virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

			virtual void Render(GNC::GCS::Contexto3D* c);

			void AutoCallibrateImage();

		protected:
			GNC::GCS::Vector          m_PosicionCursor;
			WECG*                     m_pRejillaActiva;				

		};

		//----------------------------------------------------------------------------

		typedef struct TexturasCanalECG
		{
			GNC::GCS::TexturaCairo* tl;

			TexturasCanalECG();
			~TexturasCanalECG();

			void Destruir();

		} TexturasCanalECG;

		class RecursosECG {
		public:
			typedef std::vector<TexturasCanalECG*> TListaTexturas;
				
			TListaTexturas Texturas;

			RecursosECG();

			~RecursosECG();

			void Alloc(size_t csize);

			void Dealloc();
		};

		class WECG : public GNC::GCS::Widgets::IWidget {

		protected:
			
		typedef std::map<void*, RecursosECG*> TMapaRecursos;

		public:

		//region "Constructor y destructor"

		public:

			WECG(GNC::GCS::IWidgetsManager* pManager, long vid, const char* nombre = "WECG");

			~WECG();

		//endregion

		//region "Interfaz generica"

			virtual bool SoportaAutoBorrado() const {
				return false;
			}

			virtual void AutoCallibrate();

			virtual void Reload(const GNKVisualizator::ECGStudy::TListChannelInfo& listInfoChannels);

			virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

			virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

			virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

			virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

			virtual void Render(GNC::GCS::Contexto3D* c);

			virtual void Seleccionar(bool seleccionado);

			virtual void Iluminar(bool iluminado);

			virtual void Ocultar(bool oculto);

			virtual void LiberarRecursos(GNC::GCS::IWidgetsRenderer* pKey);

		//endregion

		//region "Estado interno"

		private:


		//region "Propiedades geometricas"
		private:
			Data* CData;
			TMapaRecursos m_mapaRecursos;

		public:

		//endregion

		//endregion

		};
	}
}

