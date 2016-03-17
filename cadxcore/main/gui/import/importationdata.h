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

#include <string>
#include <vector>
#include <api/autoptr.h>
#include <api/imodelointegracion.h>


namespace GNC
{
namespace GUI
{
class EXTAPI ImportationData
{
public:
        typedef std::list<std::string> TListOfFiles;
        GNC::GCS::Ptr<GIL::IModeloIntegracion> m_pIntegrationModel;
        GIL::DICOM::DicomDataset baseImages;
        TListOfFiles m_SourceFiles;

        ImportationData()
        {
                m_pIntegrationModel = GNC::GCS::Ptr<GIL::IModeloIntegracion>(new GIL::IModeloIntegracion());
        }

        ImportationData(GNC::GCS::Ptr<GIL::IModeloIntegracion>& pModeloIntegracion)
        {
                if (pModeloIntegracion.IsValid()) {
                        m_pIntegrationModel = pModeloIntegracion;
                } else {
                        m_pIntegrationModel = GNC::GCS::Ptr<GIL::IModeloIntegracion>(new GIL::IModeloIntegracion());
                }
        }

        virtual ~ImportationData()
        {
        }
};
};
};
