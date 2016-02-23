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
