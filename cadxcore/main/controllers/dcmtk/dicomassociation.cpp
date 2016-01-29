/*
 *
 *  $Id: dicomassociation.cpp $
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
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/controladorlog.h>
#include "dicomassociation.h"
#include "dicomnetwork.h"

#include <stdio.h>
#include <stdlib.h>

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

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif


const char* Association::AllTransferSyntaxes[] = {
	UID_LittleEndianImplicitTransferSyntax,	          /// Implicit VR Little Endian: Default Transfer Syntax for DICOM
	UID_LittleEndianExplicitTransferSyntax,           /// Explicit VR Little Endian
	UID_BigEndianExplicitTransferSyntax,              /// Explicit VR Big Endian
	UID_DeflatedExplicitVRLittleEndianTransferSyntax, /// Deflated Explicit VR Little Endian
	UID_JPEGProcess1TransferSyntax,                   /// JPEG Baseline (Process 1): Default Transfer Syntax for Lossy JPEG 8 Bit Image Compression
	UID_JPEGProcess2_4TransferSyntax,                 /// JPEG Extended (Process 2 & 4): Default Transfer Syntax for Lossy JPEG 12 Bit Image Compression (Process 4 only)
//	UID_JPEGProcess3_5TransferSyntax,                 /// JPEG Extended (Process 3 & 5) - RETIRED
//	UID_JPEGProcess6_8TransferSyntax,                 /// JPEG Spectral Selection, Non-Hierarchical (Process 6 & 8) - RETIRED
//	UID_JPEGProcess7_9TransferSyntax,                 /// JPEG Spectral Selection, Non-Hierarchical (Process 7 & 9) - RETIRED
//	UID_JPEGProcess10_12TransferSyntax,               /// JPEG Full Progression, Non-Hierarchical (Process 10 & 12) - RETIRED
//	UID_JPEGProcess11_13TransferSyntax,               /// JPEG Full Progression, Non-Hierarchical (Process 11 & 13) - RETIRED
	UID_JPEGProcess14TransferSyntax,                  /// JPEG Lossless, Non-Hierarchical (Process 14)
//	UID_JPEGProcess15TransferSyntax,                  /// JPEG Lossless, Non-Hierarchical (Process 15) - RETIRED
//	UID_JPEGProcess16_18TransferSyntax,               /// JPEG Extended, Hierarchical (Process 16 & 18) - RETIRED
//	UID_JPEGProcess17_19TransferSyntax,               /// JPEG Extended, Hierarchical (Process 17 & 19) - RETIRED
//	UID_JPEGProcess20_22TransferSyntax,               /// JPEG Spectral Selection, Hierarchical (Process 20 & 22) - RETIRED
//	UID_JPEGProcess21_23TransferSyntax,               /// JPEG Spectral Selection, Hierarchical (Process 21 & 23) - RETIRED
//	UID_JPEGProcess24_26TransferSyntax,               /// JPEG Full Progression, Hierarchical (Process 24 & 26) - RETIRED
//	UID_JPEGProcess25_27TransferSyntax,               /// JPEG Full Progression, Hierarchical (Process 25 & 27) - RETIRED
//	UID_JPEGProcess28TransferSyntax,                  /// JPEG Lossless, Hierarchical (Process 28) - RETIRED
//	UID_JPEGProcess29TransferSyntax,                  /// JPEG Lossless, Hierarchical (Process 29) - RETIRED
	UID_JPEGProcess14SV1TransferSyntax,               /// JPEG Lossless, Non-Hierarchical, First-Order Prediction (Process 14 [Selection Value 1]): Default Transfer Syntax for Lossless JPEG Image Compression
	UID_JPEGLSLosslessTransferSyntax,                 /// JPEG-LS Lossless Image Compression
	UID_JPEGLSLossyTransferSyntax,                    /// JPEG-LS Lossy (Near-Lossless) Image Compression
	UID_JPEG2000LosslessOnlyTransferSyntax,           /// JPEG 2000 Image Compression (Lossless Only)
	UID_JPEG2000TransferSyntax,                       /// JPEG 2000 Image Compression (Lossless or Lossy)
	UID_JPEG2000Part2MulticomponentImageCompressionLosslessOnlyTransferSyntax, /// JPEG 2000 Part 2 Multi-component Image Compression (Lossless Only)
	UID_JPEG2000Part2MulticomponentImageCompressionTransferSyntax,             /// JPEG 2000 Part 2 Multi-component Image Compression (Lossless or Lossy)
	UID_RLELosslessTransferSyntax,                  /// RLE Lossless
	UID_MPEG2MainProfileAtMainLevelTransferSyntax, /// MPEG2 Main Profile @ Main Level
	UID_MPEG2MainProfileAtHighLevelTransferSyntax, /// MPEG2 Main Profile @ High Level
	"1.2.840.10008.1.2.4.102",                     /// MPEG-4 AVC/H.264 High Profile / Level 4.1
	"1.2.840.10008.1.2.4.103"                      /// MPEG-4 AVC/H.264 BD-compatible High Profile / Level 4.1
	
};

const unsigned int Association::AllTransferSyntaxesCount = 19;

Association::Association(const std::string& _ambitolog) :
ambitolog(_ambitolog),
m_pNotificadorProgreso(NULL),
m_Stop(false),
m_calledPort(0),
m_timeout(120),
assoc(NULL),
presId(0),
msgId(0),
Net(NULL),
Role(RT_Requestor),
AcceptorPort(0),
m_TLS(false),
m_UseUserPass(false),
m_Validate(false)
{
    sopClass[0] = 0;
    sopInstance[0] = 0;
}

Association::~Association() {
    // drop an existing association on shutdown
    if (assoc != NULL)
		Drop();
	assoc = NULL;
	m_pNotificadorProgreso = NULL;
}

OFCondition Association::Drop(OFCondition cond) {
    // tear down association
    if (cond == EC_Normal) {
		/* release association */
		cond = ASC_releaseAssociation(assoc);
    }
    else if (cond == DUL_PEERREQUESTEDRELEASE) {
		 cond = ASC_acknowledgeRelease(assoc);
		 if (cond.good()) {
			cond = ASC_abortAssociation(assoc);
			if (cond.good()) {
				return cond;
			}
		 }
    }
    else if (cond == DUL_PEERABORTEDASSOCIATION) {
		return cond;
    }
    else {
		cond = ASC_abortAssociation(assoc);
		if (cond.good()) {
			return cond;
		}
    }

    Destroy();
    return cond;
}

