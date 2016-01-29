/*
 *  
 *  $Id: wpan.h $
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
#include <vector>

#include <export/tools/itoolslider.h>
#include <api/iwidgets.h>
#include <api/math/geometry3d.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {
			//---------------------------------------------------------------------

			class WPanBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				typedef GNC::GCS::Vector                    TVector;
				typedef GNC::GCS::Vector3D                  TVector3D;
				typedef GNC::GCS::Events::EventoRaton      TEventoRaton;
				typedef GNC::GCS::Events::EventoTeclado    TEventoTeclado;
				typedef GNC::GCS::IWidgetsManager           TWidgetsManager;
				typedef GNC::GCS::Widgets::IWidget          TWidget;

				WPanBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

				~WPanBuilder();

				virtual void OnMouseEvents(TEventoRaton& event);
				virtual void OnKeyEvents(TEventoTeclado& event);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

				//region "Helpers"

				bool         m_Dentro;
				//endregion


			protected:
				TVector   m_Start;
				bool      m_Changed;
				TVector   m_NodoMoviendose;  // Cursor	
				
				bool      m_MouseDown;
				Estado    m_Estado;
			};
		}
	}
}
