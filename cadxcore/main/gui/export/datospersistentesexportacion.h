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

#include <api/dicom/idicom.h>

namespace GNC
{
namespace GCS
{
class IVista;
}
}

namespace GNC
{
namespace GUI
{
typedef struct TipoWizardExportacion {
        typedef enum {EXTERNAL_FOLDER, OVERWRITE, NEW_STUDY} Target;
        Target		  m_target;
        std::string   m_pathDestino;

        std::list<long> ListOfSeriesPks;
        //parametros principales
        std::list<std::string> ListOfDICOMPaths;

        //exportacion ginkgo
        GIL::DICOM::DicomDataset m_base;
        bool m_incluirTagsGinkgo;


        TipoWizardExportacion() :
                m_target(EXTERNAL_FOLDER),
                m_pathDestino(),
                ListOfSeriesPks(),
                m_base(),
                m_incluirTagsGinkgo(true)
        {
        }

        ~TipoWizardExportacion()
        {
        }

} TipoWizardExportacion;
}
}
