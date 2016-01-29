/*
 *  
 *  $Id: datospersistentesexportacion.h $
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

#include <api/dicom/idicom.h>

namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GNC {
	namespace GUI {
		typedef struct TipoWizardExportacion{
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

			~TipoWizardExportacion(){
			}

		} TipoWizardExportacion;
	}
}
