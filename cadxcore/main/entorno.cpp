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

#include <string>
#include <sstream>
#include <cstdio>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/event.h>

#include <api/globals.h>
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/icontroladorpermisos.h>
#include <api/observers/iextensionsobserver.h>
#include <api/observers/iviewsobserver.h>
#include <api/internationalization/internationalization.h>

#include <main/license.h>

#include <api/helpers/aetwildcards.h>
#include <api/ubication.h>

#include <main/controllers/pacscontroller.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorcarga.h>
#include <main/controllers/controladoracciones.h>
#include <main/controllers/controladorinternacionalizacion.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/integrationcontroller.h>
#include <main/managers/widgetsmanager.h>

#ifdef UNICODE
#define UNICODE_WAS_SET
#undef UNICODE
#endif 
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmdata/dcrleerg.h>
#ifdef UNICODE_WAS_SET
#define UNICODE
#endif 


#include "entorno.h"

// Definicion de variables static
#define GINKGO_LOCALAET    "GINKGO_%IP4"
#if defined GINKGO_PRO
#include <ginkgopro/ginkgo_pro_defines.h>
#else
#define EULA "Ginkgo CADx is licensed under the LGPL v3 license. You can find the legal terms of the license at http://www.gnu.org/copyleft/lgpl.html"
#endif

//helper
bool RemoveTempDir(const wxString& dirPath)
{
	//borra en cascada
	if(!wxRmdir(dirPath)){
		//vaciar
		wxDir dir;
		if (dir.Open(dirPath)) {
			wxString fileName;
			bool cont = dir.GetFirst(&fileName);
			while (cont) {
				fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +fileName;
				if(wxDir::Exists(fileName)){
					if (RemoveTempDir(fileName)) {
						cont = dir.GetFirst(&fileName);
					} else {
						cont = dir.GetNext(&fileName);
					}
				}else{
                    int result;
					#ifdef _WIN32
                    result = _chmod(TOPATH(fileName).c_str(), _S_IWRITE);
					#else 
                    result = chmod(TOPATH(fileName).c_str(), 0644);
					#endif
                    if (result != 0) {
                        LOG_ERROR("Core", "Unable to change permission of '" << TOPATH(fileName) << "':" << strerror(errno));
                    }

					if (wxRemoveFile(fileName)) {
						cont = dir.GetFirst(&fileName);
					} else {
						cont = dir.GetNext(&fileName);
					}
				}
			}
		}
	}
	if(wxDir::Exists(dirPath)){
		return wxRmdir(dirPath);
	}
	return true;
}

GNC::Entorno* GNC::Entorno::m_pInstancia = NULL;

