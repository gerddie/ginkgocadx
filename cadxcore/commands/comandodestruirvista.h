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
