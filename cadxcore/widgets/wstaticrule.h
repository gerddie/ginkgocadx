/*
 *  
 *  $Id: wanotacionesquina.h $
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
#include <api/iwidgets.h>
#include <api/api.h>
#include <api/observers/ieventsobserver.h>
#include <vector>
#include <list>
#include "openglhelper.h"


namespace GNC {
	namespace GCS {

		namespace Widgets {

			class EstadoInterno;
			class Anotaciones;

			//----------------------------------------------------------------------------------------------------
			//region Widget anotador
			class EXTAPI WStaticRule : public GNC::GCS::Widgets::IWidget, public GNC::GCS::IEventsObserver {
			public:

				WStaticRule(IWidgetsManager* pManager, long vid, const char* nombre, long gid);

				~WStaticRule();

			//endregion

			//----------------------------------------------------------------------------------------------------
			//region "Interfaz generica"
			public:
				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual void OffscreenRender(Contexto3D* c);

				virtual void Modificar(bool modificar);

				virtual void Seleccionar(bool seleccionado);

				virtual void Iluminar(bool iluminado);

				virtual void Ocultar(bool oculto);


			//endregion

				//----------------------------------------------------------------------------------------------------
				//region "Interfaz especifica"
			public:				

			//----------------------------------------------------------------------------------------------------
			//region "Interfaz de eventos ginkgo"
			public:

				virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

			//endregion

			//----------------------------------------------------------------------------------------------------
			//region "Estado interno"
			protected:
				GNC::GCS::GLHelper::TColor m_color;

			//endregion

			};
			//endregion
		}
	}
}
