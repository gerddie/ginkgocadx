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

#include "dicomcustomassociation.h"
#include "helpercompresion.h"
#include <main/controllers/pacscontroller.h>

#include <api/controllers/icontroladorlog.h>
#include <api/controllers/ipacscontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>

#include <main/controllers/dcmtk/ginkgouid.h>
#include <api/internationalization/internationalization.h>


#include <api/dicom/dcmdictionary.h>
//#include <main/controllers/dicommanager.h>
#include <api/dicom/dcmdictionary.h>

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

#include <wx/string.h>
#include <wx/intl.h>
#include <wx/filename.h>


void DIMSE_printNStatusString(std::ostream& dumpStream, int status)
{
	switch(status)
	{
	case STATUS_Success:
		dumpStream << "0x0000: Success";
		break;
	case STATUS_N_Cancel:
		dumpStream << "0xFE00: Cancel";
		break;
	case STATUS_N_AttributeListError:
		dumpStream << "0x0107: Attribute list error";
		break;
	case STATUS_N_SOPClassNotSupported:
		dumpStream << "0x0122: SOP class not supported";
		break;
	case STATUS_N_ClassInstanceConflict:
		dumpStream << "0x0119: Class/instance conflict";
		break;
	case STATUS_N_DuplicateSOPInstance:
		dumpStream << "0x0111: Duplicate SOP instance";
		break;
	case STATUS_N_DuplicateInvocation:
		dumpStream << "0x0210: Duplicate invocation";
		break;
	case STATUS_N_InvalidArgumentValue:
		dumpStream << "0x0115: Invalid argument value";
		break;
	case STATUS_N_InvalidAttributeValue:
		dumpStream << "0x0106: Invalid attribute value";
		break;
	case STATUS_N_InvalidObjectInstance:
		dumpStream << "0x0117: Invalid object instance";
		break;
	case STATUS_N_MissingAttribute:
		dumpStream << "0x0120: Missing attribute";
		break;
	case STATUS_N_MissingAttributeValue:
		dumpStream << "0x0121: Missing attribute value";
		break;
	case STATUS_N_MistypedArgument:
		dumpStream << "0x0212: Mistyped argument";
		break;
	case STATUS_N_NoSuchArgument:
		dumpStream << "0x0114: No such argument";
		break;
	case STATUS_N_NoSuchAttribute:
		dumpStream << "0x0105: No such attribute";
		break;
	case STATUS_N_NoSuchEventType:
		dumpStream << "0x0113: No such event type";
		break;
	case STATUS_N_NoSuchObjectInstance:
		dumpStream << "0x0112: No such object instance";
		break;
	case STATUS_N_NoSuchSOPClass:
		dumpStream << "0x0118: No such SOP class";
		break;
	case STATUS_N_ProcessingFailure:
		dumpStream << "0x0110: Processing failure";
		break;
	case STATUS_N_ResourceLimitation:
		dumpStream << "0x0213: Resource limitation";
		break;
	case STATUS_N_UnrecognizedOperation:
		dumpStream << "0x0211: Unrecognized operation";
		break;
	default:
		dumpStream << "0x" << STD_NAMESPACE hex << STD_NAMESPACE setfill('0') << STD_NAMESPACE setw(4)
			<< status << ": Unknown Status Code";
		break;
	}
}

CustomAssociation::CustomAssociation(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificadorProgreso) :
	Association(ambitolog), GIL::DICOM::ICustomAssociation()
{
	if (connectionKey == NULL) {
		LOG_ERROR(ambitolog, "NULL Connection key");
	}
	this->Net = GIL::DICOM::DCMTK::Network::Instance(connectionKey);
	this->m_pNotificadorProgreso = pNotificadorProgreso;
	this->ambitolog = ambitolog;
	this->blockMode = DIMSE_BLOCKING;
	this->eventHandler = NULL;
	this->presId = 0;
}