void Association::Destroy() {
    OFCondition cond = ASC_destroyAssociation(&assoc);

    Net = NULL;
    assoc = NULL;
    msgId = 0;
    presId = 0;
    sopClass[0] = 0;
    sopInstance[0] = 0;

}

OFCondition Association::SendObject(DcmDataset *dataset) {
	OFCondition cond = EC_Normal;
    DcmDataset *statusDetail = NULL;

	 if (Stopped()) {
		 return DUL_NETWORKCLOSED;
	 }

    T_DIMSE_C_StoreRQ req;
    T_DIMSE_C_StoreRSP rsp;

    // check if we SOPClass and SOPInstance in dataset
    if (!DU_findSOPClassAndInstanceInDataSet(dataset, sopClass, sopInstance)) {
		return makeOFCondition(OFM_dcmnet, DIMSEC_BADDATA, OF_error, "No SOPClass or SOPInstanceUID found on dataset");
    }

    /* which presentation context should be used */
    presId = ASC_findAcceptedPresentationContextID(assoc, sopClass);
    if (presId == 0) {
		const char *modalityName = dcmSOPClassUIDToModality(sopClass);
		if (!modalityName) modalityName = dcmFindNameOfUID(sopClass);
		if (!modalityName) modalityName = "unknown SOP class";
		std::ostringstream os;
		os << "No valid presentation context found. SOPClass = " << sopClass << " Modality = " << modalityName;
		return makeOFCondition(OFM_dcmnet, DIMSEC_BADDATA, OF_error, os.str().c_str());
    }

    bzero((char*) & req, sizeof (req));
    req.MessageID = msgId;
    strncpy(req.AffectedSOPClassUID, sopClass, sizeof (req.AffectedSOPClassUID));
    strncpy(req.AffectedSOPInstanceUID, sopInstance, sizeof (req.AffectedSOPInstanceUID));
    req.DataSetType = DIMSE_DATASET_PRESENT;
    req.Priority = DIMSE_PRIORITY_LOW;

    // convert to accepted transfer syntax
    T_ASC_PresentationContext pc;
    cond = ASC_findAcceptedPresentationContext(assoc->params, presId, &pc);
    ASC_dumpPresentationContext(&pc, COUT);

    DcmXfer opt_oxferSyn(pc.acceptedTransferSyntax);
    E_TransferSyntax ori_oxferSyn = dataset->getOriginalXfer();

    DcmXfer original_xfer(dataset->getOriginalXfer());

    if (opt_oxferSyn.getXfer() != ori_oxferSyn) {
		LOG_DEBUG(ambitolog, "Converting object into accepted Transfer-Syntax: " << opt_oxferSyn.getXferName());

		OFCondition cond;
		// create RepresentationParameter
		DJ_RPLossless rp_lossless(6, 0);
		DJ_RPLossy rp_lossy(70);

		// NEW

		const DcmRepresentationParameter *rp = NULL;

		if (opt_oxferSyn.getXfer() == EXS_JPEGProcess14SV1 || opt_oxferSyn.getXfer() == EXS_JPEGProcess14) {
			rp = &rp_lossless;
		}
		else if (opt_oxferSyn.getXfer() == EXS_JPEGProcess1 || opt_oxferSyn.getXfer() == EXS_JPEGProcess2_4) {
			rp = &rp_lossy;
		}

		// recompress ?
		if (rp != NULL) {
			if (original_xfer.isEncapsulated()) {
				LOG_DEBUG(ambitolog, "The DICOM file is already compressed. It will previously converted to uncompressed Transfer Syntax");
				if (EC_Normal != dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL)) {
					return makeOFCondition(OFM_dcmnet, DIMSEC_BADDATA, OF_error, "Unable to convert the original format to uncompressed Transfer Syntax");
				}
			}
		}

		cond = dataset->chooseRepresentation(opt_oxferSyn.getXfer(), rp);
		if (cond.bad()) {
			LOG_ERROR(ambitolog, "Error choosing representation: " << cond.text());
		}

		if (dataset->canWriteXfer(opt_oxferSyn.getXfer())) {
			LOG_DEBUG(ambitolog, "The output transfer syntax (" <<  opt_oxferSyn.getXferName() << " can be writen");
		}
		else {
			std::ostringstream os;
			os << "Unable to find any possible converson to output Transfer Syntax " <<  opt_oxferSyn.getXferName();
			return makeOFCondition(OFM_dcmnet, DIMSEC_BADDATA, OF_error, os.str().c_str());
		}
    }

    // store it

    cond = DIMSE_storeUser(
						   assoc,
						   presId,
						   &req,
						   NULL,
						   dataset,
						   NULL,
						   NULL,
						   (m_timeout == 0) ? DIMSE_BLOCKING : DIMSE_NONBLOCKING,
						   m_timeout,
						   &rsp,
						   &statusDetail);

    // increase message id
    msgId++;

    // what happened

    if (rsp.DataSetType == DIMSE_DATASET_PRESENT) {
		LOG_DEBUG(ambitolog, "Response with dataset");
    }

    if (statusDetail != NULL) {
		LOG_TRACE(ambitolog, "Status: " << DumpDataset(statusDetail));
		delete statusDetail;
    }

    if (cond != EC_Normal) {
		return cond;
    }

	if (rsp.DimseStatus == STATUS_Success) {
		return EC_Normal;
	}
	else {
		LOG_ERROR(ambitolog, "DIMSE Status failed: " << rsp.DimseStatus);
		return DIMSE_BADDATA;
	}
}

