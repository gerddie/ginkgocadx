/*
 *  
 *  $Id: ioverlay.h $
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

#include <api/math/geometry.h>

class vtkGinkgoImageViewer;
namespace GNC {
	namespace GCS {
		namespace Widgets {
			class IWidget;
		}
	}
}

namespace GNC {
	namespace GCS {
		
		/* Interfaz que se ocupa de imprimir (OpenGL + Cairo) datos referidos a un widget o a un widgetsRenderer */
		class IImpresorOverlay {
		public:
			virtual ~IImpresorOverlay() {}

			virtual std::string ImprimirOverlay(GNC::GCS::Widgets::IWidget* w, vtkGinkgoImageViewer* pViewer) = 0;
		};
		
	}
}