CustomAssociation::~CustomAssociation() {

}
bool CustomAssociation::Connect(const std::string& serverId, const std::string& local_aet)
{
	DicomServerList* listaServidores = DicomServerList::Instance();
	m_server = listaServidores->GetServer(serverId);
	if (!m_server.IsValid()) {
		LOG_ERROR("C-MOVE/C-GET", "Invalid server");
		return false;
	}

	if (!Net->Initiallized()) {
		Net->InitializeNetwork(GetTimeout(), GetRole(), GetAcceptorPort());
	}

	Create(
		m_server->AET,
		m_server->HostName,
		m_server->Port,
		local_aet
		);

	OFCondition r = Association::Connect(Net, m_server->PDU);

	if (r.bad()) {
		LOG_ERROR(ambitolog, "Error al conectar:" << r.text());
		Drop();
		Destroy();
		throw GIL::DICOM::PACSException(r.text());
	}
	return r.good();
}

void CustomAssociation::OnAddPresentationContext(T_ASC_Parameters* params) {
	
	OFCondition cond;
	if (SOPClasses.size() > 0) {
		GIL::DICOM::TransferSyntaxList tsList;
		tsList.push_back(UID_LittleEndianImplicitTransferSyntax);
		unsigned int pid = 1;
		for (GIL::DICOM::SOPClassList::const_iterator itSopClass = SOPClasses.begin(); pid <= 255 && itSopClass != SOPClasses.end() && cond.good(); ++itSopClass) {
			for (GIL::DICOM::TransferSyntaxList::const_iterator itTS = tsList.begin(); pid <= 255 && itTS != tsList.end() && cond.good(); ++itTS) {
				const char* ts[] = {(*itTS).c_str()};
				cond = ASC_addPresentationContext(params, pid, (*itSopClass).c_str(), ts, 1);
				pid += 2;
			}

		}

		if (pid >= 255) {
			LOG_WARN("C-GET", "Too many PresentationContexts setted");
		}
	} else {	
		LOG_WARN("C-GET", "NO PresentationContexts setted");
	}
}

void CustomAssociation::dumpNMessage(T_DIMSE_Message &msg, DcmItem *dataset, OFBool outgoing)
{
	OFString str;
	if (outgoing) {
		DIMSE_dumpMessage(str, msg, DIMSE_OUTGOING, dataset);
	} else {
		DIMSE_dumpMessage(str, msg, DIMSE_INCOMING, dataset);
	}
	LOG_DEBUG(ambitolog, str);
}