GNC::Entorno::Entorno()
{
	DJEncoderRegistration::registerCodecs(
		ECC_lossyYCbCr,
		EUC_default, // UID generation (never create new UID's)
		OFFalse, // verbose
		0, 0, 0, true, ESS_444, true); // optimize huffman table
	DJDecoderRegistration::registerCodecs();

	DcmRLEEncoderRegistration::registerCodecs();
	DcmRLEDecoderRegistration::registerCodecs();

	m_pApp = NULL;
	m_pVentanaPrincipal = NULL;
	m_pVentanaRaiz = NULL;
	m_GinkgoLicenseMessage = "";
	
	m_isChildInstance = false;

	std::ostringstream os;
	os << GINKGO_VERSION;
	if (sizeof(void*) == 4) {
		os << " 32 bits";
	}
	else if (sizeof(void*) == 8) {
		os << " 64 bits";
	}
	std::sscanf(GINKGO_VERSION, "%u.%u.%*c", &m_mayorVersionNumber, &m_minorVersionNumber);

	m_GinkgoVersion = os.str();

	m_GinkgoCopyRight = GINKGO_COPYRIGHT;
	
	wxFileName executable_path = wxStandardPaths::Get().GetExecutablePath();
	//executable_path.SetFullName(wxEmptyString);
	
	wxString pluginsDir;
	wxString resourcesDir;
	wxString langDir;

	
	#if defined(_WINDOWS)
		pluginsDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("Plugins");
		langDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("lang");
		resourcesDir = executable_path.GetPath();
	#elif defined(LINUX)
		#if defined(CUSTOM_PACKAGE)
			pluginsDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("Plugins");
			resourcesDir = executable_path.GetPath();
			langDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("lang");
		#else
			pluginsDir = wxStandardPaths::Get().GetPluginsDir() + wxFileName::GetPathSeparator() + wxT("Plugins");
			resourcesDir = wxStandardPaths::Get().GetResourcesDir();
			langDir = wxStandardPaths::Get().GetResourcesDir() + wxFileName::GetPathSeparator() + wxT("lang");
		#endif
		
	#else
		pluginsDir = wxStandardPaths::Get().GetPluginsDir();
		langDir = wxStandardPaths::Get().GetResourcesDir() + wxFileName::GetPathSeparator() + wxT("lang");
		resourcesDir = wxStandardPaths::Get().GetResourcesDir();
	#endif
	
	m_GinkgoPluginsDir = TOPATH(pluginsDir);
	m_GinkgoLanguageDir = TOPATH(langDir);
	m_GinkgoResourcesDir = TOPATH(resourcesDir);
	
	//std::cout << "PLUGINS_DIR = " << m_GinkgoPluginsDir.c_str() << std::endl;
	//std::cout << "LANGUAGE_DIR = " << m_GinkgoLanguageDir.c_str() << std::endl;

	wxString wxTempDir = wxStandardPaths::Get().GetTempDir() + wxFileName::GetPathSeparator() + wxT("GinkgoCADx");
	m_GinkgoTempDir = TOPATH(wxTempDir);
	if (!wxDirExists(wxTempDir)) {
                bool success;
#ifdef _WIN32
                success = wxMkdir(wxTempDir,511);
#else
                success = (wxMkDir(wxTempDir.ToUTF8(), 0770) == 0);
#endif
                if (!success) {
                        LOG_ERROR("Core", "Unable to create temporary directory '" << m_GinkgoTempDir << "'");
                }
	}
	
	m_GinkgoUserDir = TOPATH(wxStandardPaths::Get().GetUserDataDir());
	
	{
		wxString directorioUser = FROMPATH(m_GinkgoUserDir);
		//se crea el user dir
		if(!wxDirExists(directorioUser)) {
                        bool success;
#ifdef _WIN32
                        success = wxMkdir(directorioUser,511);
#else
                        success = (wxMkDir(directorioUser.ToUTF8(), 0770) == 0);
#endif
                        if (!success) {
                                LOG_ERROR("Core", "Unable to create user directory '" << m_GinkgoUserDir << "'");
                        }
		}
	}

	// Carga de ubicaciones
	GNC::GCS::ConfigurationController::TListGroups locations;
	GNC::GCS::ConfigurationController::Instance()->readGroupGeneral("/GinkgoCore/Locations", locations);
	for (GNC::GCS::ConfigurationController::TListGroups::iterator it = locations.begin(); it != locations.end(); ++it) {
		std::string title, path, descr;
		bool monitorize, cleanbefore, cleanafter;
		(*it).readStringValue("Title", title);
		(*it).readStringValue("Path", path);
		(*it).readStringValue("Description", descr);
		(*it).readBoolValue("Monitorize", monitorize, false);
		(*it).readBoolValue("CleanBefore", cleanbefore, false);
		(*it).readBoolValue("CleanAfter", cleanafter, false);

		m_Ubicaciones[title] = new GIL::Ubicacion(title, path, descr, monitorize, cleanbefore, cleanafter);
	}

	GIL::IntegrationController::Instance();
}

GNC::Entorno::~Entorno()
{
	GIL::IntegrationController::FreeInstance();
	DJEncoderRegistration::cleanup();
	DJDecoderRegistration::cleanup();

	DcmRLEEncoderRegistration::cleanup();
	DcmRLEDecoderRegistration::cleanup();

	if (!ExtensionsObservers.empty()) {
		LOG_WARN("Core", "Extensions observers list not empty");
	}
	
	if (!ViewsObservers.empty()) {
		LOG_WARN("Core", "Views observers list not empty");
	}

	//remove temp dir
	if (!m_isChildInstance) {
		RemoveTempDir(FROMPATH(m_GinkgoTempDir));	
	}

	m_pApp = NULL;
	m_pVentanaPrincipal = NULL;
	m_pVentanaRaiz = NULL;

	for (MapaUbicaciones::iterator it = m_Ubicaciones.begin(); it != m_Ubicaciones.end(); ++it) {
		delete (*it).second;
	}
	m_Ubicaciones.clear();
}

GNC::Entorno* GNC::Entorno::Instance()
{
	if (m_pInstancia == NULL) {
		m_pInstancia = new GNC::Entorno();
	}
	return m_pInstancia;
}

