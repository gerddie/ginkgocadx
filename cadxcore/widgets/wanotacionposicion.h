/*
 *  
 *  $Id: wanotacionposicion.h $
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
#include <api/controllers/ieventscontroller.h>
#include <vector>

class vtkImageData;

namespace GNC {
	namespace GCS {
		class IAnotador;
		namespace Widgets {

			//---------------------------------------------------------------------

			class WAnotacionPosicion : public GNC::GCS::Widgets::IWidget {


			//region "Constructor y destructor"

			public:
				WAnotacionPosicion(IWidgetsManager* pManager, long vid, const char* nombre, long gid);

				~WAnotacionPosicion();

				//endregion

				//region "Interfaz especifica"

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual void Seleccionar(bool seleccionado);

				virtual void Iluminar(bool iluminado);

				virtual void Ocultar(bool oculto);

				//endregion

				//region "Interfaz especifica"
			public:
				//endregion
			};
		}
	}
}
