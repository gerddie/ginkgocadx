/*
 *
 *  $Id: dicomservice.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 */

#include "dicomservice.h"
#include "dicomnetwork.h"
#include <api/controllers/icontroladorlog.h>
#include <api/internationalization/internationalization.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <commands/incomingdicomassociationcommand.h>



#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>     /* for dcmtk version name */
#include <dcmtk/dcmjpeg/djdecode.h>  /* for dcmjpeg decoders */
#include <dcmtk/dcmjpeg/djencode.h>  /* for dcmjpeg encoders */
#include <dcmtk/dcmjpeg/djrplol.h>   /* for DJ_RPLossless */
#include <dcmtk/dcmjpeg/djrploss.h>  /* for DJ_RPLossy */
#include <dcmtk/dcmjpeg/dipijpeg.h>  /* for dcmimage JPEG plugin */
#include <dcmtk/dcmimage/diregist.h>  /* include to support color images */
#include <dcmtk/ofstd/ofcmdln.h>
#include <dcmtk/dcmnet/diutil.h>

#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djrplol.h>
#include <dcmtk/dcmjpeg/djrploss.h>

#include "tls/tls.h"
#include "tls/gtlslayer.h"
#include <dcmtk/dcmtls/tlstrans.h>


#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif
GIL::DICOM::Service::Service(const std::string& _ambitolog) : GNC::GCS::Thread(_ambitolog),
ambitolog(_ambitolog),
m_pNotificadorProgreso(NULL),
m_timeout(15),
m_Role(RT_Acceptor),
m_TLS(false),
m_UseUserPass(false),
m_Validate(false),
m_WantToStop(false),
m_acceptorPort(11111)
{
	GNC::GCS::ConfigurationController::Instance()->readIntGeneral("/GinkgoCore/PACS/Local", "Puerto", m_acceptorPort, 11112);
	this->m_pNet = NULL;
	this->m_pTLSLayer = NULL;

	this->m_rcvPDUSize = 16384;
	this->m_sendPDUSize = 16384;

	this->m_rcvTimeout = 60;
	this->m_pollTimeout = 100;
	this->m_ServiceThreadId = 0;

}

GIL::DICOM::Service::~Service() {
	// drop an existing association on shutdown	
	GNC::GCS::ILocker lock(lockerRunning);
	m_pNotificadorProgreso = NULL;
}

OFCondition GIL::DICOM::Service::Start() {
	GNC::GCS::ILocker lock(lockerRunning);
	m_WantToStop = false;

	OFCondition cond;

	switch (m_Role) {
		case RT_Acceptor:
			cond = ASC_initializeNetwork(NET_ACCEPTOR, m_acceptorPort, m_timeout, &m_pNet);
			break;
		case RT_Requestor:
			cond = ASC_initializeNetwork(NET_REQUESTOR, 0, m_timeout, &m_pNet);
			break;
		case RT_AcceptorRequestor:
			cond = ASC_initializeNetwork(NET_ACCEPTORREQUESTOR, m_acceptorPort, m_timeout, &m_pNet);
			break;
		default:
			return EC_IllegalParameter;
	}

	if (cond.bad()) {
		LOG_ERROR(ambitolog, "Unable to initialize network, check port " << m_acceptorPort << " is not in use" );
		return cond;
	}

	if (m_pTLSLayer != NULL) {
		delete m_pTLSLayer;
	}

	if (m_TLS)
	{
		m_pTLSLayer = new GTLSTransportLayer(DICOM_APPLICATION_REQUESTOR, NULL);
		if (m_pTLSLayer == NULL)
		{
			LOG_ERROR(ambitolog, _Std("Unable to create TLS transport layer"));
			//return 1;
			return EC_IllegalParameter;
		}
		m_pTLSLayer->setCertificateFromString(GetCliCert());
		m_pTLSLayer->setPrivateKeyFromString(GetCliKey());

		if (! m_pTLSLayer->checkPrivateKeyMatchesCertificate())
		{
			LOG_ERROR(ambitolog, _Std("Private key and Certificate do not match"));
			return EC_IllegalParameter;
		}

		m_pTLSLayer->addSystemTrustedCertificates();

		if (GetValidate()) {
			m_pTLSLayer->setCertificateVerification(DCV_requireCertificate);
		}
		else {
			m_pTLSLayer->setCertificateVerification(DCV_ignoreCertificate);
		}
		/*
		if (opt_dhparam && ! (tLayer->setTempDHParameters(opt_dhparam)))
		{
		LOG_WARN(assoc->ambitolog, "unable to load temporary DH parameters. Ignoring");
		}
		*/

		cond = ASC_setTransportLayer(m_pNet, m_pTLSLayer, 0);
		if (cond.bad())
		{
			LOG_ERROR(ambitolog, _Std("Error setting TLS layer: ") << cond.text());
			return EC_IllegalParameter;
		}
	}

	//TODO LEAK DICOM SERVICE
	m_ServiceThreadId = GNC::GCS::ThreadController::Launch(this, false);

	return EC_Normal;
}

void* GIL::DICOM::Service::Task() {
	OFCondition cond = EC_Normal;
	OFString temp_str;


	GNC::GCS::ILocker lock(lockerRunning);
	try {
		while (!m_WantToStop && cond.good())
		{
			cond = acceptAssociation();
		}

		cond = ASC_dropNetwork(&m_pNet);

		if (cond.bad())
		{
			LOG_ERROR(ambitolog, DimseCondition::dump(temp_str, cond));
			return NULL;
		}
	} catch (...) {
		LOG_ERROR("DicomService", "Dicom service terminated with exceptions");
	}

	return 0;

}

