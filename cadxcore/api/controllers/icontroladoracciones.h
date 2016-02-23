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
#include <api/autoptr.h>


namespace GNC {
	namespace GCS {
		class IVista;
	}
}

namespace GNC {
	namespace GCS {
		class EXTAPI Accion {
		public:
			Accion(const std::string& nombre, bool puedeRehacerse = true, const int peso = 1);
			virtual ~Accion();

			virtual const std::string& GetNombre();
			virtual void Deshacer() = 0;
			virtual bool PuedeRehacer();
			virtual void Hacer();

		protected:
			virtual void SetPuedeRehacerse(bool puedeRehacerse = true);

			std::string m_nombre;
			int m_peso;
			bool m_puedeRehacerse;
		};

		class EXTAPI IControladorAcciones
		{
		protected:
			IControladorAcciones();
			~IControladorAcciones();
		public:
			static IControladorAcciones* Instance();
			static void FreeInstance();
			
			virtual void PushAccion(GNC::GCS::IVista* pVista, GNC::GCS::Ptr<Accion> pAccion) = 0;
		protected:
		};
	}
}