void GNC::Entorno::FreeInstance()
{
	if (m_pInstancia != NULL) {
		delete m_pInstancia;
	}
	m_pInstancia = NULL;
}

/** Gets the GinkgoCADx version full string **/
const std::string& GNC::Entorno::GetGinkgoVersionString()
{
	return m_GinkgoVersion;
}

/** Gets the GinkgoCADx version string **/
std::string GNC::Entorno::GetGinkgoVersion()
{
	return std::string(GINKGO_VERSION);
}

/** Gets the mayor version number **/
int GNC::Entorno::GetGinkgoMayorVersionNumber()
{
	return m_mayorVersionNumber;
}

/** Gets the mayor version minor **/
int GNC::Entorno::GetGinkgoMinorVersionNumber()
{
	return m_minorVersionNumber;
}


/** Gets the application name */
std::string GNC::Entorno::GetApplicationName()
{
	return std::string(PRODUCT_NAME);
}

/** Gets the application subtitle */
std::string GNC::Entorno::GetApplicationSubTitle()
{
	return PRODUCT_SUBTITLE;
}

/** Gets the application credits */
std::string GNC::Entorno::GetApplicationCredits()
{
	return std::string(CREDITS);
}

/** Gets the application eula */
std::string GNC::Entorno::GetApplicationEula()
{
	return std::string(EULA);
}

const std::string& GNC::Entorno::GetGinkgoCopyRight()
{
	return m_GinkgoCopyRight;
}

const std::string& GNC::Entorno::GetLicenseMessage()
{
	return m_GinkgoLicenseMessage;
}

void GNC::Entorno::SetLicenseMessage(const std::string& license)
{
	m_GinkgoLicenseMessage = license;
}

int GNC::Entorno::ParseSVNBuild(const char* str)
{
	int num = -1;
	std::string beg;
	std::string end;
	std::stringstream ss(str);
	ss >> beg >> num >> end;
	return num;
}



const std::string& GNC::Entorno::GetGinkgoTempDir()
{
	return m_GinkgoTempDir;
}

std::string GNC::Entorno::CreateGinkgoTempDir()
{
	wxString wxNuevoDirTemp;
	do {
		wxNuevoDirTemp = FROMPATH(GetGinkgoTempDir()) + wxFileName::GetPathSeparator(wxPATH_NATIVE) + wxT("_gnktmp_") + wxString::Format(wxT("%d"), (int) rand());
	} while(wxDir::Exists(wxNuevoDirTemp));

	#ifdef _WIN32
			wxFileName::Mkdir(wxNuevoDirTemp);
	#else
			wxFileName::Mkdir(wxNuevoDirTemp.c_str(), 0777);
	#endif

	return std::string(TOPATH(wxNuevoDirTemp));
}

void GNC::Entorno::RemoveDirRecursive(const std::string& path)
{
	wxString pathWx = FROMPATH(path);
	if (wxDirExists(pathWx)) {
		RemoveTempDir(pathWx);
	}
}

std::string GNC::Entorno::CreateGinkgoTempFile()
{
	std::string directory = CreateGinkgoTempDir();
	std::ostringstream ostr;
	ostr << directory << (char)wxFileName::GetPathSeparator() << "tmp";
	return ostr.str();
}

const std::string& GNC::Entorno::GetGinkgoLanguageDir()
{
	return m_GinkgoLanguageDir;
}

const std::string& GNC::Entorno::GetGinkgoResourcesDir()
{
	return m_GinkgoResourcesDir;
}

std::string GNC::Entorno::CrearDirectorioTemporal()
{
	wxString dirTmp;
	do {
		std::ostringstream ostr;
		ostr << GetGinkgoTempDir().c_str() << (char)wxFileName::GetPathSeparator() << "_gnktmp_" << rand();
		dirTmp = FROMPATH(ostr.str());
	} while (wxDirExists(dirTmp));

	#if defined(_WINDOWS)
    bool success = wxMkDir(dirTmp);
	#else
    bool success = wxMkDir(dirTmp.ToUTF8(), 0770);
	#endif

    std::string resultado(TOPATH(dirTmp));
    if (!success) {
        LOG_ERROR("Core", "Error creating temporary directory '"<<resultado << "'");
    }
	return resultado;
}

const std::string& GNC::Entorno::GetPluginsPath()
{
	return m_GinkgoPluginsDir;
}

const std::string& GNC::Entorno::GetGinkgoUserDir()
{
	return m_GinkgoUserDir;
}

