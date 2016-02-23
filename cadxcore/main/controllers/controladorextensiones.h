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
#include <api/controllers/icontroladorextensiones.h>
#include <api/controllers/imodulecontroller.h>
#include <map>
#include <string>

namespace GNC {

	class EXTAPI ControladorExtensiones : public GNC::GCS::IControladorExtensiones {
	public:

		static ControladorExtensiones* Instance();
		static void FreeInstance();

		void RegistrarModulo(GNC::GCS::IModuleController* pCtrlModulo);
		void DesRegistrarModulo(GNC::GCS::IModuleController* pCtrlModulo);
		void DesRegistrarModulos();

		GNC::GCS::IModuleController* ObtenerModulo(const std::string& idModulo);

		typedef std::map<std::string, GNC::GCS::IModuleController*> ListaModulos;
		typedef ListaModulos::iterator IteradorListaModulos;
		typedef ListaModulos::const_iterator IteradorConstanteListaModulos;

		std::list<GNC::GCS::IModuleController*> ModulosOrdenados();
		const ControladorExtensiones::ListaModulos& Modulos() const;

		// Realización de la interfaz IControladorExtensiones
		virtual GADVAPI::PrivateExtensionManager& GetPrivateExtensionsManager();

	protected:
		ControladorExtensiones();
		~ControladorExtensiones();

		void NotificarRegistro(GNC::GCS::IModuleController* pModulo);
		void NotificarDesRegistro(GNC::GCS::IModuleController* pModulo);

	protected:
		static ControladorExtensiones* m_pInstance;
		ListaModulos m_Modulos;

		GADVAPI::PrivateExtensionManager m_Manager;

	};
}