OFCondition GIL::DICOM::Service::acceptAssociation()
{
	

	T_ASC_Association *assoc = NULL;
	OFCondition cond;
	OFString sprofile;
	OFString temp_str;

	cond = ASC_receiveAssociation(m_pNet, &assoc, m_rcvPDUSize, NULL, NULL, m_TLS, DUL_NOBLOCK, 1);

	// if some kind of error occured, take care of it
	if (cond.bad())
	{
		// If something distinct noassociationrequest was wrong we might have to dump an error message.
		if( cond != DUL_NOASSOCIATIONREQUEST )
		{
			LOG_ERROR(ambitolog, _Std("Receiving Association failed: ") << DimseCondition::dump(temp_str, cond).c_str());
		}

		// no matter what kind of error occurred, we need to do a cleanup
		goto cleanup;
	}
	{
		char buf[BUFSIZ];

		LOG_INFO(ambitolog, _Std("Association received"));
		LOG_DEBUG(ambitolog, _Std("Parameters:") << std::endl << ASC_dumpParameters(temp_str, assoc->params, ASC_ASSOC_RQ).c_str());

		const GIL::DICOM::SOPClassMap& scps = GIL::DICOM::Conformance::GetScpSOPClasses();

		for (GIL::DICOM::SOPClassMap::const_iterator it = scps.begin(); cond.good() && it != scps.end(); ++it) {
			GIL::DICOM::ArrayHelper transfersyntaxes(scps.GetSupportedTransferSyntaxUIDs((*it).first));
			const char* scuid = (*it).second.scUID.c_str();
			cond = ASC_acceptContextsWithPreferredTransferSyntaxes(assoc->params, &scuid, 1, transfersyntaxes.array, transfersyntaxes.size); 
			transfersyntaxes.free();
		}

		if (cond.bad()) {
			LOG_ERROR(ambitolog, DimseCondition::dump(temp_str, cond).c_str());
			goto cleanup;
		}

		// set our app title 
		ASC_setAPTitles(assoc->params, NULL, NULL, m_localAET.c_str());

		// acknowledge or reject this association
		cond = ASC_getApplicationContextName(assoc->params, buf);
		if ((cond.bad()) || strcmp(buf, UID_StandardApplicationContext) != 0) {
			// reject: the application context name is not supported
			T_ASC_RejectParameters rej =
			{
				ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_APPCONTEXTNAMENOTSUPPORTED
			};

			LOG_ERROR(ambitolog, _Std("Association Rejected: Bad Application Context Name: ") << buf);
			cond = ASC_rejectAssociation(assoc, &rej);
			if (cond.bad())
			{
				LOG_DEBUG(ambitolog, DimseCondition::dump(temp_str, cond).c_str());
			}
			goto cleanup;

		}
		else if (strlen(assoc->params->theirImplementationClassUID) == 0) {
			// reject: the no implementation Class UID provided
			T_ASC_RejectParameters rej =
			{
				ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_NOREASON
			};

			LOG_ERROR(ambitolog, _Std("Association Rejected: No Implementation Class UID provided") );
			cond = ASC_rejectAssociation(assoc, &rej);
			if (cond.bad()) {
				LOG_DEBUG(ambitolog, DimseCondition::dump(temp_str, cond).c_str());
			}
			goto cleanup;
		}
		else {
			cond = ASC_acknowledgeAssociation(assoc);
			if (cond.bad())
			{
				LOG_ERROR(ambitolog, DimseCondition::dump(temp_str, cond).c_str());
				goto cleanup;
			}
			LOG_DEBUG(ambitolog, _Std("Association Acknowledged (Max Send PDV: ") << assoc->sendPDVLength << _Std(")"));
			if (ASC_countAcceptedPresentationContexts(assoc->params) == 0) {
				LOG_WARN(ambitolog, _Std("No valid presentation contexts"));
			}
			// dump the presentation contexts which have been accepted/refused 
			LOG_DEBUG(ambitolog, ASC_dumpParameters(temp_str, assoc->params, ASC_ASSOC_AC).c_str());

			std::string cmdName;
			{
				std::ostringstream os;
				os << _Std("Association(dicom://") << assoc->params->DULparams.callingPresentationAddress << _Std("@") << assoc->params->DULparams.callingAPTitle << _Std(")");
				cmdName = os.str();
			}

			GADAPI::PACS::IncomingDicomAssociationCommandParams* pCmdParams = new GADAPI::PACS::IncomingDicomAssociationCommandParams(assoc);
			GNC::GCS::ICommandController::Instance()->ProcessAsync( cmdName, new GADAPI::PACS::IncomingDicomAssociationCommand(pCmdParams, cmdName), this);

			return cond;
		}
	}


cleanup:

	cond = ASC_dropSCPAssociation(assoc);
	if (cond.bad())
	{
		LOG_FATAL(ambitolog, DimseCondition::dump(temp_str, cond).c_str());
	}

	cond = ASC_destroyAssociation(&assoc);
	if (cond.bad())
	{
		LOG_FATAL(ambitolog, DimseCondition::dump(temp_str, cond).c_str());
	}

	return cond;
}


void GIL::DICOM::Service::Stop() {

	GNC::GCS::ICommandController::Instance()->AbortarComandosDeOwner(this);
	m_WantToStop = true;

	if (m_pNet != NULL && m_pNet->network != NULL) {
		int sock = DUL_networkSocket(m_pNet->network);
		if (sock != -1) {
			#if defined(_WINDOWS)
			shutdown(sock, SD_BOTH);
			#else
			shutdown(sock, 2);
			#endif
		}
	}
}
