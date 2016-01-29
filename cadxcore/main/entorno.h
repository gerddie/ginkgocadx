/*
 *  
 *  $Id: entorno.h $
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
#include <list>
#include <string>

#include <api/internal.h>
#include <api/globals.h>
#include <api/ientorno.h>
#include <api/ilock.h>

class VentanaPrincipal;
class wxWindow;
class wxApp;
class wxSizer;

namespace GNC {
	namespace GCS {
		class IExtensionsObserver;
		class IObservadorVistas;
		class IEventsController;
	}
}

namespace GNC {
	class Entorno : public GNC::GCS::IEntorno {
	public:

		typedef std::list<GNC::GCS::IExtensionsObserver*> ExtensionsObserversList;
		typedef std::list<GNC::GCS::IObservadorVistas*>   ViewsObserversList;
		
		bool m_isChildInstance;

	protected:

		/* Constructor */
		Entorno();
		/* Destructor */
		~Entorno();

		static Entorno* m_pInstancia;

	public:
		static Entorno* Instance();
		static void FreeInstance();
		
		
		bool IsChildInstance() {
			return m_isChildInstance;
		}
		
		void SetChildInstance(bool child) {
			m_isChildInstance = child;
		}

		// Miembros estaticos

		//region version

		/** Gets the GinkgoCADx version full string **/
		virtual const std::string& GetGinkgoVersionString();

		/** Gets the GinkgoCADx version string **/
		virtual std::string GetGinkgoVersion();

		/** Gets the revision mark (incremental) **/
		virtual int GetGinkgoRevision();

		/** Gets the build mark (incremental) **/
		virtual std::string GetGinkgoBuild();

		/** Gets the mayor version number **/
		virtual int GetGinkgoMayorVersionNumber();
		
		/** Gets the mayor version minor **/
		virtual int GetGinkgoMinorVersionNumber();

		/** Gets the application name */
		virtual std::string GetApplicationName();

		/** Gets the application subtitle */
		virtual std::string GetApplicationSubTitle();
		
		/** Gets the application credits */
		virtual std::string GetApplicationCredits();

		/** Gets the application eula */
		virtual std::string GetApplicationEula();
		//endregion
		
		//region license
		virtual const std::string& GetLicenseMessage();
		virtual void SetLicenseMessage(const std::string& license);
		
		virtual const std::string& GetGinkgoCopyRight();
		//enregion

		//region helpers
		virtual int ParseSVNBuild(const char* str);
		//endregion

		virtual const std::string& GetPluginsPath();
		virtual const std::string& GetGinkgoUserDir();
		virtual const std::string& GetGinkgoTempDir();
		virtual std::string CreateGinkgoTempDir();
		virtual void RemoveDirRecursive(const std::string& path);
		virtual std::string CreateGinkgoTempFile();
		virtual const std::string& GetGinkgoLanguageDir();
		virtual const std::string& GetGinkgoResourcesDir();
		virtual std::string CrearDirectorioTemporal();
		virtual std::string  GetDicomLocalAET();

		std::string ParseAET(const std::string& AET);

		virtual wxApp* GetApp();
		virtual void SetApp(wxApp* app);

		virtual void YieldApp();

		virtual VentanaPrincipal* GetVentanaPrincipal();
		virtual void SetVentanaPrincipal(VentanaPrincipal* ventana);

		virtual wxWindow* GetVentanaRaiz();
		virtual void SetVentanaRaiz(wxWindow* window);

		virtual GNC::GCS::ICommandController* GetCommandController();

		virtual GIL::DICOM::IPACSController* GetPACSController();

		virtual GNC::GCS::IControladorVistas* GetControladorVistas();

		virtual GNC::GCS::IEventsController* GetControladorEventos();

		virtual GNC::GCS::IControladorCarga* GetControladorCarga();

		virtual GNC::GCS::IControladorPermisos* GetControladorPermisos();

		virtual GNC::GCS::IControladorExtensiones* GetControladorExtensiones();

		virtual GNC::GCS::IControladorAcciones*  GetControladorAcciones();

		virtual GNC::GCS::IControladorInternacionalizacion*  GetControladorInternacionalizacion();

		virtual GNC::GCS::IConfigurationController* GetConfigurationController();

		virtual GNC::GCS::IWidgetsManager* NewWidgetsManager(GNC::GCS::IVista* pVista);
		virtual void FreeWidgetsManager(GNC::GCS::IWidgetsManager* pManager);

		virtual GNC::GCS::IEntorno::MapaUbicaciones& GetUbicaciones() { return m_Ubicaciones; }


//------------------------------------------------------------------------------
//region "Extensions observers registry interface"

	public:
		virtual void RegisterExtensionsObserver(GNC::GCS::IExtensionsObserver* observer);
		virtual void UnRegisterExtensionsObserver(GNC::GCS::IExtensionsObserver* observer);
	
	public: // TODO
		ExtensionsObserversList ExtensionsObservers;
		GNC::GCS::ILockable ExtensionsObserversLock;
//endregion

//------------------------------------------------------------------------------
//region "Views observers registry interface"

	public:
		virtual void RegisterViewsObserver(GNC::GCS::IObservadorVistas* observer);
		virtual void UnRegisterViewsObserver(GNC::GCS::IObservadorVistas* observer);
		
	public: // TODO
		ViewsObserversList ViewsObservers;
		GNC::GCS::ILockable ViewsObserversLock;
//endregion

	private:
		std::string m_GinkgoVersion;
		std::string m_GinkgoCopyRight;
		std::string m_GinkgoPluginsDir;
		std::string m_GinkgoUserDir;
		std::string m_GinkgoTempDir;
		std::string m_GinkgoLanguageDir;
		std::string m_GinkgoResourcesDir;
		std::string m_GinkgoLicenseMessage;

		int m_mayorVersionNumber;
		int m_minorVersionNumber;

		wxApp* m_pApp;
		VentanaPrincipal* m_pVentanaPrincipal;
		wxWindow* m_pVentanaRaiz;

		friend class ::VentanaPrincipal;
	};
}
