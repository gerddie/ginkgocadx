/*
*  
*  $Id: comandocarga.h $
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
#include <api/icommand.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/autoptr.h>

namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GADAPI {
	class ComandoCarga;

	class ComandoCargaParams : public GNC::GCS::IComandoParams, public GNC::GCS::IObservadorVistas {
	public:
		ComandoCargaParams(GNC::GCS::IVista* pVista, const bool loadFirst);

		virtual ~ComandoCargaParams();

		virtual void OnVistaDestruida(GNC::GCS::IVista* pVista);

		GNC::GCS::IVista*               IVista;
		ComandoCarga*                   Padre;
		bool									  LoadFirst;

	};

	class ComandoCarga : public GNC::GCS::IComando {
	public:

		ComandoCarga(ComandoCargaParams* pParams);

    protected:

		virtual void Execute();
		virtual void Update();

	protected:

		ComandoCargaParams* CargaParams;
		std::string         Error;

	};
}