OFCondition Association::SendObject(DcmFileFormat *dcmff) {
	if (Stopped()) {
		return DUL_NETWORKCLOSED;
	}
   DcmDataset* dataset = dcmff->getDataset();
   return SendObject(dataset);
}

void Association::Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, const char *abstractSyntax) {
    // no connected association till now
    assoc = NULL;

    // fill in parameters
    if (abstractSyntax != NULL) {
		m_abstractSyntax = (char*) abstractSyntax;
    }

    m_calledAET = title;
    m_calledPeer = peer;
    m_calledPort = port;

    m_ourAET = ouraet;

    msgId = 0;
    presId = 0;
}

OFCondition Association::SendEchoRequest() {
    DIC_US status;
    DcmDataset *statusDetail = NULL;

	 if (Stopped()) {
		 return DUL_NETWORKCLOSED;
	 }

    OFCondition cond = DIMSE_echoUser(assoc, ++msgId, DIMSE_BLOCKING, 0, &status, &statusDetail);
    if (cond.good()) {
		LOG_DEBUG(ambitolog, "DIMSE ECHO completed. Status: " << DU_cstoreStatusString(status));
    }
    else {
		LOG_DEBUG(ambitolog, "DIMSE ECHO Failed: " << cond.text());
    }

    if (statusDetail != NULL) {
		delete statusDetail;
    }

    return cond;
}

