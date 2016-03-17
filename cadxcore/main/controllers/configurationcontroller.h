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
#include <api/controllers/iconfigurationcontroller.h>
#include <api/api.h>
#include <wx/string.h>

#include <string>
#include <map>
#include <list>
#include <wx/thread.h>

class wxFileConfig;

namespace GNC
{
namespace GCS
{
class ProxySettings
{
public:
        std::string hostName;
        std::string userName;
        std::string password;
        std::string exceptions;
        int port;
        bool useProxy;
        bool requiresAuth;

        ProxySettings();
        ~ProxySettings();
};

class EXTAPI ConfigurationController: public GNC::GCS::IConfigurationController
{
public:

        static ConfigurationController* Instance();
        static void FreeInstance();
        static void SetPreferredConfigFile(const std::string& path);

        virtual void Flush();

        virtual bool deleteEntryGeneral(const std::string& scope, const std::string&key = "");
        virtual bool writeStringGeneral(const std::string& scope, const std::string& key, const std::string& value);
        virtual bool readStringGeneral(const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue = "");
        virtual bool writeBoolGeneral(const std::string& scope, const std::string& key, bool value);
        virtual bool readBoolGeneral(const std::string& scope, const std::string& key, bool& value, bool defaultValue = true);
        virtual bool readIntGeneral(const std::string& scope, const std::string& key, int& value, int defaultValue = 0);
        virtual bool writeIntGeneral(const std::string& scope, const std::string& key, int value);
        virtual bool readDoubleGeneral(const std::string& scope, const std::string& key, double& value, double defaultValue = 0.0);
        virtual bool writeDoubleGeneral(const std::string& scope, const std::string& key, double value);
        virtual bool writeGroupGeneral(const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix = "");
        virtual bool readGroupGeneral(const std::string& scope, TListGroups& listOfGroups);

        virtual bool deleteEntryUser(const std::string& scope, const std::string&key = "");
        virtual bool writeStringUser(const std::string& scope, const std::string& key, const std::string& value);
        virtual bool readStringUser(const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue = "");
        virtual bool readBoolUser(const std::string& scope, const std::string& key, bool& value, bool defaultValue = true);
        virtual bool writeBoolUser(const std::string& scope, const std::string& key, bool value);
        virtual bool readIntUser(const std::string& scope, const std::string& key, int& value, int defaultValue = 0);
        virtual bool writeIntUser(const std::string& scope, const std::string& key, int value);
        virtual bool readDoubleUser(const std::string& scope, const std::string& key, double& value, double defaultValue = 0.0);
        virtual bool writeDoubleUser(const std::string& scope, const std::string& key, double value);
        virtual bool writeGroupUser(const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix = "");
        virtual bool readGroupUser(const std::string& scope, TListGroups& listOfGroups);

        //proxy settings..
        virtual ProxySettings GetProxySettings();
        virtual void SetProxySettings(const ProxySettings& settings);
        //

        virtual bool saveGlobalConfigurationFile(const std::string& path);
        virtual bool loadGlobalConfigurationFile(const std::string& path);
protected:
        typedef enum TConfig {
                TC_General,
                TC_Machine,
                TC_User
        } TConfig;
        bool deleteEntry(TConfig type, const std::string& scope, const std::string&key = "");
        bool writeString(TConfig type, const std::string& scope, const std::string& key, const std::string& value);
        bool readString(TConfig type, const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue = "");
        bool readBool(TConfig type, const std::string& scope, const std::string& key, bool& value, bool defaultValue = true);
        bool writeBool(TConfig type, const std::string& scope, const std::string& key, bool value);
        bool writeInt(TConfig type, const std::string& scope, const std::string& key, int value);
        bool readInt(TConfig type, const std::string& scope, const std::string& key, int& value, int defaultValue = 0);
        bool writeDouble(TConfig type, const std::string& scope, const std::string& key, double value);
        bool readDouble(TConfig type, const std::string& scope, const std::string& key, double& value, double defaultValue = 0.0);
        bool writeGroup(TConfig type, const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix = "");
        bool readGroup(TConfig type, const std::string& scope, TListGroups& listOfGroups);

        ConfigurationController();
        ~ConfigurationController();
        static ConfigurationController* m_pInstance;
        static wxCriticalSection m_criticalSection;
        static wxString m_PreferredGeneralConfigPath;

        wxFileConfig * m_pPreferredGeneralConfig;
        wxFileConfig * m_pGeneralConfig;
        wxFileConfig * m_pUserConfig;
        wxFileConfig * m_pMachineConfig;
        wxString m_pathGlobalConfig;
        wxString m_pathUserConfig;
};
}
}
