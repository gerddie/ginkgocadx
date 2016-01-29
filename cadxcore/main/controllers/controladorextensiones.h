/*
 *  
 *  $Id: controladorextensiones.h $
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
