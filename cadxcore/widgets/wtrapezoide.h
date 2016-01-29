/*
 *  
 *  $Id: wtrapezoide.h $
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
#include <api/widgetserializable.h>
#include <vector>

class vtkImageData;

namespace GNC {
	namespace GCS {
		namespace Widgets {
			//---------------------------------------------------------------------

			class WTrapezoideBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Events::EventoRaton   TEventoRaton;
				typedef GNC::GCS::Events::EventoTeclado TEventoTeclado;
				typedef GNC::GCS::IWidgetsManager        TWidgetsManager;
				typedef GNC::GCS::Widgets::IWidget       TWidget;

				WTrapezoideBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid, int numMaxTrapezoides = 0);

				~WTrapezoideBuilder();

				virtual void OnMouseEvents(TEventoRaton& event);
				virtual void OnKeyEvents(TEventoTeclado& event);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

				//region "Helpers"

				bool         m_Dentro;
				//endregion


			protected:
				TVector                  m_NodoMoviendose;  // Cursor
				TVector                  m_Start;           // Origen del boundingBox
				TVector                  m_Stop;            // Destino del boundingBox
				int                      m_NumMaxTrapezoides;

				TVector                  m_Vertices[4]; // Vertices del bounding box.

				bool                     m_MouseDown;
				Estado                   m_Estado;
			};

		}
	}
}
