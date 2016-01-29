/*
*  
*  $Id: comandoincluirhistorial.h $
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
#include <set>

#include <api/api.h>
#include <api/icommand.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/controllers/ihistorycontroller.h>
#include <api/dicom/imodelodicom.h>
#include <wx/string.h>

namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GADAPI {
	//lee un directorio en profundidad y lo incluye en el dicomdir
	namespace LinkHistoryCommand
		{
		class LinkHistoryCommand;
		class EXTAPI LinkHistoryCommandParams : public GNC::GCS::IComandoParams {
		public:
			LinkHistoryCommandParams(const GNC::GCS::Ptr<IModeloDicom>& pModelo);
			virtual ~LinkHistoryCommandParams();

			GNC::GCS::Ptr<IModeloDicom> m_pModelo;
			GNC::GCS::IHistoryController::TAddErrorList m_ErrorList;
			std::list<long>                             m_InsertedSeries;
			bool m_abrirDespuesDeCargar;
			bool m_informar;
		};

		class EXTAPI LinkHistoryCommand : public GNC::GCS::IComando {
		public:

			LinkHistoryCommand(LinkHistoryCommandParams* pParams);

        protected:
			virtual void Execute();
			virtual void Update();

        private:
			LinkHistoryCommandParams* m_pLinkParams;
		};
	}
}