OFCondition CustomAssociation::sendNRequest(
	T_ASC_PresentationContextID presId,
	T_DIMSE_Message &request,
	DcmDataset *rqDataSet,
	T_DIMSE_Message &response,
	DcmDataset* &statusDetail,
	DcmDataset* &rspDataset)
{
	OFCondition cond = EC_Normal;
	T_DIMSE_Command expectedResponse;
	DIC_US expectedMessageID=0;
	if (assoc == NULL)
	{
		return DIMSE_ILLEGALASSOCIATION;
	}

	T_DIMSE_DataSetType datasetType = DIMSE_DATASET_NULL;
	if (rqDataSet && (rqDataSet->card() > 0)) datasetType = DIMSE_DATASET_PRESENT;

	switch(request.CommandField)
	{
	case DIMSE_N_GET_RQ:
		request.msg.NGetRQ.DataSetType = datasetType;
		expectedResponse = DIMSE_N_GET_RSP;
		expectedMessageID = request.msg.NGetRQ.MessageID;
		break;
	case DIMSE_N_SET_RQ:
		request.msg.NSetRQ.DataSetType = datasetType;
		expectedResponse = DIMSE_N_SET_RSP;
		expectedMessageID = request.msg.NSetRQ.MessageID;
		break;
	case DIMSE_N_ACTION_RQ:
		request.msg.NActionRQ.DataSetType = datasetType;
		expectedResponse = DIMSE_N_ACTION_RSP;
		expectedMessageID = request.msg.NActionRQ.MessageID;
		break;
	case DIMSE_N_CREATE_RQ:
		request.msg.NCreateRQ.DataSetType = datasetType;
		expectedResponse = DIMSE_N_CREATE_RSP;
		expectedMessageID = request.msg.NCreateRQ.MessageID;
		break;
	case DIMSE_N_DELETE_RQ:
		request.msg.NDeleteRQ.DataSetType = datasetType;
		expectedResponse = DIMSE_N_DELETE_RSP;
		expectedMessageID = request.msg.NDeleteRQ.MessageID;
		break;
	default:
		return DIMSE_BADCOMMANDTYPE;
		/* break; */
	}

	dumpNMessage(request, rqDataSet, OFTrue);
	cond = DIMSE_sendMessageUsingMemoryData(assoc, presId, &request, NULL, rqDataSet, NULL, NULL);
	if (cond.bad()) return cond;

	T_ASC_PresentationContextID thisPresId;
	T_DIMSE_Message eventReportRsp;
	DIC_US eventReportStatus;
	do
	{
		thisPresId = presId;
		statusDetail = NULL;
		cond = DIMSE_receiveCommand(assoc, blockMode, this->m_timeout, &thisPresId, &response, &statusDetail);
		if (cond.bad()) return cond;

		if (response.CommandField == DIMSE_N_EVENT_REPORT_RQ)
		{
			/* handle N-EVENT-REPORT-RQ */
			rspDataset = NULL;
			if (response.msg.NEventReportRQ.DataSetType == DIMSE_DATASET_PRESENT)
			{
				cond = DIMSE_receiveDataSetInMemory(assoc, blockMode, m_timeout, &thisPresId, &rspDataset, NULL, NULL);
				if (cond.bad()) return cond;
			}  
			dumpNMessage(response, rspDataset, OFFalse);
			// call event handler if registered
			eventReportStatus = STATUS_Success;
			if (eventHandler) eventReportStatus = eventHandler->handleEvent(response.msg.NEventReportRQ, rspDataset, statusDetail);
			if (rspDataset) delete rspDataset;
			rspDataset = NULL;
			if (statusDetail) delete statusDetail;
			statusDetail = NULL;

			// send back N-EVENT-REPORT-RSP */
			eventReportRsp.CommandField = DIMSE_N_EVENT_REPORT_RSP;
			eventReportRsp.msg.NEventReportRSP.MessageIDBeingRespondedTo = response.msg.NEventReportRQ.MessageID;
			eventReportRsp.msg.NEventReportRSP.EventTypeID = response.msg.NEventReportRQ.EventTypeID;
			eventReportRsp.msg.NEventReportRSP.DimseStatus = eventReportStatus;
			eventReportRsp.msg.NEventReportRSP.DataSetType = DIMSE_DATASET_NULL;
			eventReportRsp.msg.NEventReportRSP.opts = O_NEVENTREPORT_EVENTTYPEID;
			eventReportRsp.msg.NEventReportRSP.AffectedSOPClassUID[0] = 0;
			eventReportRsp.msg.NEventReportRSP.AffectedSOPInstanceUID[0] = 0;
			dumpNMessage(eventReportRsp, NULL, OFTrue);
			cond = DIMSE_sendMessageUsingMemoryData(assoc, thisPresId, &eventReportRsp, NULL, NULL, NULL, NULL);
			if (cond.bad()) return cond;
		} else {
			/* No N-EVENT-REPORT-RQ. Check if this message is what we expected */
			if (response.CommandField != expectedResponse)
			{
				char buf1[256];
				sprintf(buf1, "DIMSE: Unexpected Response Command Field: 0x%x", (unsigned)response.CommandField);
				return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, buf1);
			}
			T_DIMSE_DataSetType responseDataset = DIMSE_DATASET_NULL;
			DIC_US responseMessageID = 0;
			/** change request to response */     
			switch(expectedResponse)
			{
			case DIMSE_N_GET_RSP:
				responseDataset = response.msg.NGetRSP.DataSetType;
				responseMessageID = response.msg.NGetRSP.MessageIDBeingRespondedTo;
				break;
			case DIMSE_N_SET_RSP:
				responseDataset = response.msg.NSetRSP.DataSetType;
				responseMessageID = response.msg.NSetRSP.MessageIDBeingRespondedTo;
				break;
			case DIMSE_N_ACTION_RSP:
				responseDataset = response.msg.NActionRSP.DataSetType;
				responseMessageID = response.msg.NActionRSP.MessageIDBeingRespondedTo;
				break;
			case DIMSE_N_CREATE_RSP:
				responseDataset = response.msg.NCreateRSP.DataSetType;
				responseMessageID = response.msg.NCreateRSP.MessageIDBeingRespondedTo;
				break;
			case DIMSE_N_DELETE_RSP:
				responseDataset = response.msg.NDeleteRSP.DataSetType;
				responseMessageID = response.msg.NDeleteRSP.MessageIDBeingRespondedTo;
				break;
			default:
				{
					char buf1[256];
					sprintf(buf1, "DIMSE: Unexpected Response Command Field: 0x%x", (unsigned)response.CommandField);
					return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, buf1);
				}
				/* break; */
			}
			if (responseMessageID != expectedMessageID)
			{
				char buf1[256];
				sprintf(buf1, "DIMSE: Unexpected Response Command Field: 0x%x", (unsigned)response.CommandField);
				return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, buf1);
			}
			rspDataset = NULL;
			if (responseDataset == DIMSE_DATASET_PRESENT)
			{
				cond = DIMSE_receiveDataSetInMemory(assoc, blockMode, m_timeout, &thisPresId, &rspDataset, NULL, NULL);
				if (cond.bad()) return cond;
			}
			dumpNMessage(response, rspDataset, OFFalse);
		}
	} while (response.CommandField == DIMSE_N_EVENT_REPORT_RQ);
	return EC_Normal;
}    

