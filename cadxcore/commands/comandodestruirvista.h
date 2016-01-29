/*
*  
*  $Id: comandodestruirvista.h $
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
#include <api/api.h>
#include <api/icommand.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/autoptr.h>

namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GADAPI {
	class EXTAPI ComandoDestruirVistaParams : public GNC::GCS::IComandoParams {
	public:
		ComandoDestruirVistaParams(GNC::GCS::IVista* pVista);

		virtual ~ComandoDestruirVistaParams();

		GNC::GCS::IVista*   m_pVista;
	};

	class EXTAPI ComandoDestruirVista : public GNC::GCS::IComando {
	public:

		ComandoDestruirVista(ComandoDestruirVistaParams* pParams);

		virtual void Execute();

		virtual void Update();

	protected:

		ComandoDestruirVistaParams* m_pDestruirParams;

	};
}
