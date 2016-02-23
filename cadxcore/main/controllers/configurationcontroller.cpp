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

#include <wx/string.h>
#include <api/globals.h>
#include "controladorlog.h"
#include "configurationcontroller.h"
#include <main/controllers/configurationcontroller.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/file.h>
#ifdef __WINDOWS__
#include <wx/msw/private.h>     // includes <windows.h>
#endif

namespace GNC {
	namespace GCS {
		ProxySettings::ProxySettings()
		{
			useProxy = false;
		}
		ProxySettings::~ProxySettings()
		{
		}


		ConfigurationController* ConfigurationController::m_pInstance = NULL;
		wxCriticalSection ConfigurationController::m_criticalSection;

		wxString ConfigurationController::m_PreferredGeneralConfigPath;

		ConfigurationController* ConfigurationController::Instance() {
			if (m_pInstance == NULL) {
				wxCriticalSectionLocker locker(m_criticalSection);
				m_pInstance = new ConfigurationController();
			}
			return m_pInstance;	  
		}

		void ConfigurationController::FreeInstance() {
			wxCriticalSectionLocker locker(m_criticalSection);
			if (m_pInstance != NULL) {
				delete m_pInstance;
				m_pInstance = NULL;
			}			
		}

		void ConfigurationController::SetPreferredConfigFile(const std::string& path) {
			m_PreferredGeneralConfigPath = wxString(FROMPATH(path));
		}

		void ConfigurationController::Flush()
		{
			wxFileName user(m_pathUserConfig);
			if (user.IsFileWritable()) {
				m_pUserConfig->Flush();
			} 
			wxFileName global(m_pathGlobalConfig);
			if (global.IsFileWritable()) {
				m_pGeneralConfig->Flush();
			}
		}

		bool ConfigurationController::deleteEntryGeneral(const std::string& scope, const std::string&key)
		{
			return deleteEntry(TC_General, scope, key);
		}

		bool ConfigurationController::writeStringGeneral(const std::string& scope, const std::string& key, const std::string& value)
		{
			return writeString(TC_General, scope, key, value);
		}

		bool ConfigurationController::readStringGeneral(const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue)
		{
			return readString(TC_General, scope, key, value, defaultValue);
		}

		bool ConfigurationController::readBoolGeneral(const std::string& scope, const std::string& key, bool& value, bool defaultValue)
		{
			return readBool(TC_General, scope, key, value, defaultValue);
		}

		bool ConfigurationController::writeBoolGeneral(const std::string& scope, const std::string& key, bool value)
		{
			return writeBool(TC_General, scope, key, value);
		}

		bool ConfigurationController::readIntGeneral(const std::string& scope, const std::string& key, int& value, int defaultValue)
		{
			return readInt(TC_General, scope, key, value, defaultValue);
		}

		bool ConfigurationController::writeIntGeneral(const std::string& scope, const std::string& key, int value)
		{
			return writeInt(TC_General, scope, key, value);
		}

		bool ConfigurationController::readDoubleGeneral(const std::string& scope, const std::string& key, double& value, double defaultValue)
		{
			return readDouble(TC_General, scope, key, value, defaultValue);
		}

		bool ConfigurationController::writeDoubleGeneral(const std::string& scope, const std::string& key, double value)
		{
			return writeDouble(TC_General, scope, key, value);
		}

		bool ConfigurationController::writeGroupGeneral(const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix)
		{
			return writeGroup(TC_General, scope, listOfGroups, prefix);
		}

		bool ConfigurationController::readGroupGeneral(const std::string& scope, TListGroups& listOfGroups) 
		{
			return readGroup(TC_General, scope, listOfGroups);
		}

		bool ConfigurationController::deleteEntryUser(const std::string& scope, const std::string&key)
		{
			return deleteEntry(TC_User, scope, key);
		}

		bool ConfigurationController::writeStringUser(const std::string& scope, const std::string& key, const std::string& value)
		{
			return writeString(TC_User, scope, key, value);
		}

		bool ConfigurationController::readStringUser(const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue)
		{
			if (!readString(TC_General, scope, key, value, defaultValue)) {
				if (!readString(TC_Machine, scope, key, value, defaultValue)) { 
					return readString(TC_User, scope, key, value, defaultValue);
				}
			}
			return true;			
		}

		bool ConfigurationController::readBoolUser(const std::string& scope, const std::string& key, bool& value, bool defaultValue)
		{
			if (!readBool(TC_General, scope, key, value, defaultValue)) {
				if (!readBool(TC_Machine, scope, key, value, defaultValue)) {
					return readBool(TC_User, scope, key, value, defaultValue);
				}
			} 
			return true;
		}

