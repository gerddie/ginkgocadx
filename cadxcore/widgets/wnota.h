/*
 *  
 *  $Id: wnota.h $
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
#include <cmath>
#include <api/iwidgets.h>
#include <api/widgetserializable.h>
#include <widgets/openglhelper.h>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations
namespace GNC {
	namespace GCS {
		namespace Widgets {
			
			namespace Dialogos {
				class SeleccionTexto;
			}
			
			class WTextBox;
		}
	}
}
//endregion

namespace GNC {
	namespace GCS {
		namespace Widgets {

			namespace Dialogos {
				class SeleccionTexto;
			}

			class WTextBox;

			//====================================================================================================
			//= Builder de notas
			//====================================================================================================
			class WNotaBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:
				typedef GNC::GCS::Vector TVector;
				WNotaBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask);

				~WNotaBuilder();

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

			protected:
				TVector m_Posicion;
				GNC::GCS::Widgets::IWidgetBuilder::Estado m_Estado;
				bool m_MouseDown;

				WTextBox* m_pTempWidget;

			};
		}
	}
}
