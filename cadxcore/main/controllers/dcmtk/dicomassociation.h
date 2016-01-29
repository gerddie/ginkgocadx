/*
 *
 *  $Id: dicomassociation.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *  Code adapted from Aeskulap
 *
 */
#pragma once

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif

#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmnet/cond.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <api/iproxynotificadorprogreso.h>
#include "helpers.h"


#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

#include <api/dicom/idicomconformance.h>


namespace GIL {
	namespace DICOM {
		namespace DCMTK {
			class Network;
		}
	}
}

class Association {
public:

	typedef enum RoleType {
		RT_Acceptor,
		RT_Requestor,
		RT_AcceptorRequestor
	} RoleType;

	std::string ambitolog;

	GNC::IProxyNotificadorProgreso* m_pNotificadorProgreso;
	/**
	Constructors
	*/

	Association(const std::string& ambitolog);
	virtual ~Association();

	/**
	Create the association object (connect through DicomNetwork::Connect(..) )
	*/
	void Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, const char *abstractSyntax = NULL);

	/*Set notificador de progreso*/
	void SetNotificadorProgreso(GNC::IProxyNotificadorProgreso* pNotificadorProgreso) {

		m_pNotificadorProgreso = pNotificadorProgreso;
	}

	bool NotificarProgreso(float progresoNormalizado, std::string& texto) {
		if(m_pNotificadorProgreso!=NULL) {
			return m_pNotificadorProgreso->NotificarProgreso(progresoNormalizado,texto);
		}
		return true;
	}

	/**
	Connect the association to a dicom network
	*/
	OFCondition Connect(GIL::DICOM::DCMTK::Network* network, int pdu = ASC_DEFAULTMAXPDU);

	void Destroy();

	/**
	Drop the association
	*/
	OFCondition Drop(OFCondition cond = EC_Normal);

	/**
	Send a dataset through the association (C-Store)
	*/
	virtual OFCondition SendObject(DcmDataset* dataset);

	/**
	Send a fileformat object through the association (C-Store)
	*/
	virtual OFCondition SendObject(DcmFileFormat* dcmff);

	/**
	Send a C-Echo request through the association
	*/
	OFCondition SendEchoRequest();

	/**
	Return the DicomNetwork this association is connected to
	*/
	GIL::DICOM::DCMTK::Network* GetNetwork();

	RoleType GetRole()
	{
		return Role;
	}

	void SetRole(RoleType role)
	{
		Role = role;
	}

	unsigned short GetAcceptorPort()
	{
		return AcceptorPort;
	}

	void SetAcceptorPort(unsigned short port)
	{
		AcceptorPort = port;
	}

	/**
	add a query key to a dataset
	*/

	static bool AddKey(DcmItem *query, const DcmTagKey& tag, const char* value = NULL);

	static bool AddKey(DcmItem *query, const DcmTagKey& tag, int value);
	static bool AddKey(DcmItem *query, const DcmTagKey& tag, double value, const char* format = "%lf");

	static bool AddKey(DcmDataset *query, const DcmTagKey& tag, const char* value = NULL);

	//template< class T >

	static bool AddCustomKey(DcmItem* query, const DcmTagKey& t, const char* value) {
		DcmTag tag(t);
		Uint16 g = tag.getGTag();
		Uint16 e = tag.getETag();

		if (tag.error() != EC_Normal) {
			printf("unknown tag: (%04x,%04x)", g, e);
			return false;
		}

		DcmElement *elem = newDicomElement(tag);
		if (elem == NULL) {
			printf("cannot create element for tag: (%04x,%04x)", g, e);
			return false;
		}

		if (value != NULL) {
			if (strlen(value) > 0) {
				OFCondition cond = elem->putString(value);

				if (!cond.good()) {
					printf("cannot put tag value: (%04x,%04x)=\"%s\"", g, e, value);
					return false;
				}
			}
		}

		delete query->remove(t);
		query->insert(elem, OFTrue);

		return true;
	}

	static bool AddKey(DcmDataset *query, const DcmTagKey& tag, int value);
	static bool AddKey(DcmDataset *query, const DcmTagKey& tag, double value, const char* format = "%lf");

	/**
	get a key from the dataset
	*/
	static const char* GetKey(DcmDataset* query, const DcmTagKey& tag);

	/**
	add a query level to a dataset
	*/
	static bool AddQueryLevel(DcmDataset* query, const std::string& level);

	const std::string& GetOurAET();

	void SetTimeout(int t);

	int GetTimeout();

	void SetTLS(const std::string& CliCert, const std::string& CliKey, bool validate) { m_TLS = true; m_CliCert = CliCert; m_CliKey = CliKey; m_Validate = validate; }
	bool IsSecure() const { return m_TLS; }

	//http://support.dcmtk.org/wiki/dcmtk/howto/useridentitynegotiation
	void SetUserPass(const std::string& PacsUser, const std::string& PacsPass) { m_UseUserPass = true; m_pacsUser = PacsUser, m_pacsPass = PacsPass; }
	bool UseUserPass() { return m_UseUserPass; }
	const std::string& GetUser() {return m_pacsUser;}
	const std::string& GetPass() {return m_pacsPass;}

	const std::string& GetCliCert() const { return m_CliCert; }
	const std::string& GetCliKey() const { return m_CliKey; }
	const bool GetValidate() const { return m_Validate; }

	static const char* AllTransferSyntaxes[];
	static const unsigned int AllTransferSyntaxesCount;

	void Stop() {
		m_Stop = true;
	}
	bool Stopped() {
		return m_Stop;
	}

protected:

	/**
	Callback function to add user defined presentation context to association parameters
	*/
	virtual void OnAddPresentationContext(T_ASC_Parameters *params) = 0;
	OFCondition addAllStoragePresentationContexts(T_ASC_Parameters *params, bool bProposeCompression, int lossy);

	/**
	Protected data
	*/

	std::string m_abstractSyntax;
	std::string m_calledAET;
	std::string m_calledPeer;
	std::string m_ourAET;

	bool m_Stop;
	int  m_calledPort;
	int  m_timeout;

	T_ASC_Association* assoc;
	T_ASC_PresentationContextID presId;
	DIC_UI sopClass;
	DIC_UI sopInstance;
	DIC_US msgId;

	GIL::DICOM::DCMTK::Network*          Net;
	RoleType                             Role;
	unsigned short                       AcceptorPort;                                  

	bool m_TLS;
	bool m_UseUserPass;
	std::string m_pacsUser;
	std::string m_pacsPass;
	std::string m_CliCert;
	std::string m_CliKey;
	bool m_Validate;

	friend class GIL::DICOM::DCMTK::Network;
};