		bool ConfigurationController::writeBoolUser(const std::string& scope, const std::string& key, bool value)
		{
			return writeBool(TC_User, scope, key, value);
		}

		bool ConfigurationController::readIntUser(const std::string& scope, const std::string& key, int& value, int defaultValue)
		{
			if (!readInt(TC_General, scope, key, value, defaultValue)) {
				if (!readInt(TC_Machine, scope, key, value, defaultValue)) {
					return readInt(TC_User, scope, key, value, defaultValue);	
				}
			} 
			return true;			
		}

		bool ConfigurationController::writeIntUser(const std::string& scope, const std::string& key, int value)
		{
			return writeInt(TC_User, scope, key, value);
		}

		bool ConfigurationController::readDoubleUser(const std::string& scope, const std::string& key, double& value, double defaultValue)
		{
			if (!readDouble(TC_General, scope, key, value, defaultValue)) {
				if (!readDouble(TC_Machine, scope, key, value, defaultValue)) {
					return readDouble(TC_User, scope, key, value, defaultValue);
				}
			} 
			return true;
		}

		bool ConfigurationController::writeDoubleUser(const std::string& scope, const std::string& key, double value)
		{
			return writeDouble(TC_User, scope, key, value);
		}

		bool ConfigurationController::writeGroupUser(const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix)
		{
			return writeGroup(TC_User, scope, listOfGroups, prefix);
		}

		bool ConfigurationController::readGroupUser(const std::string& scope, TListGroups& listOfGroups) 
		{
			if (!readGroup(TC_General, scope, listOfGroups)) {
				if (!readGroup(TC_Machine, scope, listOfGroups)) {
					return readGroup(TC_User, scope, listOfGroups);
				}
			}
			return true;
		}

		//proxy settings..
		ProxySettings ConfigurationController::GetProxySettings()
		{
			ProxySettings settings;
			readStringUser("/GinkgoCore/Station/Proxy", "Hostname", settings.hostName);
			readStringUser("/GinkgoCore/Station/Proxy", "Username", settings.userName);
			readStringUser("/GinkgoCore/Station/Proxy", "Password", settings.password);
			readStringUser("/GinkgoCore/Station/Proxy", "Exceptions", settings.exceptions);
			readIntUser("/GinkgoCore/Station/Proxy", "Port", settings.port);
			readBoolUser("/GinkgoCore/Station/Proxy", "UseProxy", settings.useProxy, false);
			readBoolUser("/GinkgoCore/Station/Proxy", "RequiresAuth", settings.requiresAuth, false);

			return settings;
		}

		void ConfigurationController::SetProxySettings(const ProxySettings& settings)
		{
			writeStringUser("/GinkgoCore/Station/Proxy", "Hostname", settings.hostName);
			writeStringUser("/GinkgoCore/Station/Proxy", "Username", settings.userName);
			writeStringUser("/GinkgoCore/Station/Proxy", "Password", settings.password);
			writeStringUser("/GinkgoCore/Station/Proxy", "Exceptions", settings.exceptions);
			writeIntUser("/GinkgoCore/Station/Proxy", "Port", settings.port);
			writeBoolUser("/GinkgoCore/Station/Proxy", "UseProxy", settings.useProxy);
			writeBoolUser("/GinkgoCore/Station/Proxy", "RequiresAuth", settings.requiresAuth);
		}
		//

		bool ConfigurationController::saveGlobalConfigurationFile(const std::string& path)
		{
			wxString pathWx = FROMPATH(path);
			wxCriticalSectionLocker locker(m_criticalSection);
			wxFileOutputStream ostream(pathWx);

			if (m_pPreferredGeneralConfig != NULL) {
				return false;
			}

			return m_pGeneralConfig->Save(ostream);	
		}

