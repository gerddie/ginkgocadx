/*
*  
*  $Id: controladorextensiones.cpp 4943 2012-06-20 12:51:12Z carlos $
*  Ginkgo CADx Project
*
*  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*
*/

#include <string>
#include <map>
#include <cctype>
#include <algorithm>
#include <exception>
#include <set>

#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/dynlib.h>

#include <api/globals.h>
#include <prvext/prvext.h>
#include <api/internationalization/internationalization.h>
#include <api/controllers/imodulecontroller.h>
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/configurationcontroller.h>

#include <main/entorno.h>

#if defined(__WXOSX__)
#include <dlfcn.h>
#elif defined(__WXGTK__)
#include <dlfcn.h>
#elif defined(_WINDOWS)
#else
#error Plataforma no soportada
#endif

#define EXT wxT("*.inf")

#if defined(MONOLITIC)
PrivateExtension* init_GNKProExtension();
PrivateExtension* init_VisualizatorExtension();

PrivateExtension* init_OfthalmologicExtension();
PrivateExtension* init_RadiologicalExtension();
PrivateExtension* init_DermatologyExtension();
#endif


namespace GADVAPI {
	class PrivateExtensionWrapper : public IPrivateExtensionWrapper {

	public:

		PrivateExtensionWrapper() {
			this->privateExtension = NULL;
			
			this->loaded = false;
			this->dll = NULL;
		}

		virtual ~PrivateExtensionWrapper() {
			UnLoad();
		}

		bool operator==(const PrivateExtension* other) const {
			return other != NULL && (*this) == (*other);
		}

		bool operator==(const PrivateExtension& other) const {
			return 
				this->sid == other.GetSID();
		}

		#if defined(MONOLITIC)

		virtual bool Load(PrivateExtension* pExt) 
		{
			if (pExt != NULL) {
				this->privateExtension = pExt;
				this->sid = pExt->GetSID();
				this->provider = pExt->GetProvider();
				this->description = pExt->GetDescription();
				this->updateurl = pExt->GetUpdateURL();
				this->loaded = true;				
			}
			return this->loaded;
		}

		#else

