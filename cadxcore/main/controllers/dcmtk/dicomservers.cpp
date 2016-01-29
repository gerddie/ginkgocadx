/*
 *  
 *  $Id: dicomservers.cpp $
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
#include "dicomservers.h"
#include <iostream>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/entorno.h>
#include <api/globals.h>

WLConfig::WLConfig()
{
	this->useAbortMessage = this->useCompletedMessage = this->useInProgressMessage = false;
	this->useDICOMMPPS = true;

	GNC::GCS::Permisos::EstadoPermiso perm = GNC::GCS::IControladorPermisos::Instance()->Get("gnkwl.messages", "inprogress");
	if (perm) {
		this->inProgressTemplate = perm.GetRawValue();
	}
	perm = GNC::GCS::IControladorPermisos::Instance()->Get("gnkwl.messages", "completed");
	if (perm) {
		this->completedTemplate = perm.GetRawValue();
	}
	perm = GNC::GCS::IControladorPermisos::Instance()->Get("gnkwl.messages", "abort");
	if (perm) {
		this->abortTemplate = perm.GetRawValue();
	}
}

WLConfig::WLConfig( const WLConfig& o)
{
	*this = o;	
}

WLConfig& WLConfig::operator=(const WLConfig& o)
{
	this->abortTemplate = o.abortTemplate;
	this->completedTemplate = o.completedTemplate;
	this->inProgressTemplate = o.inProgressTemplate;
	this->useAbortMessage = o.useAbortMessage;
	this->useCompletedMessage = o.useCompletedMessage;
	this->useInProgressMessage = o.useInProgressMessage;
	this->useDICOMMPPS = o.useDICOMMPPS;
	return *this;
}

WLConfig::~WLConfig()
{
}

DicomServer::DicomServer(const std::string& pID, const std::string& pAET, const std::string& pHostName, int pPort, int pLossy, bool pIsDefault, int pPDU, bool pUseTLS,
						 const std::string& pPacsUser, const std::string& pPacsPass, TRetrieveMethod pRetrieveMethod, const std::string& pWadoURI, bool pRetrSeries,
						 bool pReuseConnection, bool pVerify, const std::string& pCert, const std::string& pKey, const std::string& pDefaultCharset) :
	ID(pID),
	AET(pAET),
	HostName(pHostName),
	Port(pPort),
	PDU(pPDU),
	Lossy(pLossy),
	isDefault(pIsDefault),
	retrieveMethod(pRetrieveMethod),
	wadoURI(pWadoURI),
	reuseConnection(pReuseConnection),
	retrieveSeries(pRetrSeries),
	useTLS(pUseTLS),
	pacsUser(pPacsUser),
	pacsPass(pPacsPass),
	verifyCredentials(pVerify),
	certificate(pCert),
	privateKey(pKey),
	defaultCharset(pDefaultCharset)
{
}

/**local server constructor*/
DicomServer::DicomServer(const std::string& pID, const std::string& pAET, const std::string& pHostName, int pPort, int pPDU) :
	ID(pID),
	AET(pAET),
	HostName(pHostName),
	Port(pPort),
	PDU(pPDU),
	Lossy(0),
	isDefault(false),
	retrieveMethod(DicomServer::GET),
	wadoURI("/wado"),
	reuseConnection(true),
	retrieveSeries(true),
	useTLS(false),
	pacsUser(""),
	pacsPass(""),
	verifyCredentials(false),
	certificate(""),
	privateKey(""),
	defaultCharset("ISO_IR 100")
{
}

/**
default constructor
*/
DicomServer::DicomServer() :
	ID(""),
	AET(""),
	HostName(""),
	Port(11112),
	PDU(16384),
	Lossy(0),
	isDefault(false),
	retrieveMethod(DicomServer::GET),
	wadoURI("/wado"),
	reuseConnection(true),
	retrieveSeries(true),
	useTLS(false),
	pacsUser(""),
	pacsPass(""),
	verifyCredentials(false),
	certificate(""),
	privateKey(""),
	defaultCharset("ISO_IR 100")
{
}

DicomServer::DicomServer( const DicomServer& o) {
	*this = o;
}

DicomServer::~DicomServer() 
{ }

DicomServer& DicomServer::operator=(const DicomServer& o)
{
	this->ID = o.ID;
	this->AET = o.AET;
	this->HostName = o.HostName;
	this->Port = o.Port;
	this->Lossy = o.Lossy;
	this->isDefault = o.isDefault;
	this->PDU = o.PDU;
	this->useTLS = o.useTLS;
	this->pacsUser = o.pacsUser;
	this->pacsPass = o.pacsPass;
	this->retrieveMethod = o.retrieveMethod;
	this->wadoURI = o.wadoURI;
	this->reuseConnection = o.reuseConnection;
	this->retrieveSeries = o.retrieveSeries;
	this->verifyCredentials = o.verifyCredentials;
	this->certificate = o.certificate;
	this->privateKey = o.privateKey;
	this->defaultCharset = o.defaultCharset;
	this->wlConfig = o.wlConfig;
	return *this;
}