bool CustomAssociation::createRQ( const std::string& sopclassUID, const GIL::DICOM::DicomDataset& attributeListIn,  int& status, std::string& affectedSopinstanceUID)
{
	if (assoc == NULL)
	{
		LOG_ERROR(ambitolog, DIMSE_ILLEGALASSOCIATION.theText);
		return false;
	}
	

	T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID.c_str());
	if (presCtx == 0)
	{
		LOG_ERROR(ambitolog, DIMSE_NOVALIDPRESENTATIONCONTEXTID.theText);
		return false;
	}  

	T_DIMSE_Message request;
	T_DIMSE_Message response;
	DcmDataset *statusDetail = NULL;

	// construct N-CREATE-RQ
	request.CommandField = DIMSE_N_CREATE_RQ;
	request.msg.NCreateRQ.MessageID = assoc->nextMsgID++;

    strncpy(request.msg.NCreateRQ.AffectedSOPClassUID, sopclassUID.c_str(), sizeof(request.msg.NCreateRQ.AffectedSOPClassUID) - 1);
    request.msg.NCreateRQ.AffectedSOPClassUID[sizeof(request.msg.NCreateRQ.AffectedSOPClassUID)-1] = 0;
	request.msg.NCreateRQ.AffectedSOPInstanceUID[0] = 0;
	request.msg.NCreateRQ.opts = 0;
	
	DcmDataset inAttr;
	DcmDataset* outAttr;
	GIL::DICOM::PACSController::Instance()->FillInQuery(attributeListIn, &inAttr, m_server);

	OFCondition cond = sendNRequest(presCtx, request, &inAttr, response, statusDetail, outAttr);
	if (statusDetail) {
		delete statusDetail;
		statusDetail = NULL;
	}
	if (cond.good()) 
	{
		status = response.msg.NCreateRSP.DimseStatus;
		if (status != 0) {
			LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
			std::ostringstream ostr;
			ostr << "Error sending create rq status: "<< status << " details: ";
			DIMSE_printNStatusString(ostr, status);
			throw GIL::DICOM::PACSException(ostr.str(), "GIL::DICOMAssociation");
		}
		// if response contains SOP Instance UID, copy it.
		if (response.msg.NCreateRSP.opts & O_NCREATE_AFFECTEDSOPINSTANCEUID)
		{
			affectedSopinstanceUID = response.msg.NCreateRSP.AffectedSOPInstanceUID;
		}
	}
	return cond.good();
}