		virtual bool Load() {

			if (this->loaded || this->dll != NULL) {
				return this->loaded;
			}

			this->loaded = false;

			// Load library file
			this->dll = new wxDynamicLibrary();
			LOG_DEBUG("Extensions", _Std("Loading").c_str() << " " << this->sid << " [" << filePath << "]")
			if(!dll->Load(FROMPATH(filePath)))
			{
				LOG_ERROR("Extensions", _Std("Could not load extension library file") << "[" << filePath << "]");
				this->error = _Std("Could not load library file. Check log");
				//std::cout << "Lib not loaded!" << std::endl;
				delete this->dll;
				this->dll = NULL;
			}
			else {
#if defined(_DEBUG)
				//Create a valid function pointer using the function pointer type in plugin.h
				wxDYNLIB_FUNCTION(CreatePrivateExtension_function,CreatePrivateExtensionDebug,*dll);
				//check if the function is found
				if(pfnCreatePrivateExtensionDebug)
				{
					privateExtension = pfnCreatePrivateExtensionDebug();
#else
				//Create a valid function pointer using the function pointer type in header
				wxDYNLIB_FUNCTION(CreatePrivateExtension_function,CreatePrivateExtension,*dll);
				//check if the function is found
				if(pfnCreatePrivateExtension)
				{
					privateExtension = pfnCreatePrivateExtension();
#endif

					// We calls
					if (*this == privateExtension) {
						this->loaded = true;
					}
					else {
						if (this->privateExtension != NULL) {
							delete this->privateExtension;
							this->privateExtension = NULL;
						}
						if (this->dll != NULL) {
							delete this->dll;
							this->dll = NULL;
						}
						this->error = _Std("Extension descriptor does not match");
					}
				}
				else {
					this->error = _Std("Could not find entry point");
					delete this->dll;
					this->dll = NULL;
				}
			}

			return this->loaded;
		}

		#endif



		virtual bool UnLoad() {
			
			if (this->loaded) {
				this->loaded = false;
				if (this->privateExtension != NULL) {
					delete this->privateExtension;
				}
				if (this->dll != NULL) {
					delete this->dll;
					this->dll = NULL;
				}
			}
			return !this->loaded;
		}

		virtual bool IsLoaded() const {
			return this->loaded;
		}

		void setSid(const std::string& sidStr) {
			this->sid = sidStr;
		}

		const std::string& getSid() const {
			return this->sid;
		}

		void setFilePath(const std::string& path) {
			this->filePath = path;
		}

		virtual const std::string& GetProvider() const {
			return this->provider;
		}

		void setProvider(const std::string& providerStr) {
			this->provider = providerStr;
		}

		virtual const std::string& GetDescription() const {
			return this->description;
		}

		void setDescription(const std::string& descriptionStr) {
			this->description = descriptionStr;
		}

		virtual const std::string& GetUpdateURL() const {
			return this->updateurl;
		}

		void setUpdateURL(const std::string& updateURLStr) {
			this->updateurl = updateURLStr;
		}

		virtual const std::string& GetError() const {
			return this->error;
		}

		virtual PrivateExtension* GetExtension() {
			return this->privateExtension;
		}

		int parseInt(const std::string& str) {
			int val = -1;
			std::istringstream is(str);
			is >> val;
			return val;
		}

	private:
		bool loaded;

		std::string sid;

		std::string filePath;
		std::string provider;
		std::string description;

		std::string updateurl;
		std::string error;
		PrivateExtension* privateExtension;
		wxDynamicLibrary* dll;
	};
}



GNC::GCS::IExtensionException::IExtensionException(const std::string& msg, const std::string& component, bool fatal) throw() : GNC::GCS::IException(msg, component, fatal) {}
GNC::GCS::IExtensionException::~IExtensionException() throw() {}


GNC::GCS::IControladorExtensiones::IControladorExtensiones() {}
GNC::GCS::IControladorExtensiones::~IControladorExtensiones() {}


GNC::ControladorExtensiones* GNC::ControladorExtensiones::m_pInstance = 0;

GNC::ControladorExtensiones::ControladorExtensiones()
{
	m_Manager.Scan();
	for (GADVAPI::PrivateExtensionManager::iterator it= m_Manager.begin(); it != m_Manager.end(); ++it)
	{
		GADVAPI::IPrivateExtensionWrapper* iew = (*it).second;
		if (iew->IsLoaded()) {
			PrivateExtension::TListControllers list = iew->GetExtension()->InitializeLibrary(GNC::GCS::IEntorno::Instance());
			for (PrivateExtension::TListControllers::iterator it = list.begin(); it != list.end(); ++it) {
				if ((*it) != NULL) {
					RegistrarModulo((*it));
				}
			}
		}
	}
}

GNC::ControladorExtensiones::~ControladorExtensiones()
{
	DesRegistrarModulos();
	m_Manager.UnLoadAll();
}

GNC::ControladorExtensiones* GNC::ControladorExtensiones::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new GNC::ControladorExtensiones();
	}
	return m_pInstance;
}

void GNC::ControladorExtensiones::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = 0;
	}
	
}

void GNC::ControladorExtensiones::RegistrarModulo(GNC::GCS::IModuleController* pCtrlModulo)
{

	if (pCtrlModulo == NULL) {
		return;
	}

	pCtrlModulo->RegistrarConfiguracion();

	const std::string uid = pCtrlModulo->GetUID();
	m_Modulos[uid] = pCtrlModulo;
	NotificarRegistro(pCtrlModulo);
}

void GNC::ControladorExtensiones::DesRegistrarModulo(GNC::GCS::IModuleController* pCtrlModulo)
{
	if (pCtrlModulo == NULL) {
		return;
	}

	NotificarDesRegistro(pCtrlModulo);

	m_Modulos.erase(pCtrlModulo->GetUID());
	delete pCtrlModulo;
}

