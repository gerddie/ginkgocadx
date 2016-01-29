#pragma once
/*
*
*  $Id: iwidgetscontract.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
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