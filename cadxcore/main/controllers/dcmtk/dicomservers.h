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
#include <string>
#include <vector>
#include <map>

#include <api/api.h>
#include <api/autoptr.h>
#include <api/ilock.h>
#include <api/internationalization/internationalization.h>
#include <api/iexception.h>


//estas constantes vienen dadas por dcmtk
#define DEFAULT_PDU_LENGTH 16384
#define MAX_PDU_LENGTH 131072
#define MIN_PDU_LENGTH 4096

namespace GNC
{
namespace GCS
{
class IEntorno;
}
}

class GinkgoNoServerFoundException : public GNC::GCS::IException
{
public:
        GinkgoNoServerFoundException() throw() : GNC::GCS::IException(_Std("Server not found"), "DICOM", false) {}
};

class WLConfig
{
public:
        WLConfig();
        WLConfig( const WLConfig& o);
        WLConfig& operator=(const WLConfig& o);
        ~WLConfig();


        bool useDICOMMPPS;
        bool useAbortMessage;
        std::string abortTemplate;
        bool useCompletedMessage;
        std::string completedTemplate;
        bool useInProgressMessage;
        std::string inProgressTemplate;
};

class DicomServer
{
public:
        typedef enum TRetrieveMethod {
                MOVE = 0,
                GET,
                WADO
        } TRetrieveMethod;
        /* Constructor with all parameters*/
        DicomServer(const std::string& ID, const std::string& AET, const std::string& HostName, int port, int Lossy, bool isDefault, int PDU, bool useTLS, const std::string& pacsUser, const std::string& pacsPass, TRetrieveMethod retrieveMethod, const std::string& wadoURI, bool retrSeries, bool reuseConnection, bool verify, const std::string& cert, const std::string& key, const std::string& defaultCharset) ;

        /**local server constructor*/
        DicomServer(const std::string& ID, const std::string& AET, const std::string& HostName, int port, int PDU) ;
        /**
        default constructor
        */
        DicomServer();

        DicomServer( const DicomServer& o);

        ~DicomServer();

        DicomServer& operator=(const DicomServer& o);

        const std::string& GetID();

        const std::string& GetAET();

        const std::string& GetHostName();

        int GetPort();

        int GetLossy();

        bool GetIsDefault() ;

        int GetPDU();

        bool GetUseTLS();

        const std::string& GetPACSUser();

        const std::string& GetPACSPass();

        TRetrieveMethod GetRetrieveMethod();

        bool GetRetrieveSeries();

        bool GetReuseConnection();

        bool GetverifyCredentials();

        const std::string& GetCertificate();

        const std::string& GetPrivateKey();

        const std::string& GetDefaultCharset();

        const std::string& GetWADOURI() ;

        WLConfig& GetWlConfig() ;

        std::string ID;
        std::string AET;
        std::string HostName;
        int  Port;
        int  PDU;
        int  Lossy;
        bool isDefault;

        WLConfig wlConfig;

        TRetrieveMethod retrieveMethod;
        std::string wadoURI;
        bool reuseConnection;
        bool retrieveSeries;
        bool useTLS;
        std::string pacsUser;
        std::string pacsPass;
        bool verifyCredentials;
        std::string certificate;
        std::string privateKey;
        std::string defaultCharset;
};

class EXTAPI DicomServerList: public GNC::GCS::ILockable
{
public:
        typedef std::list<GNC::GCS::Ptr<DicomServer> > TServerList;
        typedef std::map<std::string, GNC::GCS::Ptr<DicomServer> > TServerMap;

        static DicomServerList* Instance();
        static void FreeInstance();

        void AddServer(const DicomServer& server, bool isDefault);
        bool TieneServer(const std::string& ID);
        const GNC::GCS::Ptr<DicomServer>& GetLocalServer();
        const GNC::GCS::Ptr<DicomServer>& GetServer(const std::string& ID);
        const GNC::GCS::Ptr<DicomServer>& GetDefaultServer();
        void SetDefaultServer(const std::string& ID);
        bool Empty();
        TServerList GetServerList();
        void Reload();

protected:
        DicomServerList();
        ~DicomServerList();
        TServerMap ServerMap;
        GNC::GCS::Ptr<DicomServer> LocalServer;

protected:
        static DicomServerList *m_pInstance;
};