bool CustomAssociation::setRQ( const std::string& sopclassUID, const std::string& sopinstanceUID, const GIL::DICOM::DicomDataset& modificationList, int& status)
{
	if (assoc == NULL)
	{
		LOG_ERROR(ambitolog, DIMSE_ILLEGALASSOCIATION.theText);
		return false;
	}
	

	T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID.c_str());
	if (presCtx == 0)
	{
		LOG_ERROR(ambitolog, DIMSE_NOVALIDPRESENTATIONCONTEXTID.theText);
		return false;
	}  

	T_DIMSE_Message request;
	T_DIMSE_Message response;
	DcmDataset *statusDetail = NULL;

	// construct N-CREATE-RQ
	request.CommandField = DIMSE_N_SET_RQ;
	request.msg.NSetRQ.MessageID = assoc->nextMsgID++;
    strncpy(request.msg.NSetRQ.RequestedSOPClassUID, sopclassUID.c_str(), sizeof(request.msg.NSetRQ.RequestedSOPClassUID)-1);
    request.msg.NSetRQ.RequestedSOPClassUID[sizeof(request.msg.NSetRQ.RequestedSOPClassUID)-1] = 0;

    strncpy(request.msg.NSetRQ.RequestedSOPInstanceUID, sopinstanceUID.c_str(), sizeof(request.msg.NSetRQ.RequestedSOPInstanceUID)-1);
    request.msg.NSetRQ.RequestedSOPInstanceUID[sizeof(request.msg.NSetRQ.RequestedSOPInstanceUID)-1] = 0;

	DcmDataset inAttr;
	DcmDataset* outAttr;
	GIL::DICOM::PACSController::Instance()->FillInQuery(modificationList, &inAttr, m_server);

	OFCondition cond = sendNRequest(presCtx, request, &inAttr, response, statusDetail, outAttr);
	if (cond.good()) status = response.msg.NSetRSP.DimseStatus;
	if (status != 0) {
		LOG_ERROR(ambitolog, "Error sending nset rq status: "<< status);
		std::ostringstream ostr;
		ostr << "Error sending nset rq status: "<< status << " details: ";
		DIMSE_printNStatusString(ostr, status);
		throw GIL::DICOM::PACSException(ostr.str(), "GIL::CustomAssociation");
	}
	if (statusDetail) delete statusDetail;
	return cond.good();   
}

bool CustomAssociation::getRQ( const std::string& /*sopclassUID*/, const std::string& /*sopinstanceUID*/, const int* /*attributeIdentifierList*/, unsigned long /*numShorts*/, int& /*status*/,  GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& /*attributeListOut*/)
	/*
	OFCondition CustomAssociation::getRQ(
	const char *sopclassUID, 
	const char *sopinstanceUID, 
	const Uint16 *attributeIdentifierList,
	ssize_t numShorts,
	Uint16& status, 
	DcmDataset* &attributeListOut)
	*/
{
	/*
	if (assoc == NULL)
	{
	return DIMSE_ILLEGALASSOCIATION;
	}
	if ((sopclassUID==NULL)||(sopinstanceUID==NULL))
	{
	return DIMSE_NULLKEY;
	}

	T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
	if (presCtx == 0)
	{
	return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
	}  

	T_DIMSE_Message request;
	T_DIMSE_Message response;
	DcmDataset *statusDetail = NULL;

	// construct N-GET-RQ
	request.CommandField = DIMSE_N_GET_RQ;
	request.msg.NGetRQ.MessageID = assoc->nextMsgID++;
	strcpy(request.msg.NGetRQ.RequestedSOPClassUID, sopclassUID);
	strcpy(request.msg.NGetRQ.RequestedSOPInstanceUID, sopinstanceUID);
	request.msg.NGetRQ.ListCount = 0;
	if (attributeIdentifierList) request.msg.NGetRQ.ListCount = (int)numShorts;
	request.msg.NGetRQ.AttributeIdentifierList = (DIC_US *)attributeIdentifierList;

	OFCondition cond = sendNRequest(presCtx, request, NULL, response, statusDetail, attributeListOut);
	if (cond.good()) status = response.msg.NGetRSP.DimseStatus;
	if (status != 0) {
	LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
	std::ostringstream ostr;
	ostr << "Error sending create rq status: "<< status << " details: ";
	DIMSE_printNStatusString(ostr, status);
	throw GIL::DICOM::PACSException(ostr.str(), "GIL::CustomAssociation");
	}
	if (statusDetail) delete statusDetail;
	return cond;   
	*/
	return false;
}

