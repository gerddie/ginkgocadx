/*
 *  
 *  $Id: wbrightnesscontrast.h $
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

namespace GNC {
	namespace GCS {
		namespace Widgets {
			//---------------------------------------------------------------------

			class WBrightnessContrastBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				typedef GNC::GCS::Vector TVector;

				WBrightnessContrastBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

				~WBrightnessContrastBuilder();

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

			protected:

				void AdjustBrightnessContrast(const TVector& offset);

				TVector            m_StartBCPoint;
				TVector            m_NodoMoviendose;  // Cursor
				bool               m_Changed;
				TVector            m_Start;           // Origen del trazado
				TVector            m_Stop;           // Fin del trazado

				bool               m_MouseDown;
			};
		}
	}
}