bool Association::AddKey(DcmDataset *query, const DcmTagKey& tag, int value) {
    static char temp[16];
#if defined(_WINDOWS)
	 sprintf_s(temp, 16, "%i", value);
#else
    snprintf(temp, 16, "%i", value);
#endif
    return AddKey(query, tag, temp);
}

bool Association::AddKey(DcmDataset *query, const DcmTagKey& tag, double value, const char* format) {
    static char temp[16];
 #if defined(_WINDOWS)
	 sprintf_s(temp, 16, format, value);
#else
    snprintf(temp, 16, format, value);
#endif
    return AddKey(query, tag, temp);
}

bool Association::AddKey(DcmItem *query, const DcmTagKey& tag, int value) {
    static char temp[16];
#if defined(_WINDOWS)
	 sprintf_s(temp, 16, "%i", value);
#else
    snprintf(temp, 16, "%i", value);
#endif
    return AddKey(query, tag, temp);
}

bool Association::AddKey(DcmItem *query, const DcmTagKey& tag, double value, const char* format) {
    static char temp[16];
#if defined(_WINDOWS)
	 sprintf_s(temp, 16, format, value);
#else
    snprintf(temp, 16, format, value);
#endif
    return AddKey(query, tag, temp);
}

bool Association::AddKey(DcmDataset *query, const DcmTagKey& t, const char* value) {
    return AddCustomKey/*< DcmDataset >*/(query, t, value);
}

bool Association::AddKey(DcmItem *query, const DcmTagKey& t, const char* value) {
    return AddCustomKey/*< DcmItem >*/(query, t, value);
}

bool Association::AddQueryLevel(DcmDataset *query, const std::string& level) {
    return AddKey(query, DCM_QueryRetrieveLevel, level.c_str());
}

GIL::DICOM::DCMTK::Network* Association::GetNetwork() {
    return Net;
}

OFCondition Association::Connect(GIL::DICOM::DCMTK::Network *network, int pdu) {
    Net = network;
    return network->ConnectAssociation(this, pdu);
}

const std::string& Association::GetOurAET() {
    return m_ourAET;
}

const char* Association::GetKey(DcmDataset* query, const DcmTagKey& tag) {
    OFString val;
    static char buffer[129];
    query->findAndGetOFString(tag, val, 0, OFTrue);
	 #if defined(_WINDOWS)
	 strncpy_s(buffer, val.c_str(), sizeof (buffer));
#else
    strncpy(buffer, val.c_str(), sizeof (buffer));
#endif

    return buffer;
}

void Association::SetTimeout(int t) {
    m_timeout = t;
}

int Association::GetTimeout() {
    return m_timeout;
}

OFCondition Association::addAllStoragePresentationContexts(T_ASC_Parameters *params, bool /*bProposeCompression*/, int /*lossy*/) {
    unsigned int pid = 1;

	OFCondition cond = ASC_addPresentationContext(params, pid, m_abstractSyntax.c_str(), AllTransferSyntaxes, 3);
	if (cond.bad()) {
		LOG_ERROR(ambitolog, "Unable to add presentation context for " << m_abstractSyntax);
		return cond;
	}

	pid += 2;

	if (pid >= 255) {
		LOG_WARN(ambitolog, "Too many PresentationContexts setted");
	}

    return cond;
}
