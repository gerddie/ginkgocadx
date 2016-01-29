/*
 *  
 *  $Id: controladorinternacionalizacion.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <api/api.h>
#include <api/controllers/icontroladorinternacionalizacion.h>

namespace GNC {
	namespace GCS {
		class EXTAPI ControladorInternacionalizacion: public IControladorInternacionalizacion
		{
		public:
			static ControladorInternacionalizacion* Instance();
			static void FreeInstance();
			
			//interfaz del icontrolador (la que ven los modulos)
			virtual void AddCatalog(const std::string& catalogName);
			int GetIdiomaInterfaz();

		protected:
			static ControladorInternacionalizacion* m_pInstance;
			ControladorInternacionalizacion();
			~ControladorInternacionalizacion();
			int m_idiomaInterfazSesion;
		};
	}
}
