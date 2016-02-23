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
