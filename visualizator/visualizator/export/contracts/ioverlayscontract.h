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

#ifndef IOVERLAYSCONTRACT_H
#define IOVERLAYSCONTRACT_H
#include <list>
#include <api/icontract.h>
#include <api/autoptr.h>
#include <visualizator/estudios/overlay.h>

class vtkGinkgoImageViewer;
class vtkLookupTable;
class vtkAlgorithmOutput;
namespace GNC
{
namespace GCS
{
class IVista;
}
}
namespace GNKVisualizator
{
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
