/*
 *  
 *  $Id$
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once

#ifndef IOVERLAYSCONTRACT_H
#define IOVERLAYSCONTRACT_H
#include <list>
#include <api/icontract.h>
#include <api/autoptr.h>
#include <visualizator/estudios/overlay.h>

class vtkGinkgoImageViewer;
class vtkLookupTable;
class vtkAlgorithmOutput;
namespace GNC {
	namespace GCS {
		class IVista;
	}
}
namespace GNKVisualizator {
	class IOverlaysContract: public GNC::GCS::IContract
	{
	private:
		GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>  m_pListaOverlays;
	public:
		GNKVisualizator::TEstadoOverlay m_EstadoOverlays;
		GNC::GCS::IVista* m_pView;
	
		GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& GetOverlayList();

		IOverlaysContract();
		~IOverlaysContract();
		virtual void Setup(GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& pListaOverlays, GNC::GCS::IVista* pView);
	};
}
#endif