bool CustomAssociation::actionRQ( const std::string& /*sopclassUID*/, const std::string& /*sopinstanceUID*/, int /*actionTypeID*/, const GIL::DICOM::DicomDataset& /*actionInformation*/, int& /*status*/,  GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& /*attributeListOut*/)
	/*
	OFCondition CustomAssociation::actionRQ(
	const char *sopclassUID, 
	const char *sopinstanceUID, 
	Uint16 actionTypeID, 
	DcmDataset *actionInformation,
	Uint16& status, 
	DcmDataset* &actionReply)
	*/
{
	/*
	if (assoc == NULL)
	{
	return DIMSE_ILLEGALASSOCIATION;
	}
	if ((sopclassUID==NULL)||(sopinstanceUID==NULL))
	{
	return DIMSE_NULLKEY;
	}

	T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
	if (presCtx == 0)
	{
	return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
	}  

	T_DIMSE_Message request;
	T_DIMSE_Message response;
	DcmDataset *statusDetail = NULL;

	// construct N-ACTION-RQ
	request.CommandField = DIMSE_N_ACTION_RQ;
	request.msg.NActionRQ.MessageID = assoc->nextMsgID++;
	strcpy(request.msg.NActionRQ.RequestedSOPClassUID, sopclassUID);
	strcpy(request.msg.NActionRQ.RequestedSOPInstanceUID, sopinstanceUID);
	request.msg.NActionRQ.ActionTypeID = (DIC_US)actionTypeID;
	////////TODO: ESTÁ FALLANDO
	OFCondition cond = sendNRequest(presCtx, request, actionInformation, response, statusDetail, actionReply);
	if (cond.good()) status = response.msg.NActionRSP.DimseStatus;
	if (status != 0) {
	LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
	std::ostringstream ostr;
	ostr << "Error sending create rq status: "<< status << " details: ";
	DIMSE_printNStatusString(ostr, status);
	throw GIL::DICOM::PACSException(ostr.str(), "GIL::CustomAssociation");
	}
	if (statusDetail) delete statusDetail;
	return cond;   
	*/
	return false;
}

bool CustomAssociation::deleteRQ( const std::string& /*sopclassUID*/, const std::string& /*sopinstanceUID*/, int& /*status*/)
	/*
	OFCondition CustomAssociation::deleteRQ(
	const char *sopclassUID, 
	const char *sopinstanceUID, 
	Uint16& status)
	*/
{
	/*
	if (assoc == NULL)
	{
	return DIMSE_ILLEGALASSOCIATION;
	}
	if ((sopclassUID==NULL)||(sopinstanceUID==NULL))
	{
	return DIMSE_NULLKEY;
	}

	T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
	if (presCtx == 0)
	{
	return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
	}  

	T_DIMSE_Message request;
	T_DIMSE_Message response;
	DcmDataset *statusDetail = NULL;
	DcmDataset *attributeListOut = NULL;

	// construct N-DELETE-RQ
	request.CommandField = DIMSE_N_DELETE_RQ;
	request.msg.NDeleteRQ.MessageID = assoc->nextMsgID++;
	strcpy(request.msg.NDeleteRQ.RequestedSOPClassUID, sopclassUID);
	strcpy(request.msg.NDeleteRQ.RequestedSOPInstanceUID, sopinstanceUID);

	OFCondition cond = sendNRequest(presCtx, request, NULL, response, statusDetail, attributeListOut);
	if (cond.good()) status = response.msg.NDeleteRSP.DimseStatus;
	if (status != 0) {
	LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
	std::ostringstream ostr;
	ostr << "Error sending create rq status: "<< status << " details: ";
	DIMSE_printNStatusString(ostr, status);
	throw GIL::DICOM::PACSException(ostr.str(), "GIL::CustomAssociation");
	}
	if (statusDetail) delete statusDetail;
	if (attributeListOut) delete attributeListOut;  // should never happen
	return cond;   
	*/
	return false;
}

