/*
 *  
 *  $Id: iwidgetsobserver.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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