std::string GNC::Entorno::GetDicomLocalAET()
{
	std::string value;
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/PACS/Local", "AET", value, GINKGO_LOCALAET);

	return ParseAET(value);
}

std::string GNC::Entorno::ParseAET(const std::string& AET)
{
	return GNC::GCS::AETWildcards::Parse(AET);
}

wxApp* GNC::Entorno::GetApp()
{
	return m_pApp;
}

void GNC::Entorno::YieldApp()
{
	//DEPRECATED
	//m_pApp->Yield();
}


void GNC::Entorno::SetApp(wxApp* app)
{
	m_pApp = app;
}

VentanaPrincipal* GNC::Entorno::GetVentanaPrincipal()
{
	return m_pVentanaPrincipal;
}

void GNC::Entorno::SetVentanaPrincipal(VentanaPrincipal* ventana)
{
	m_pVentanaPrincipal = ventana;
}

wxWindow* GNC::Entorno::GetVentanaRaiz()
{
	return m_pVentanaRaiz;
}

void GNC::Entorno::SetVentanaRaiz(wxWindow* ventana)
{
	m_pVentanaRaiz = ventana;
}

GNC::GCS::ICommandController* GNC::Entorno::GetCommandController()
{
	return GNC::GCS::ICommandController::Instance();
}

GIL::DICOM::IPACSController* GNC::Entorno::GetPACSController()
{
	return GIL::DICOM::PACSController::Instance();
}

GNC::GCS::IControladorVistas* GNC::Entorno::GetControladorVistas()
{
	return GNC::GCS::ControladorVistas::Instance();
}

GNC::GCS::IEventsController* GNC::Entorno::GetControladorEventos()
{
	return GNC::GCS::ControladorEventos::Instance();
}

GNC::GCS::IControladorCarga* GNC::Entorno::GetControladorCarga()
{
	return GNC::GCS::ControladorCarga::Instance();
}

GNC::GCS::IControladorPermisos* GNC::Entorno::GetControladorPermisos()
{
	return GNC::GCS::IControladorPermisos::Instance();
}

GNC::GCS::IControladorExtensiones* GNC::Entorno::GetControladorExtensiones()
{
	return GNC::ControladorExtensiones::Instance();
}

GNC::GCS::IControladorAcciones* GNC::Entorno::GetControladorAcciones()
{
	return GNC::GCS::ControladorAcciones::Instance();
}

GNC::GCS::IControladorInternacionalizacion* GNC::Entorno::GetControladorInternacionalizacion()
{
	return GNC::GCS::ControladorInternacionalizacion::Instance();
}

GNC::GCS::IConfigurationController* GNC::Entorno::GetConfigurationController()
{
	return GNC::GCS::ConfigurationController::Instance();
}

GNC::GCS::IWidgetsManager* GNC::Entorno::NewWidgetsManager(GNC::GCS::IVista* pVista)
{
	return new GNC::WidgetsManager(pVista);
}

void GNC::Entorno::FreeWidgetsManager(GNC::GCS::IWidgetsManager* pManager)
{
	if (pManager != NULL) {
		delete pManager;
	}
}


//------------------------------------------------------------------------------
//region "Extensions observers registry interface"

void GNC::Entorno::RegisterExtensionsObserver(GNC::GCS::IExtensionsObserver* observer)
{
	GNC::GCS::ILocker lock(ExtensionsObserversLock);

	for (ExtensionsObserversList::const_iterator it = ExtensionsObservers.begin(); it != ExtensionsObservers.end(); ++it) {
		if (*it == observer) {
			LOG_WARN("Core", "Extensions observer already registered");
			return;
		}
	}
	ExtensionsObservers.push_back(observer);
}

void GNC::Entorno::UnRegisterExtensionsObserver(GNC::GCS::IExtensionsObserver* observer)
{
	GNC::GCS::ILocker lock(ExtensionsObserversLock);
	ExtensionsObservers.remove(observer);
}

//endregion

//------------------------------------------------------------------------------
//region "Views observers registry interface"
void GNC::Entorno::RegisterViewsObserver(GNC::GCS::IObservadorVistas* /*observer*/)
{
	GNC::GCS::ILocker lock(ViewsObserversLock);
	//TODO
}

void GNC::Entorno::UnRegisterViewsObserver(GNC::GCS::IObservadorVistas* /*observer*/)
{
	GNC::GCS::ILocker lock(ViewsObserversLock);
	//TODO
}
//endregion
