/*
*  
*  $Id: exporttodicomdircommand.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
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