		bool ConfigurationController::loadGlobalConfigurationFile(const std::string& path)
		{			

			if (m_pPreferredGeneralConfig != NULL) {
				return false;
			}

			wxString pathWx = FROMPATH(path);
			wxCriticalSectionLocker locker(m_criticalSection);
			delete m_pGeneralConfig;
			bool res = wxCopyFile(pathWx, m_pathGlobalConfig, true);
			if (!res) {
				LOG_WARN("ConfigurationController", "Error Loading configuration file " << path);
			}
			m_pGeneralConfig = new wxFileConfig(wxEmptyString,wxEmptyString,m_pathGlobalConfig,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
			return res;
		}

		bool ConfigurationController::deleteEntry(TConfig type, const std::string& scope, const std::string&key)
		{

			if (m_pPreferredGeneralConfig != NULL) {
				return false;
			}

			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			if (type==TC_General) {
				config = m_pGeneralConfig;
			} else {
				config = m_pUserConfig;
			}

			if (key == "") {
				return config->DeleteGroup(wxString::FromUTF8(scope.c_str()));
			} else {
				config->SetPath(wxString::FromUTF8(scope.c_str()));
				return config->DeleteEntry(wxString::FromUTF8(key.c_str()));
			}
		}

		bool ConfigurationController::writeString(TConfig type, const std::string& scope, const std::string& key, const std::string& value) 
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = NULL;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			bool result = config->Write(wxString::FromUTF8(key.c_str()),wxString::FromUTF8(value.c_str()));
			Flush();   
			return result;
		}

