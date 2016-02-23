/*
 * This file is part of Ginkgo CADx
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

namespace GNC {
	namespace GCS {
		namespace Widgets {
			
			//region "Forward declarations"
			
			class IWidget;
			
			//endregion
			
			//-----------------------------------------------------------------------------------------------
			
			class IObservadorWidget {
			public:
				
				long GID; // GID de los widgets a escuchar (filtro por tipo)
				long VID; // VID de los widgets a escuchar (VistaID)
				
				/* Construye un observador que escucha eventos de widgets con un gid especifico */
				IObservadorWidget(long vid, long gid=0)
				{
					SetVID(vid);
					SetGID(gid);
				}

				virtual ~IObservadorWidget()
				{

				}
				
				virtual void SetVID(long vid)
				{
					VID = vid;
				}
				
				virtual void SetGID(long gid)
				{
					GID = gid;
				}
				
				virtual void OnWidgetCreado(IWidget*) {}
				
				virtual void OnWidgetModificado(IWidget*) {}
				
				virtual void OnWidgetDestruido(IWidget*) {}
				
				virtual void OnWidgetSeleccionado(IWidget*) {}
				
				virtual void OnWidgetDeseleccionado(IWidget*) {}
			};
		}
	}
}
