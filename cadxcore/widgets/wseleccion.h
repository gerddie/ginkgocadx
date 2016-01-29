/*
 *  
 *  $Id: wseleccion.h $
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
#include <api/math/geometry3d.h>
#include <vector>


namespace GNC {
	namespace GCS {
		namespace Events {
			class EventoRaton;
			class EventoTeclado;
		}
	}
}


namespace GNC {
	namespace GCS {
		namespace Widgets {
			//---------------------------------------------------------------------

			class WSeleccionBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Events::EventoRaton    TEventoRaton;
				typedef GNC::GCS::Events::EventoTeclado  TEventoTeclado;
				typedef GNC::GCS::IWidgetsManager        TWidgetsManager;
				typedef GNC::GCS::Widgets::IWidget       TWidget;

				WSeleccionBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

				~WSeleccionBuilder();

				virtual void OnMouseEvents(TEventoRaton& event);
				virtual void OnKeyEvents(TEventoTeclado& event);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

				//region "Helpers"

				bool         m_Dentro;
				//endregion


			protected:
				TVector  m_NodoMoviendose;  // Cursor
				TVector  m_Start;           // Origen del boundingBox
				TVector  m_Stop;            // Destino del boundingBox				

				TVector  m_BB[4]; // Vertices del bounding box.

				bool     m_MouseDown;
				Estado   m_Estado;
			};
		}
	}
}