void GNC::ControladorExtensiones::DesRegistrarModulos()
{
	GNC::ControladorExtensiones::IteradorListaModulos it;

	for (it = m_Modulos.begin(); it != m_Modulos.end(); ++it) {
		GNC::GCS::IModuleController* item = (*it).second;
		NotificarDesRegistro(item);
		delete item;
	}
	m_Modulos.clear();
}

GNC::GCS::IModuleController* GNC::ControladorExtensiones::ObtenerModulo(const std::string& idModulo)
{
	GNC::GCS::IModuleController* cm = NULL;
	IteradorListaModulos it = m_Modulos.find(idModulo);
	if (it != m_Modulos.end()) {
		cm = (*it).second;
	}
	return cm;
}



bool compareListaModulos(GNC::GCS::IModuleController* s1, GNC::GCS::IModuleController* s2)
{
	return s1->GetPriority()< s2->GetPriority();
}

std::list<GNC::GCS::IModuleController*> GNC::ControladorExtensiones::ModulosOrdenados()
{
	std::list<GNC::GCS::IModuleController*> listaModulosPriorizados;
	for(ListaModulos::iterator it = m_Modulos.begin(); it!= m_Modulos.end(); ++it)
	{
		listaModulosPriorizados.push_back((*it).second);
	}

	listaModulosPriorizados.sort(compareListaModulos);	

	return listaModulosPriorizados;
}

const GNC::ControladorExtensiones::ListaModulos& GNC::ControladorExtensiones::Modulos() const
{
	return m_Modulos;
}

// Realización de la interfaz IControladorExtensiones
GADVAPI::PrivateExtensionManager& GNC::ControladorExtensiones::GetPrivateExtensionsManager()
{
	return m_Manager;
}

void GNC::ControladorExtensiones::NotificarRegistro(GNC::GCS::IModuleController* /*pModulo*/)
{
	/*
	GNC::Entorno* pEntorno = GNC::Entorno::Instance();
	GNC::Entorno::ListaObservadoresExtensiones::const_iterator it;
	for (it = pEntorno->ObservadoresExtensiones.begin(); it != pEntorno->ObservadoresExtensiones.end(); ++it) {
		GNC::GCS::IObservadorExtensiones* ne = *it;
		ne->OnModuloCargado(pModulo);
	}
	*/
}

void GNC::ControladorExtensiones::NotificarDesRegistro(GNC::GCS::IModuleController* /*pModulo*/)
{
	/*
	GNC::GCS::IEntorno* pEntorno = GNC::GCS::IEntorno::Instance();
	Entorno::ListaObservadoresExtensiones::const_iterator it;
	for (it = pEntorno->ObservadoresExtensiones.begin(); it != pEntorno->ObservadoresExtensiones.end(); ++it) {
		GNC::GCS::IObservadorExtensiones* ne = *it;
		ne->OnModuloDescargado(pModulo);
	}
	*/
}

//-------------------------------------------------------------------------------------------
// Helpers

inline std::wstring StringToWString(const std::string& str) {
	std::wstring temp(str.length(),L' ');
	std::copy(str.begin(), str.end(), temp.begin());
	return temp;
}


//-------------------------------------------------------------------------------------------
// PrivateExtensionManager

GADVAPI::PrivateExtensionManager::PrivateExtensionManager() : GADVAPI::PrivateExtensionManager::Base()
{

}

GADVAPI::PrivateExtensionManager::~PrivateExtensionManager()
{
	UnLoadAll();
}

GADVAPI::IPrivateExtensionWrapper* GADVAPI::PrivateExtensionManager::GetExtension(const std::string& nombre)
{
	Base& base = *this;
	if (base.find(nombre) != base.end()) {
		return base[nombre];
	} else {
		return NULL;
	}
}

