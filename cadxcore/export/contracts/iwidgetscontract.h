#pragma once
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

#ifndef IWIDGETSCONTRACT_H
#define IWIDGETSCONTRACT_H

#include <list>

#include <api/api.h>
#include <api/icontract.h>

class vtkGinkgoImageViewer;

namespace GNC {
	namespace GCS {
		class IVista;
		class IWidgetsManager;
	}
}

namespace GNC {
	namespace GCS {
		class Contexto3D;

		class EXTAPI IWidgetsContract : public IContract {
		public:

			typedef std::list<vtkGinkgoImageViewer*> TViewerList;

			IWidgetsContract();

			~IWidgetsContract();

			virtual GNC::GCS::IWidgetsManager* GetManager() = 0;

			virtual void GetAllViewers(TViewerList& viewerList);

			bool Inicializado();

			//Anotation section
			//this method processes special keys like: ${VOLUME.IDX} ${SLICE.WIDTH}...
			virtual std::string GetAnnotationValue(GNC::GCS::Contexto3D* c, const std::string& key);

			//return dicom tag value from key
			virtual std::string GetTagValue(GNC::GCS::Contexto3D* c, const std::string& key);

			vtkGinkgoImageViewer* GetViewerActivo();
		};
	}
}
#endif