const std::string& DicomServer::GetID()
{
	return ID;
}

const std::string& DicomServer::GetAET()
{
	return AET;
}

const std::string& DicomServer::GetHostName()
{
	return HostName;
}

int DicomServer::GetPort()
{
	return Port;
}

int DicomServer::GetLossy()
{
	return Lossy;
}

bool DicomServer::GetIsDefault() 
{
	return isDefault;
}

int DicomServer::GetPDU()
{
	return PDU;
}

bool DicomServer::GetUseTLS()
{
	return useTLS;
}

const std::string& DicomServer::GetPACSUser()
{
	return pacsUser;
}

const std::string& DicomServer::GetPACSPass()
{
	return pacsPass;
}

DicomServer::TRetrieveMethod DicomServer::GetRetrieveMethod()
{
	return retrieveMethod;
}

bool DicomServer::GetRetrieveSeries()
{
	return retrieveSeries;
}

bool DicomServer::GetReuseConnection()
{
	return reuseConnection;
}

bool DicomServer::GetverifyCredentials()
{
	return verifyCredentials;
}

const std::string& DicomServer::GetCertificate()
{
	return certificate;
}

const std::string& DicomServer::GetPrivateKey()
{
	return privateKey;
}

const std::string& DicomServer::GetDefaultCharset()
{
	return defaultCharset;
}

const std::string& DicomServer::GetWADOURI() {
	return wadoURI;
}

WLConfig& DicomServer::GetWlConfig()
{
	return wlConfig;
}

/////////////////////////////////////////////////////////////////////

DicomServerList* DicomServerList::m_pInstance = 0;

DicomServerList::DicomServerList()
{
	Reload();
}

DicomServerList::~DicomServerList()
{
	GNC::GCS::ILocker lock(this);
	ServerMap.clear();
}

DicomServerList* DicomServerList::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new DicomServerList();
	}
	return m_pInstance;
}

void DicomServerList::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = 0;
	}

}

void DicomServerList::AddServer(const DicomServer& newServer, bool isDefault)
{
	GNC::GCS::ILocker lock(this);
	if (isDefault) {
		for (TServerMap::iterator it = ServerMap.begin(); it != ServerMap.end(); ++it) {
			(*it).second->isDefault = false;
		}
	}
	if (ServerMap.find(newServer.ID) != ServerMap.end()) {
		*ServerMap[newServer.ID] = newServer;
		ServerMap[newServer.ID]->isDefault = isDefault;
	}
	else {
		GNC::GCS::Ptr<DicomServer> pNewServer(new DicomServer(newServer));
		pNewServer->isDefault = isDefault;
		ServerMap[newServer.ID] = pNewServer;
	}
}

bool DicomServerList::TieneServer(const std::string& ID)
{
	GNC::GCS::Ptr<DicomServer> server = GetServer(ID);
	return server.IsValid();
}

const GNC::GCS::Ptr<DicomServer>& DicomServerList::GetServer(const std::string& ID)
{
	GNC::GCS::ILocker lock(this);
	if (ServerMap.find(ID) != ServerMap.end()) {
		return ServerMap[ID];
	} else {
		GNC::GCS::Ptr<DicomServer> server;
		throw GinkgoNoServerFoundException();
	}
}

const GNC::GCS::Ptr<DicomServer>& DicomServerList::GetLocalServer()
{
	if (!LocalServer.IsValid()) {
		throw GinkgoNoServerFoundException();
	}
	return LocalServer;
}

bool DicomServerList::Empty()
{
	return ServerMap.empty();
}

DicomServerList::TServerList DicomServerList::GetServerList()
{
	GNC::GCS::ILocker lock(this);
	TServerList listOfServers;
	for (TServerMap::iterator it = ServerMap.begin(); it != ServerMap.end(); ++it) {
		listOfServers.push_back((*it).second);
	}
	return listOfServers;
}

const GNC::GCS::Ptr<DicomServer>& DicomServerList::GetDefaultServer()
{
	GNC::GCS::ILocker lock(this);
	for (TServerMap::iterator it = ServerMap.begin(); it != ServerMap.end(); ++it) {
		if ((*it).second->isDefault) {
			return (*it).second;
		}
	}
	if (!ServerMap.empty()) {
		return (*ServerMap.begin()).second;
	} else {
		throw GinkgoNoServerFoundException();
	}
}

