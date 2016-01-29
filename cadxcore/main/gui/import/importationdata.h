/*
 *  
 *  $Id: importationdata.h $
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

#include <string>
#include <vector>
#include <api/autoptr.h>
#include <api/imodelointegracion.h>


namespace GNC {
	namespace GUI {
		class EXTAPI ImportationData {
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