void GADVAPI::PrivateExtensionManager::Scan()
{

	#if defined (MONOLITIC)

	if(empty()) {

		Base& base = *this;
		PrivateExtensionWrapper* pExt = NULL;
		
		pExt = new PrivateExtensionWrapper();
		pExt->Load(init_VisualizatorExtension());
		
		base[pExt->getSid()] = pExt;
		
		#if defined(GINKGO_PRO)

		pExt = new PrivateExtensionWrapper();
		pExt->Load(init_OfthalmologicExtension());
		base[pExt->getSid()] = pExt;
		
		pExt = new PrivateExtensionWrapper();
		pExt->Load(init_GNKProExtension());
		base[pExt->getSid()] = pExt;

		pExt = new PrivateExtensionWrapper();
		pExt->Load(init_RadiologicalExtension());
		base[pExt->getSid()] = pExt;

		pExt = new PrivateExtensionWrapper();
		pExt->Load(init_DermatologyExtension());
		base[pExt->getSid()] = pExt;
		#endif

	}

	#else

	UnLoadAll();

	wxDir dir;
	wxString rutas[1] = {
		FROMPATH(GNC::GCS::IEntorno::Instance()->GetPluginsPath())
	};

	wxString descFileName;
	wxString descFilePath;
	std::string stdExtensionDir;

	for (int i = 0; i < 1; i++) {
		if (dir.Exists(rutas[i]) && dir.Open(rutas[i])) {
			bool cont = dir.GetFirst(&descFileName, EXT, wxDIR_FILES);
			while (cont) {
				stdExtensionDir = std::string( (rutas[i] + wxFileName::GetPathSeparator()).ToUTF8() );
				descFilePath = rutas[i] + wxFileName::GetPathSeparator() + descFileName;

				std::string descStdFilePath(descFilePath.ToUTF8());

				std::ifstream file (descStdFilePath.c_str());
				std::string line;
				std::vector<std::string> tokens(2);

				/*
				DLL=file.dll
				SID=TheSid
				PROVIDER=TheProvider
				DESCRIPTION=TheDescription
				COMPILATION=TheBuildDate
				VERSION=TheVersion
				SUBVERSION=TheSubVersion
				RELEASE=TheRelease
				BUILD=TheBuild
				CODENAME=TheCodeNameString
				UPDATEURL=TheUpdateURL
				CORE_VERSION=TheCoreExactCompatibleVersion
				CORE_SUBVERSION=TheCoreExactCompatibleSubVersion
				*/

				if (file.is_open()) {

					PrivateExtensionWrapper* pExt = new PrivateExtensionWrapper();

					while(std::getline(file, line)) {

						std::string::size_type sepPos = line.find("=");
						if (sepPos != std::string::npos) {
							std::string key = line.substr(0, sepPos);
							std::string value = line.substr(sepPos+1);

							if (key == "DLL") {
								pExt->setFilePath(stdExtensionDir + value);
							}
							else if (key == "SID") {
								pExt->setSid(value);
							}
							else if (key == "PROVIDER") {
								pExt->setProvider(value);
							}
							else if (key == "DESCRIPTION") {
								pExt->setDescription(value);
							}
							else if (key == "UPDATEURL") {
								pExt->setUpdateURL(value);
							}
						}
					}

					if (this->find(pExt->getSid()) != end()) {
						LOG_ERROR("Extensions", _Std("Skipping extension with duplicated SID:") << " " << pExt->getSid());
						delete pExt;
					}
					else {
						Base& base = *this;
						base[pExt->getSid()] = pExt;
						pExt->Load();
					}

				}
				cont = dir.GetNext(&descFileName);
			}
		}
	}
	#endif
}

void GADVAPI::PrivateExtensionManager::UnLoadAll() {
	for(iterator it = begin(); it != end(); ++it)
	{
		(*it).second->UnLoad();
		delete (*it).second;
	}
	clear();
}

GADVAPI::PrivateExtensionManager::iterator GADVAPI::PrivateExtensionManager::begin() {
	return Base::begin();
}

GADVAPI::PrivateExtensionManager::iterator GADVAPI::PrivateExtensionManager::end() {
	return Base::end();
}
