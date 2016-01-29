/*
 *  
 *  $Id: wwindowlevel.h $
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

#include <api/iwidgets.h>
#include <widgets/observers/iobservadorwindowlevel.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {
			//---------------------------------------------------------------------

			class WWindowLevelBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Widgets::Observadores::IObservadorWindowLevel TObservador;

				WWindowLevelBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

				void SetObservador(TObservador* pObservador) {
					m_pObservador = pObservador;
				};

				~WWindowLevelBuilder();

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

			protected:
				TObservador*             m_pObservador;
				TVector                  m_NodoMoviendose;  // Cursor
				TVector                  m_Start;           // Origen del trazado de la recta
				bool                     m_MouseDown;
				bool                     m_Changed;
			};
		};
	};
};
