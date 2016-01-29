/*
*  
*  $Id: comandopacs.h $
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
//#define _GINKGO_TRACE
#include <api/globals.h>
#include <api/icommand.h>
#include <api/ivista.h>
#include <api/autoptr.h>
#include <api/dicom/imodelodicom.h>
#include <api/controllers/ipacscontroller.h>

#include <main/controllers/dcmtk/dicomservers.h>

namespace GNC {
	namespace GCS {
		class StoredQuery;
	}
}

namespace GIL{
	class IModeloIntegracion;
	namespace DICOM{
		class IPACSController;
	}
}

namespace GADAPI {
	class ComandoPACSParams : public GNC::GCS::IComandoParams {
	public:
		typedef std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> > TSearchResultList;
		ComandoPACSParams(const GNC::GCS::Ptr<GNC::GCS::StoredQuery>& query) ;

		virtual ~ComandoPACSParams();

	public:
		GNC::GCS::Ptr<GNC::GCS::StoredQuery> pQuery;
		GNC::GCS::Ptr<TSearchResultList> m_apResults;

		bool                     m_informar;
		std::string              m_error;
	};



	class EXTAPI ComandoPACS : public GNC::GCS::IComando {
	public:
		ComandoPACS(ComandoPACSParams* pParams);

    protected:
		virtual void Execute();
		virtual void Update();

    public:
		virtual bool NotificarProgreso(float progresoNormalizado, const std::string &texto);

		void LiberarRecursos();

		ComandoPACSParams* m_pPACSParams;
	};
}
