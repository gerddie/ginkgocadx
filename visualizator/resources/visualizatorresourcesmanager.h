/*
 *  
 *  $Id: visualizatorresourcemanager.h $
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
#include <wx/bitmap.h>

namespace GNKVisualizator {
	 class ResourcesManager {

 		public:
 		class ToolIcons {
		public:
			static wxBitmap GetIcoCapas();
			static wxBitmap GetIcoReset();
			static wxBitmap GetIcoMenuImagen();
			static wxBitmap GetIcoECGMetadata();
 		};
		
		class T3DBar {
		public:
			static wxBitmap GetIco3DArrow();		
			static wxBitmap GetIcoStereo();	
			static wxBitmap GetIcoPin();	
			static wxBitmap GetIcoDemo();
		};

		class SynchronizationBar {
		public:
			static wxBitmap GetIcoDessincronizar();
		};

		class Logos {
		public:
			static wxBitmap GetIcoVisualizator();
		};


	 };
}
