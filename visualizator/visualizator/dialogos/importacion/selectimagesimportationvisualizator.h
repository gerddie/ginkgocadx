/*
 *  
 *  $Id: selectimagesimportationvisualizator.h $
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
#include <api/iwizard.h>
#include <string>
#include <vector>
#include <main/gui/import/importationdata.h>
#include <api/dicom/idicomizador.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <main/gui/import/selectimagesimportation.h>

namespace GNC {
	namespace GCS {
		class IEntorno;
	}
}

namespace GNKVisualizator {
	namespace GUI {

		class SelectImagesImportationVisualizator: public GNC::GUI::SelectImagesImportation
		{
			public:
				SelectImagesImportationVisualizator(wxWindow* pParent,IWizard* pWizard,const GNC::GCS::Ptr<GNC::GUI::ImportationData>& importationData,const std::string &dirTemporal);
				~SelectImagesImportationVisualizator();

				virtual bool Validar();

				virtual bool Dicomizar();

				void RellenarTagsComunes(GIL::DICOM::DicomDataset& base);

				GNC::GCS::Ptr<GNC::GUI::ImportationData> m_pImportationData;
				std::string m_dirTemporal;
				bool m_dicomizado;
		};
	}
}
