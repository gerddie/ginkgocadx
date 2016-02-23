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
#include <list>

#include <api/autoptr.h>
#include <api/icommand.h>
#include <api/dicom/dicomdataset.h>


namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GIL {
	namespace DICOM {
		class DicomDataset;
	}
}

namespace GADAPI {
	class ExportToDicomDirCommand;

	class EXTAPI ExportToDicomDirCommandParams : public GNC::GCS::IComandoParams {
	public:
		typedef std::list<long> TListOfPks;
		ExportToDicomDirCommandParams(const TListOfPks& seriesList, const std::string& destinationPath, const GIL::DICOM::DicomDataset& anonymized, bool includeGinkgo);

		virtual ~ExportToDicomDirCommandParams();

		TListOfPks m_seriesList;
		std::string m_destinationPath;
		GIL::DICOM::DicomDataset m_anonymized;
		bool m_includeGinkgoTags;

		std::string m_Error;
		bool m_hasError;
	};

	class EXTAPI ExportToDicomDirCommand : public GNC::GCS::IComando {
	public:

		ExportToDicomDirCommand(ExportToDicomDirCommandParams* pParams);

    protected:

		virtual void Execute();
		virtual void Update();

	protected:

		ExportToDicomDirCommandParams* m_pExportacionParams;
	};
}
