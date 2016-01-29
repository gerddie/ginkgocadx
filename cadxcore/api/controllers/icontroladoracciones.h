/*
 *  
 *  $Id: icontroladoracciones.h $
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

