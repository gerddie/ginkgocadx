#pragma once
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

#ifndef IENVIRONMENT_H
#define IENVIRONMENT_H

#include <string>
#include <map>

#include <api/api.h>
#include <api/autoptr.h>

// Forward Declarations
class wxPanel;
class wxWindow;
class wxPropertyGrid;
class wxApp;
class VentanaPrincipal;

// Forward Declarations
namespace GNC {
	namespace GCS {
		class IControladorVistas;
		class ICommandController;
		class IEventsController;
		class IControladorCarga;
		class IConfigurationController;
		class IControladorPermisos;
		class IControladorLog;
		class IControladorExtensiones;
		class IControladorAcciones;
		class IControladorInternacionalizacion;
		class IWidgetsManager;
		class IVista;

		class IExtensionsObserver;
		class IObservadorVistas;
	}
	
}
namespace GIL {
	namespace DICOM {
		class IPACSController;
	}
	class Ubicacion;
}

namespace GNC {

	namespace GCS {

		class EXTAPI IEntorno {

		public:			
			typedef std::map< std::string, GIL::Ubicacion* > MapaUbicaciones;
			static IEntorno* m_pInstance;

		protected:
			IEntorno();
			virtual ~IEntorno();

			MapaUbicaciones         m_Ubicaciones;

		public:

			static IEntorno* Instance();
			static void FreeInstance();

			//region version

			/** Gets the GinkgoCADx version full string **/
			virtual const std::string& GetGinkgoVersionString() = 0;

			/** Gets the GinkgoCADx version string **/
			virtual std::string GetGinkgoVersion() = 0;

			/** Gets the revision mark (incremental) **/
			virtual int GetGinkgoRevision() = 0;

			/** Gets the build mark (incremental) **/
			virtual std::string GetGinkgoBuild() = 0;

			/** Gets the mayor version number **/
			virtual int GetGinkgoMayorVersionNumber() = 0;
			
			/** Gets the mayor version minor **/
			virtual int GetGinkgoMinorVersionNumber() = 0;

			/** Gets the application name */
			virtual std::string GetApplicationName() = 0;

			/** Gets the application subtitle */
			virtual std::string GetApplicationSubTitle() = 0;
			
			/** Gets the application credits */
			virtual std::string GetApplicationCredits() = 0;
			
			/** Gets the application eula */
			virtual std::string GetApplicationEula() = 0;
			
			//endregion

			//region license
			virtual const std::string& GetLicenseMessage() = 0;
			//enregion

			virtual const std::string& GetGinkgoCopyRight() = 0;
			virtual int ParseSVNBuild(const char* str) = 0;
			virtual const std::string& GetPluginsPath() = 0;
			virtual const std::string& GetGinkgoUserDir() = 0;
			virtual const std::string& GetGinkgoTempDir() = 0;
			virtual std::string CreateGinkgoTempDir() = 0;
			virtual const std::string& GetGinkgoResourcesDir() = 0;
			virtual std::string CrearDirectorioTemporal() = 0;
			virtual std::string GetDicomLocalAET() = 0;
			virtual std::string ParseAET(const std::string& aet) = 0;
			virtual void SetLicenseMessage(const std::string& desc) = 0;

			virtual void YieldApp() = 0;
			virtual wxApp* GetApp() = 0;
			virtual VentanaPrincipal* GetVentanaPrincipal() = 0;
			virtual wxWindow* GetVentanaRaiz() = 0;

			virtual GNC::GCS::IControladorVistas* GetControladorVistas() = 0;
			virtual GNC::GCS::ICommandController* GetCommandController() = 0;
			
			virtual GNC::GCS::IEventsController* GetControladorEventos() = 0;
			virtual GNC::GCS::IControladorCarga* GetControladorCarga() = 0;
			virtual GNC::GCS::IControladorPermisos* GetControladorPermisos() = 0;
			virtual GNC::GCS::IControladorExtensiones* GetControladorExtensiones() = 0;
			virtual GNC::GCS::IControladorAcciones*  GetControladorAcciones() = 0;
			virtual GNC::GCS::IControladorInternacionalizacion*  GetControladorInternacionalizacion() = 0;
			virtual GNC::GCS::IConfigurationController* GetConfigurationController() = 0;
			virtual GNC::GCS::IWidgetsManager* NewWidgetsManager(GNC::GCS::IVista* pVista) = 0;

			virtual GIL::DICOM::IPACSController* GetPACSController() = 0;

			virtual void FreeWidgetsManager(GNC::GCS::IWidgetsManager* pManager) = 0;

			virtual MapaUbicaciones& GetUbicaciones() = 0;
			virtual void RemoveDirRecursive(const std::string& path) = 0;
			virtual std::string CreateGinkgoTempFile() = 0;

//------------------------------------------------------------------------------
//region "Extensions observers registry interface"

			virtual void RegisterExtensionsObserver(GNC::GCS::IExtensionsObserver* observer) = 0;
			virtual void UnRegisterExtensionsObserver(GNC::GCS::IExtensionsObserver* observer) = 0;
//endregion

//------------------------------------------------------------------------------
//region "Views observers registry interface"

			virtual void RegisterViewsObserver(GNC::GCS::IObservadorVistas* observer) = 0;
			virtual void UnRegisterViewsObserver(GNC::GCS::IObservadorVistas* observer) = 0;
//endregion

		};
	}
}
#endif
