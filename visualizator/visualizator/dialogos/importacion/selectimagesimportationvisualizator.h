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
#include <api/iwizard.h>
#include <string>
#include <vector>
#include <main/gui/import/importationdata.h>
#include <api/dicom/idicomizador.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <main/gui/import/selectimagesimportation.h>

namespace GNC
{
namespace GCS
{
class IEntorno;
}
}

namespace GNKVisualizator
{
namespace GUI
{

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