void DicomServerList::SetDefaultServer(const std::string& ID)
{
	GNC::GCS::ILocker lock(this);
	for (TServerMap::iterator it = ServerMap.begin(); it != ServerMap.end(); ++it) {
		if ((*it).second->ID != ID) {
			(*it).second->isDefault = false;
		} else {
			(*it).second->isDefault = true;
		}
	}
}

void DicomServerList::Reload()
{
	GNC::GCS::ILocker lock(this);
	ServerMap.clear();
	
	GNC::GCS::ConfigurationController::TListGroups servers;
	GNC::GCS::ConfigurationController::Instance()->readGroupGeneral("/GinkgoCore/PACS/Servidores", servers);

	bool existDefault = false;
	for (GNC::GCS::ConfigurationController::TListGroups::iterator it = servers.begin(); it != servers.end(); ++it) {
		GNC::GCS::ConfigurationController::TMapValues& values = (*it);
		std::string id, aet, host;
		int port, pdu;
		values.readStringValue("Identificador", id);
		values.readStringValue("AET", aet);
		values.readStringValue("Host", host);
		values.readIntValue("Puerto", port);
		values.readIntValue("PDU", pdu, DEFAULT_PDU_LENGTH);

		GNC::GCS::Ptr<DicomServer> server(new DicomServer(id, aet, host, port, pdu));
		values.readBoolValue("isDefault",server->isDefault,false);
		values.readBoolValue("useTLS",server->useTLS,false);
		values.readStringValue("pacsUser", server->pacsUser);
		values.readStringValue("pacsPass", server->pacsPass);
		values.readBoolValue("verifyCredentials",server->verifyCredentials,false);
		values.readStringValue("certificate", server->certificate);
		values.readStringValue("privateKey", server->privateKey);
		int tmpInt;
		if (values.readIntValue("retrieveMethod", tmpInt)) {
			server->retrieveMethod = (DicomServer::TRetrieveMethod)tmpInt;
		} else {
		//retro compatibility
			bool tmpBool;
			values.readBoolValue("retrieveWithMove",tmpBool,true);
			if (tmpBool) {
				server->retrieveMethod = DicomServer::MOVE;
			} else {
				server->retrieveMethod = DicomServer::GET;
			}
		}
		//
		values.readStringValue("wadoURI", server->wadoURI, "");
		values.readBoolValue("reuseConnection", server->reuseConnection, false);
		values.readBoolValue("retrieveSeries",server->retrieveSeries,true);
		values.readStringValue("defaultCharset", server->defaultCharset, "ISO_IR 100");

		//wl...
		values.readBoolValue("useDICOMMPPS",server->GetWlConfig().useDICOMMPPS, server->GetWlConfig().useDICOMMPPS);
		values.readBoolValue("useAbortMessage",server->GetWlConfig().useAbortMessage, server->GetWlConfig().useAbortMessage);
		values.readBoolValue("useCompletedMessage",server->GetWlConfig().useCompletedMessage, server->GetWlConfig().useCompletedMessage);
		values.readBoolValue("useInProgressMessage",server->GetWlConfig().useInProgressMessage, server->GetWlConfig().useInProgressMessage);
		values.readStringValue("abortTemplate",server->GetWlConfig().abortTemplate, server->GetWlConfig().abortTemplate);
		values.readStringValue("completedTemplate",server->GetWlConfig().completedTemplate, server->GetWlConfig().completedTemplate);
		values.readStringValue("inProgressTemplate",server->GetWlConfig().inProgressTemplate, server->GetWlConfig().inProgressTemplate);
		//

		ServerMap[server->ID] = server;
		existDefault = existDefault || server->isDefault;
	}

	if (!existDefault && !ServerMap.empty()) {
		ServerMap.begin()->second->isDefault = true;
	}

	//local server
	//se actualiza el fichero de configuracion
	int port;
	GNC::GCS::ConfigurationController::Instance()->readIntGeneral("/GinkgoCore/PACS/Local", "Puerto", port, 11112);
	std::string localAet = GNC::Entorno::Instance()->GetDicomLocalAET();
	
	LocalServer = new DicomServer("ginkgo",localAet, "localhost", port, 16384);
	GNC::GCS::ConfigurationController::Instance()->readBoolGeneral("/GinkgoCore/PACS/Local", "useTLS", LocalServer->useTLS, false);
	GNC::GCS::ConfigurationController::Instance()->readBoolGeneral("/GinkgoCore/PACS/Local", "verifyCredentials", LocalServer->verifyCredentials, false);
#ifdef GINKGO_PRIVATE_KEY
	LocalServer->privateKey = GINKGO_PRIVATE_KEY;
#endif
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/PACS/Local", "privateKey", LocalServer->privateKey);

#ifdef GINKGO_PUBLIC_KEY
	LocalServer->certificate = GINKGO_PUBLIC_KEY;
#endif
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/PACS/Local", "certificate", LocalServer->certificate);
}