bool CustomAssociation::releaseAssociation() 
	/*
	OFCondition CustomAssociation::releaseAssociation()
	*/
{
	/*
	OFCondition result = EC_Normal;
	if (assoc)
	{
	result = ASC_releaseAssociation(assoc);
	Drop();
	Destroy();
	assoc = NULL;
	}
	return result;
	*/
	return false;
}

bool CustomAssociation::abortAssociation()
	/*
	OFCondition CustomAssociation::abortAssociation()
	*/
{
	/*	
	OFCondition result = EC_Normal;
	if (assoc)
	{
	result = ASC_abortAssociation(assoc);
	Drop();
	Destroy();
	assoc =  NULL;
	}
	return result;
	*/
	return false;
}

T_ASC_PresentationContextID CustomAssociation::findAcceptedPC(const char *sopclassuid)
{
	/*
	if ((assoc==NULL)||(sopclassuid==NULL)) return 0;

	// if the SOP class is one of the Basic Grayscale Print Management Meta SOP Classes,
	// look for a presentation context for Basic Grayscale Print.
	OFString sopclass(sopclassuid);
	if ((sopclass == UID_BasicFilmSessionSOPClass) ||
	(sopclass == UID_BasicFilmBoxSOPClass) ||
	(sopclass == UID_BasicGrayscaleImageBoxSOPClass) ||
	(sopclass == UID_PrinterSOPClass)) sopclassuid = UID_BasicGrayscalePrintManagementMetaSOPClass;
	*/
	OFString sopclass(sopclassuid);
	return ASC_findAcceptedPresentationContextID(assoc, sopclassuid);
}


void CustomAssociation::Gnk2Dcmtk(const GIL::DICOM::DicomDataset& /*base*/, DcmDataset* /*query*/)
{
	/*
	OFCondition cond;
	typedef GIL::DICOM::DicomDataset TJerarquia;
	typedef GIL::DICOM::ListaTags TListaTags;

	for (TListaTags::const_iterator it = base.tags.begin(); it != base.tags.end(); ++it) {
	//DcmElement* e = GIL::DICOM::DICOMManager::CrearElementoConValor((*it).first.c_str());

	if (e != NULL) {
	const std::string& val = (*it).second;
	if (val.size() > 0) {
	e->putString( (*it).second.c_str() );
	}					

	cond = query->insert(e, true, false);

	if (cond.bad()) {
	LOG_ERROR("PACSCONTROLLER", "No se pudo insertar el elemento: (" << e->getTag().toString().c_str() << "): " << cond.text());
	}
	}

	for (TJerarquia::DatasetList::const_iterator it2 = base.secuencias.begin(); it2 != base.secuencias.end(); it2++) {
	const TJerarquia& seq = (*it2);
	//DcmElement* es = GIL::DICOM::DICOMManager::CrearElementoConValor(seq.tagName.c_str());			

	for (TJerarquia::DatasetList::const_iterator it3 = seq.items.begin(); it3 != seq.items.end(); it3++) {
	const TJerarquia& item = (*it3);
	DcmItem* di = new DcmItem();

	for (TListaTags::const_iterator it4 = item.tags.begin(); it4 != item.tags.end(); it4++) {
	//DcmElement* ei = GIL::DICOM::DICOMManager::CrearElementoConValor((*it4).first.c_str());
	if (ei != NULL) {
	const std::string& val = (*it4).second;
	if (val.size() > 0) {
	ei->putString( (*it4).second.c_str() );
	}					
	cond = di->insert(ei, true, false);

	if (cond.bad()) {
	LOG_ERROR("PACSCONTROLLER", "No se pudo insertar el elemento: (" << ei->getTag().toString().c_str() << "): " << cond.text());
	}
	}

	}

	query->insertSequenceItem(es->getTag(), di);
	}					
	}
	}
	*/
}