		bool ConfigurationController::readString(TConfig type, const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue)
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = m_pPreferredGeneralConfig;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));
			wxString wxTmp;
			bool result = config->Read(wxString::FromUTF8(key.c_str()),&wxTmp,wxString::FromUTF8(defaultValue.c_str()));
			value = wxTmp.ToUTF8();
			return result;
		}

		bool ConfigurationController::writeBool(TConfig type, const std::string& scope, const std::string& key, bool value) 
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = NULL;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			bool result = config->Write(wxString::FromUTF8(key.c_str()),value);
			Flush();    
			return result;
		}

		bool ConfigurationController::readBool(TConfig type, const std::string& scope, const std::string& key, bool& value, bool defaultValue)
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = m_pPreferredGeneralConfig;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			bool result = config->Read(wxString::FromUTF8(key.c_str()),&value,defaultValue);

			return result;
		}

		bool ConfigurationController::readInt(TConfig type, const std::string& scope, const std::string& key, int& value, int defaultValue)
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = m_pPreferredGeneralConfig;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			bool result = config->Read(wxString::FromUTF8(key.c_str()),&value,defaultValue);

			return result;
		}

		bool ConfigurationController::writeInt(TConfig type, const std::string& scope, const std::string& key, int value) 
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = NULL;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			bool result = config->Write(wxString::FromUTF8(key.c_str()),value);
			Flush();    
			return result;
		}

		bool ConfigurationController::readDouble(TConfig type, const std::string& scope, const std::string& key, double& value, double defaultValue)
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = m_pPreferredGeneralConfig;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			bool result = config->Read(wxString::FromUTF8(key.c_str()),&value,defaultValue);

			return result;
		}

		bool ConfigurationController::writeDouble(TConfig type, const std::string& scope, const std::string& key, double value) 
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = NULL;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			bool result = config->Write(wxString::FromUTF8(key.c_str()),value);
			Flush();    
			return result;
		}

		bool ConfigurationController::writeGroup(TConfig type, const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix)
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = NULL;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->DeleteGroup(wxString::FromUTF8(scope.c_str()));
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			int i=0;
			bool ok = true;
			for (TListGroups::const_iterator itGroups = listOfGroups.begin(); itGroups != listOfGroups.end(); ++itGroups, ++i) {
				for (TMapValues::const_iterator itValues = (*itGroups).begin(); itValues != (*itGroups).end(); ++itValues) {
					std::ostringstream ostrKey;
					ostrKey << prefix << i << "/" << (*itValues).first;
					ok &= config->Write(wxString::FromUTF8(ostrKey.str().c_str()), wxString::FromUTF8((*itValues).second.c_str()));
				}
			}
			return ok;
		}

		bool ConfigurationController::readGroup(TConfig type, const std::string& scope, TListGroups& listOfGroups)
		{
			wxCriticalSectionLocker locker(m_criticalSection);
			wxConfigBase * config;
			switch (type) {
			case TC_User:
				config = m_pUserConfig;
				break;
			case TC_Machine: 
				config = m_pMachineConfig;
				break;
			case TC_General:
			default:
				if (m_pPreferredGeneralConfig != NULL) {
					config = m_pPreferredGeneralConfig;
				}
				else {
					config = m_pGeneralConfig;
				}
			}
			if (config == NULL) {
				return false;
			}
			config->SetPath(wxString::FromUTF8(scope.c_str()));

			long dummy;
			wxString str;
			bool bCont = config->GetFirstGroup(str, dummy);
			while ( bCont ) {
				config->SetPath(str);
				TMapValues mapTmp;
				long dummy2;
				wxString str2;
				bool bCont2 = config->GetFirstEntry(str, dummy2); 
				while (bCont2) {
					config->Read(str, &str2);
					mapTmp[std::string(str.ToUTF8())] = std::string(str2.ToUTF8());
					bCont2 = config->GetNextEntry(str, dummy2);
				}
				listOfGroups.push_back(mapTmp);
				config->SetPath(wxT(".."));
				bCont = config->GetNextGroup(str, dummy);
			}
			return !listOfGroups.empty();
		}


		/**Overwrite wxWidgets home user searching*/
		wxString GetOverwrittedHomeDir(wxString *pstr)
		{
			wxString& strDir = *pstr;

			// first branch is for Cygwin
#if defined(__UNIX__) && !defined(__WINE__)
			const wxChar *szHome = wxGetenv(wxT("HOME"));
			if ( szHome == NULL ) {
				// we're homeless...
				//wxLogWarning(_("can't find user's HOME, using current directory."));
				strDir = wxT(".");
			}
			else
				strDir = szHome;

			// add a trailing slash if needed
			if ( strDir.Last() != wxT('/') )
				strDir << wxT('/');

#ifdef __CYGWIN__
			// Cygwin returns unix type path but that does not work well
			static wxChar windowsPath[MAX_PATH];
			cygwin_conv_to_full_win32_path(strDir, windowsPath);
			strDir = windowsPath;
#endif
#elif defined(__WXWINCE__)
			strDir = wxT("\\");
#elif defined(__WXWINDOWS__)
			strDir.clear();

			const wxChar *szHome = wxGetenv(wxT("HOMEDRIVE"));
			if ( szHome != NULL )
				strDir << szHome;
			szHome = wxGetenv(wxT("HOMEPATH"));

			if ( szHome != NULL )
			{
				strDir << szHome;

				// the idea is that under NT these variables have default values
				// of "%systemdrive%:" and "\\". As we don't want to create our
				// config files in the root directory of the system drive, we will
				// create it in our program's dir. However, if the user took care
				// to set HOMEPATH to something other than "\\", we suppose that he
				// knows what he is doing and use the supplied value.
				if ( wxStrcmp(szHome, wxT("\\")) == 0 )
					strDir.clear();
			} else {
				if ( strDir.empty() )
				{
					// If we have a valid USERPROFILE directory, as is the case in
					// Windows NT, 2000 and XP, we should use that as our home directory.
					szHome = wxGetenv(wxT("USERPROFILE"));

					if ( szHome != NULL ) {
						strDir = szHome;
					} else {
						// If we have a valid HOME directory, as is used on many machines that
						// have unix utilities on them, we should use that.
						szHome = wxGetenv(wxT("HOME"));
						if ( szHome != NULL )
						{

							strDir = szHome;
						} 
					}
				}
			}				

			if ( !strDir.empty() )
			{
				// sometimes the value of HOME may be "%USERPROFILE%", so reexpand the
				// value once again, it shouldn't hurt anyhow
				strDir = wxExpandEnvVars(strDir);
			}
			else // fall back to the program directory
			{
				// extract the directory component of the program file name
				wxFileName::SplitPath(wxGetFullModuleName(), &strDir, NULL, NULL);
			}
#else 
			strDir.clear();

			// If we have a valid HOME directory, as is used on many machines that
			// have unix utilities on them, we should use that.
			const wxChar *szHome = wxGetenv(wxT("HOME"));

			if ( szHome != NULL )
			{
				strDir = szHome;
			}
			else // no HOME, try HOMEDRIVE/PATH
			{
				szHome = wxGetenv(wxT("HOMEDRIVE"));
				if ( szHome != NULL )
					strDir << szHome;
				szHome = wxGetenv(wxT("HOMEPATH"));

				if ( szHome != NULL )
				{
					strDir << szHome;

					// the idea is that under NT these variables have default values
					// of "%systemdrive%:" and "\\". As we don't want to create our
					// config files in the root directory of the system drive, we will
					// create it in our program's dir. However, if the user took care
					// to set HOMEPATH to something other than "\\", we suppose that he
					// knows what he is doing and use the supplied value.
					if ( wxStrcmp(szHome, wxT("\\")) == 0 )
						strDir.clear();
				}
			}

			if ( strDir.empty() )
			{
				// If we have a valid USERPROFILE directory, as is the case in
				// Windows NT, 2000 and XP, we should use that as our home directory.
				szHome = wxGetenv(wxT("USERPROFILE"));

				if ( szHome != NULL )
					strDir = szHome;
			}

			if ( !strDir.empty() )
			{
				// sometimes the value of HOME may be "%USERPROFILE%", so reexpand the
				// value once again, it shouldn't hurt anyhow
				strDir = wxExpandEnvVars(strDir);
			}
			else // fall back to the program directory
			{
				// extract the directory component of the program file name
				wxSplitPath(wxGetFullModuleName(), &strDir, NULL, NULL);
			}
#endif  // UNIX

			return strDir.c_str();
		}

		wxString GetOverwrittedLocalDir()
		{
			wxString strDir;

#if defined(__WXMAC__) || defined(__DOS__)
			// no local dir concept on Mac OS 9 or MS-DOS
			strDir << GetGlobalDir() ;
#else
			GetOverwrittedHomeDir(&strDir);

#ifdef  __UNIX__
			if (
				(strDir.Last() != wxT('/'))
#ifdef __VMS
				&& (strDir.Last() != wxT(']'))
#endif
				)
				strDir << wxT('/');
#else
			if (strDir.Last() != wxT('\\'))
				strDir << wxT('\\');
#endif
#endif

			return strDir;
		}
		wxString GetLocalFileName(const wxString& appName)
		{
#ifdef __VMS__
			// On VMS I saw the problem that the home directory was appended
			// twice for the configuration file. Does that also happen for
			// other platforms?
			wxString str = wxT( '.' );
#else
			wxString str = GetOverwrittedLocalDir();
#endif

#if defined( __UNIX__ ) && !defined( __VMS ) && !defined( __WXMAC__ )
			str << wxT('.');
#endif

			str << appName;

#if defined(__WINDOWS__) || defined(__DOS__)
			if ( wxStrchr(appName, wxT('.')) == NULL )
				str << wxT(".ini");
#endif

#ifdef __WXMAC__
			str << wxT(" Preferences") ;
#endif

			return str;
		}

		ConfigurationController::ConfigurationController() 
		{			

			if (!m_PreferredGeneralConfigPath.IsEmpty()) {
				m_pPreferredGeneralConfig = new wxFileConfig(wxEmptyString, wxEmptyString, m_PreferredGeneralConfigPath, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
			}
			else {
				m_pPreferredGeneralConfig = NULL;
			}
			wxString localFileconfig = GetLocalFileName(wxT("GinkgoUser"));
			m_pUserConfig = new wxFileConfig(wxT("GinkgoUser"), wxT("Metaemotion"), localFileconfig, wxEmptyString, wxCONFIG_USE_LOCAL_FILE );
			
			wxString pathEjecutable = wxStandardPaths::Get().GetExecutablePath();
			wxFileName fileName(pathEjecutable);
			//= than execute
			fileName.SetExt(wxT("ini"));
			m_pathGlobalConfig = fileName.GetLongPath();
			if(wxFileExists(m_pathGlobalConfig)) {
				m_pGeneralConfig = new wxFileConfig(wxEmptyString,wxEmptyString,m_pathGlobalConfig,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
			} else {
				//Ginkgo.ini
				m_pathGlobalConfig = wxPathOnly(m_pathGlobalConfig) +  wxFileName::GetPathSeparator() + wxT("GinkgoGeneral.ini");
				if(wxFileExists(m_pathGlobalConfig)) {
					m_pGeneralConfig = new wxFileConfig(wxEmptyString,wxEmptyString,m_pathGlobalConfig,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
				} else {
					m_pathGlobalConfig = wxPathOnly(m_pathGlobalConfig) +  wxFileName::GetPathSeparator() + wxT("Ginkgo.ini");
					if(wxFileExists(m_pathGlobalConfig)) {
						m_pGeneralConfig = new wxFileConfig(wxEmptyString,wxEmptyString,m_pathGlobalConfig,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
					} else {
						//se inicializa el fichero de propiedades
						m_pathGlobalConfig = GetLocalFileName(wxT("Ginkgo"));
						m_pGeneralConfig = new wxFileConfig(wxT("Ginkgo"), wxT("Metaemotion"),m_pathGlobalConfig,wxEmptyString,wxCONFIG_USE_LOCAL_FILE );
					}
				}
			}
			m_pathUserConfig = m_pGeneralConfig->GetLocalFileName(wxT("GinkgoUser"));

			wxString pathMachineConfig = fileName.GetPath(true) +  wxT("GinkgoMachine.ini");
			if(wxFileExists(pathMachineConfig)) {
				m_pMachineConfig = new wxFileConfig(wxEmptyString,wxEmptyString,pathMachineConfig,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
			} else {
				m_pMachineConfig = NULL;
			}
		}

		ConfigurationController::~ConfigurationController() {
			Flush();
			delete m_pUserConfig;
			delete m_pGeneralConfig;
			if (m_pMachineConfig != NULL) {
				delete m_pMachineConfig;
			}
		}  

	}
}
