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

#define LOGGER "C-GET"
#include "dicomnetwork.h"
#include "dicomgetassociation.h"

#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpeg/djrplol.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <api/controllers/icontroladorlog.h>
#include <main/controllers/controladorlog.h>
#include <api/dicom/imodelodicom.h>
#include <main/controllers/controladorpermisos.h>
#include <api/controllers/ipacscontroller.h>
#include <main/entorno.h>

#include "istorecallback.h"
#include <wx/intl.h>



GetAssociation::GetAssociation(const std::string& _ambitolog, IModeloDicom* pModelo) :
        FindAssociation(_ambitolog),
	m_maxReceivePDULength(ASC_DEFAULTMAXPDU),
	m_pModelo(pModelo),
        m_wellKnownNumResults(0), 
	m_numeroImagenes(0),
	m_mensaje(""),
	m_errorMessage(""),
	m_bytesDescargados(0)
{
	m_abstractSyntax.assign(UID_GETStudyRootQueryRetrieveInformationModel);
}

GetAssociation::~GetAssociation() { }

void GetAssociation::Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, /*int ourPort,*/ const char *abstractSyntax) {
	Association::Create(title, peer, port, ouraet, abstractSyntax);
}

void GetAssociation::SetWellKnownNumResults(unsigned int numResults){
	m_wellKnownNumResults = numResults;
}

void GetAssociation::SetModelo(IModeloDicom* pModelo) {
	m_pModelo = pModelo;
}

OFCondition GetAssociation::SendObject(DcmDataset *dataset) {
	return getSCU(dataset);
}

void GetAssociation::SetStorageSOPClasses(const GIL::DICOM::SOPClassList& SOPClasses)
{
	this->SOPClasses = SOPClasses;
}

void GetAssociation::OnAddPresentationContext(T_ASC_Parameters * params) {

	unsigned int pid = 1;

	OFCondition cond = ASC_addPresentationContext(params, pid, m_abstractSyntax.c_str(), AllTransferSyntaxes, 3);
	if (cond.bad()) {
		LOG_ERROR(ambitolog, "Unable to add GETStudyRootQueryRetrieveInformationModel presentation context");
		return;
	}

	pid += 2;

	if (SOPClasses.size() > 0) {
		/*for (unsigned int i = 0; pid <= 255 && i < (unsigned int) numberOfDcmLongSCUStorageSOPClassUIDs && cond.good(); ++i) {
			if ( SOPClasses.find(dcmLongSCUStorageSOPClassUIDs[i]) != SOPClasses.end() ) {

				for (unsigned int j = 0; pid <= 255 && j < AllTransferSyntaxesCount && cond.good(); j++) {
					cond = ASC_addPresentationContext(params, pid, dcmLongSCUStorageSOPClassUIDs[i], &AllTransferSyntaxes[j], 1);
					pid += 2;
				}
			}
		}*/
		const GIL::DICOM::TransferSyntaxMap& tsMap = GIL::DICOM::Conformance::GetTransferSyntaxes();
		
		for (GIL::DICOM::SOPClassList::const_iterator itSopClass = SOPClasses.begin(); pid <= 255 && itSopClass != SOPClasses.end() && cond.good(); ++itSopClass) {
			for (GIL::DICOM::TransferSyntaxMap::const_iterator itTS = tsMap.begin(); pid <= 255 && itTS != tsMap.end() && cond.good(); ++itTS) {
				const char* ts[] = {(*itTS).second.c_str()};
				cond = ASC_addPresentationContext(params, pid, (*itSopClass).c_str(), ts, 1);
				pid += 2;
			}

		}

		if (pid >= 255) {
			LOG_WARN("C-GET", "Too many PresentationContexts setted");
		}
	}
	else {
		for (unsigned int i = 0; pid <= 255 && i < (unsigned int) numberOfDcmShortSCUStorageSOPClassUIDs && cond.good(); ++i) {
			cond = ASC_addPresentationContext(params, pid, dcmShortSCUStorageSOPClassUIDs[i], AllTransferSyntaxes, AllTransferSyntaxesCount);
			pid += 2;			
		}
		if (pid >= 255) {
			LOG_WARN("C-GET", "Too many PresentationContexts setted");
		}
	}
}

static int
selectReadable(T_ASC_Association *assoc,
					T_ASC_Network *net, T_ASC_Association *subAssoc,
					T_DIMSE_BlockingMode blockMode, int timeout)
{
	T_ASC_Association *assocList[2];
	int assocCount = 0;

	if (net != NULL && subAssoc == NULL) {
		if (ASC_associationWaiting(net, 0)) {
			/* association request waiting on network */
			return 2;
		}
	}
	assocList[0] = assoc;
	assocCount = 1;
	assocList[1] = subAssoc;
	if (subAssoc != NULL) assocCount++;
	if (subAssoc == NULL) {
		timeout = 1;    /* poll wait until an assoc req or move rsp */
	} else {
		if (blockMode == DIMSE_BLOCKING) {
			timeout = 10000;    /* a long time */
		}
	}
	if (!ASC_selectReadableAssociation(assocList, assocCount, timeout)) {
		/* none readable */
		return 0;
	}
	if (assocList[0] != NULL) {
		/* main association readable */
		return 1;
	}
	if (assocList[1] != NULL) {
		/* sub association readable */
		return 2;
	}
	/* should not be reached */
	return 0;
}


OFCondition MDIMSE_getUser(
									T_ASC_Association* assoc,
									GetAssociation* getassoc,
									T_ASC_PresentationContextID presID,
									T_DIMSE_C_GetRQ *request,
									DcmDataset *requestIdentifiers,
									T_DIMSE_BlockingMode blockMode, int timeout,
									/* out */
									T_DIMSE_C_GetRSP * /*response*/, DcmDataset **statusDetail,
									DcmDataset **rspIds)
{
	T_DIMSE_Message req, rsp;
	DIC_US msgId;
	int responseCount = 0;
	T_ASC_Association *subAssoc = NULL;
	DIC_US status = STATUS_Pending;

	if (requestIdentifiers == NULL) return DIMSE_NULLKEY;

	bzero((char*)&req, sizeof(req));
	bzero((char*)&rsp, sizeof(rsp));

	req.CommandField = DIMSE_C_GET_RQ;
	request->DataSetType = DIMSE_DATASET_PRESENT;
	req.msg.CGetRQ = *request;

	msgId = request->MessageID;

	LOG_DEBUG("C-GET", "Accepted presID " << (int)presID);

	OFCondition cond = DIMSE_sendMessageUsingMemoryData(assoc, presID, &req,
		NULL, requestIdentifiers,
		NULL, NULL);
	if (cond != EC_Normal) {
		return cond;
	}

	/* receive responses */

	while (cond == EC_Normal && status == STATUS_Pending) {

		/* if user wants, multiplex between net/subAssoc
		* and responses over main assoc.
		*/
		switch (selectReadable(assoc, getassoc->GetNetwork()->GetDcmtkNet(), subAssoc, blockMode, timeout)) {
		  case 0:
			  /* none are readble, timeout */
			  if (blockMode == DIMSE_BLOCKING) {
				  continue;       /* continue with while loop */
			  } else {
				  return DIMSE_NODATAAVAILABLE;
			  }
			  /* break; */ // never reached after continue or return.
		  case 1:
			  /* main association readable */
			  break;
		  case 2:
			  /* net/subAssoc readable */
			  getassoc->subOpCallback(NULL, getassoc->GetNetwork()->GetDcmtkNet(), &subAssoc);
			  continue;   /* continue with main loop */
			  /* break; */ // never reached after continue statement
		}

		bzero((char*)&rsp, sizeof(rsp));

		cond = DIMSE_receiveCommand(assoc, blockMode, timeout, &presID,
			&rsp, statusDetail);
		if (cond != EC_Normal) {
			return cond;
		}

		{
			OFString str;
			DIMSE_dumpMessage(str, rsp, DIMSE_INCOMING);
			LOG_DEBUG("C-GET", "Command received" << std::endl << str.c_str());
		}

		switch (rsp.CommandField){
		case DIMSE_C_STORE_RQ:
			{
				if (rsp.msg.CStoreRQ.DataSetType != DIMSE_DATASET_NULL) {
					cond = getassoc->storeSCP(assoc, &rsp, presID);
					if (cond != EC_Normal) {
						return cond;
					}
					else {
						;
					}
				}
			}
			break;
		case DIMSE_C_GET_RSP:
			{
				if (rsp.msg.CGetRSP.MessageIDBeingRespondedTo != msgId)
				{
					std::stringstream ss;
					ss << "DIMSE: Unexpected Response MsgId: " << rsp.msg.CGetRSP.MessageIDBeingRespondedTo << " (expected: " << msgId << ")";
					return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, ss.str().c_str());
				}

				status = rsp.msg.CGetRSP.DimseStatus;
				responseCount++;

				switch (status) {
		case STATUS_Pending:
			if (*statusDetail != NULL) {
				DCMNET_WARN(DIMSE_warn_str(assoc) << "getUser: Pending with statusDetail, ignoring detail");
				delete *statusDetail;
				*statusDetail = NULL;
			}
			if (rsp.msg.CGetRSP.DataSetType != DIMSE_DATASET_NULL) {
				DCMNET_WARN(DIMSE_warn_str(assoc) << "getUser: Status Pending, but DataSetType!=NULL");
				DCMNET_WARN(DIMSE_warn_str(assoc) << "Assuming NO response identifiers are present");
			}

			break;

		default:

			if (rsp.msg.CGetRSP.DataSetType != DIMSE_DATASET_NULL) {
				cond = DIMSE_receiveDataSetInMemory(assoc, blockMode, timeout,
					&presID, rspIds, NULL, NULL);
				if (cond != EC_Normal) {
					return cond;
				}
			}
			break;
				}
			}
			break;
		default:
			{
				std::stringstream ss;
				ss << "DIMSE: Unexpected Response Command Field: " << (unsigned)rsp.CommandField;
				return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, ss.str().c_str());
			}
			break;
		}
	}


	/* do remaining sub-association work, we may receive a non-pending
	* status before the sub-association has cleaned up.
	*/
	while (subAssoc != NULL) {
		getassoc->subOpCallback(getassoc, getassoc->GetNetwork()->GetDcmtkNet(), &subAssoc);
	}


	return cond;
}


OFCondition GetAssociation::getSCU(DcmDataset *pdset) {
	OFCondition cond;
	T_ASC_PresentationContextID presId;
	T_DIMSE_C_GetRQ req;
	T_DIMSE_C_GetRSP rsp;
	DIC_US msgId = assoc->nextMsgID++;
	DcmDataset* rspIds = NULL;
	const char* sopClass;
	DcmDataset* statusDetail = NULL;

	if (pdset == NULL) {
		LOG_ERROR(ambitolog, "Dataset nulo en getSCU");
		return DIMSE_NULLKEY;
	}

	{
		OFString str;
		ASC_dumpParameters(str, assoc->params, ASC_ASSOC_AC);
		LOG_DEBUG("C-GET", str.c_str());

	}

	//sopClass = m_abstractSyntaxMove;
	sopClass = m_abstractSyntax.c_str();

	// which presentation context should be used
	presId = ASC_findAcceptedPresentationContextID(assoc, sopClass);

	if (presId == 0) {
		return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
	}

	req.MessageID = msgId;
	strcpy(req.AffectedSOPClassUID, sopClass);
	req.Priority = DIMSE_PRIORITY_HIGH;
	req.DataSetType = DIMSE_DATASET_PRESENT;

	ResetearMedida(true);
	m_numeroImagenes=0;

	{
		OFString str;
		DIMSE_dumpMessage(str, req, DIMSE_OUTGOING, pdset, presId);
		LOG_DEBUG("C-GET", str.c_str());
	}

	cond = MDIMSE_getUser(
		assoc,
		this,
		presId,
		&req,
		pdset,
		DIMSE_BLOCKING,
		0,
		&rsp, &statusDetail, &rspIds);

	if (cond.bad()) {
		std::ostringstream os;
		os << DU_cgetStatusString(cond.status());
		if (statusDetail != NULL) {
			OFString errorComment;
			if (statusDetail->findAndGetOFString(DCM_ErrorComment, errorComment).good()) {
				os << ". " << errorComment.c_str();
			}
		}
		m_errorMessage = os.str();
	}

	if (statusDetail != NULL) {
		LOG_DEBUG(ambitolog, "DIMSE_getUser(): Estado: " << std::endl << DumpDataset(statusDetail));
		delete statusDetail;
	}

	if (rspIds != NULL) {

		delete rspIds;
	}

	return cond;

	/*if( rsp.DimseStatus != STATUS_Success && rsp.DimseStatus != STATUS_Pending) {
		std::ostringstream os;
		os << DU_cgetStatusString(rsp.DimseStatus);
		if (statusDetail != NULL) {
			OFString errorComment;
			if (statusDetail->findAndGetOFString(DCM_ErrorComment, errorComment).good()) {
				os << ". " << errorComment.c_str();
			}
		}
		m_errorMessage = os.str();
	}

	if (statusDetail != NULL) {
		LOG_DEBUG(ambitolog, "DIMSE_getUser(): Estado: " << std::endl << DumpDataset(statusDetail));
		delete statusDetail;
	}

	if (rspIds != NULL) {

		delete rspIds;
	}

	if(m_errorMessage != "" || rsp.NumberOfFailedSubOperations > 0 ) {
		if (m_errorMessage.size() == 0) {
			m_errorMessage = "Some operations failed";
		}
		return makeOFCondition(OFM_dcmnet, 18, OF_error, m_errorMessage.c_str());
	}

	return cond;*/
}

void GetAssociation::getCallback(void * /*callbackData*/, T_DIMSE_C_GetRQ * /*request*/, int /*responseCount*/, T_DIMSE_C_GetRSP * /*response*/)
{

}

void GetAssociation::subOpCallback(void *pCaller, T_ASC_Network *aNet, T_ASC_Association **subAssoc) {
	GetAssociation* caller = static_cast<GetAssociation*>(pCaller);

	if (caller->GetNetwork() == NULL) {
		return;
	}
	
	wxString msg = wxString::Format(_("Downloading file %d"), (int)(++caller->m_numeroImagenes));
	caller->m_mensaje = std::string(msg.ToUTF8());
	LOG_DEBUG(caller->ambitolog, caller->m_mensaje);
	caller->NotificarProgreso((float)caller->m_numeroImagenes/100,caller->m_mensaje);

	if (*subAssoc == NULL) {
		// negotiate association
		LOG_DEBUG(caller->ambitolog, "Aceptando subasociacion");
		caller->acceptSubAssoc(aNet, subAssoc);
	}
	else {
		// be a service class provider
		LOG_DEBUG(caller->ambitolog, "Invocando subOp SCP");
		caller->subOpSCP(subAssoc);
	}
}

OFCondition GetAssociation::acceptSubAssoc(T_ASC_Network* /*aNet*/, T_ASC_Association** /*assoc*/) {
	OFCondition cond = EC_Normal;

	return cond;

}

OFCondition GetAssociation::subOpSCP(T_ASC_Association **subAssoc) {
	T_DIMSE_Message msg;
	T_ASC_PresentationContextID presID;

	/* just in case */
	if (!ASC_dataWaiting(*subAssoc, 0)) {
		LOG_TRACE(ambitolog, "No hay datos pendientes");
		return DIMSE_NODATAAVAILABLE;
	}

	OFCondition cond = DIMSE_receiveCommand(*subAssoc, DIMSE_BLOCKING, 0, &presID, &msg, NULL);

	if (cond == EC_Normal) {
		switch (msg.CommandField) {
			case DIMSE_C_STORE_RQ:
				LOG_TRACE(ambitolog, "Invocando C-STORE_RQ");
				cond = storeSCP(*subAssoc, &msg, presID);
				break;
			case DIMSE_C_ECHO_RQ:
				LOG_TRACE(ambitolog, "Invocando C-ECHO_RQ");
				cond = echoSCP(*subAssoc, &msg, presID);
				break;
			default:
				LOG_ERROR(ambitolog, "Tipo de comando incorrecto. Sólo se aceptan C-STORE_RQ o C-ECHO_RQ en esta etapa" << cond.text());
				cond = DIMSE_BADCOMMANDTYPE;
				break;
		}
	}

	// clean up on association termination
	if (cond == DUL_PEERREQUESTEDRELEASE) {
		cond = ASC_acknowledgeRelease(*subAssoc);
		ASC_dropSCPAssociation(*subAssoc);
		ASC_destroyAssociation(subAssoc);
		return cond;
	}
	else if (cond == DUL_PEERABORTEDASSOCIATION) {
		LOG_ERROR(ambitolog, "El PACS remoto aborto la asociacion"  << cond.text());
	}
	else if (cond != EC_Normal) {
		LOG_ERROR(ambitolog, "Ha ocurrido un error y se abortara la asociacion"  << cond.text());
		// some kind of error so abort the association
		cond = ASC_abortAssociation(*subAssoc);
	}

	if (cond != EC_Normal) {
		ASC_dropAssociation(*subAssoc);
		ASC_destroyAssociation(subAssoc);
	}
	return cond;
}

OFCondition GetAssociation::storeSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID) {
	OFCondition cond;
	T_DIMSE_C_StoreRQ* req;

	req = &msg->msg.CStoreRQ;

	StoreCallbackInfo callbackData;
	callbackData.pCaller = this;
	callbackData.assoc = assoc;
	callbackData.lastTick = std::clock();

	std::string fileName = GNC::Entorno::Instance()->CreateGinkgoTempFile();

	cond = DIMSE_storeProvider(assoc, presID, req, fileName.c_str(), 1,
		NULL, storeSCPCallback, (void*) & callbackData,
		DIMSE_BLOCKING, 0);

	if (cond.good()) {
		//we are going to read received file dataset
		DcmFileFormat ff;
		ff.loadFile(fileName.c_str(), EXS_Unknown, EGL_noChange, DCM_TagInfoLength);
		DcmDataset* imageDataSet = ff.getDataset();

		if ((imageDataSet)) {
			// do not duplicate the dataset, let the user do this
			// if he wants to
			OnFileReceived(fileName, imageDataSet);
		}
	}

	return cond;
}

void GetAssociation::storeSCPCallback(void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ *req, char* imageFileName, DcmDataset ** /*imageDataSet*/, T_DIMSE_C_StoreRSP *rsp, DcmDataset **statusDetail) {
	DIC_UI sopClass;
	DIC_UI sopInstance;

	StoreCallbackInfo *cbdata = (StoreCallbackInfo*) callbackData;
	GetAssociation* caller = cbdata->pCaller;

	float partial = 0.0f;
	float step = 0.0f;

	if (caller->m_wellKnownNumResults > 0 ) {		
		step = 1.0f / (float) caller->m_wellKnownNumResults;
		partial = (float)caller->m_numeroImagenes * step;
	}
	else {
		;
	}

	if (progress->state == DIMSE_StoreBegin) {
		wxString msg;
		if (caller->m_wellKnownNumResults > 0) {
			msg = wxString::Format(_("Downloading file %d/%d"), (int)(caller->m_numeroImagenes + 1), (int)(caller->m_wellKnownNumResults) );
		}
		else {
			msg = wxString::Format(_("Downloading file %d"), (int)(caller->m_numeroImagenes + 1) );
		}
		caller->m_mensaje = std::string(msg.ToUTF8());
		LOG_DEBUG(caller->ambitolog, caller->m_mensaje);

		if (!caller->NotificarProgreso((float) partial, caller->m_mensaje) ) {
			rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
			caller->Stop();
			ASC_releaseAssociation(caller->assoc);
			LOG_INFO(caller->ambitolog, "Operation canceled by user");
			return;
		}

		caller->m_bytesDescargados += progress->totalBytes;
		GNC::GCS::Permisos::EstadoPermiso estado = GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits", "study_size");
		if (estado) {
			if (estado.ObtenerValor<long>() < (caller->m_bytesDescargados / 1024) ) {
				caller->m_errorMessage = _Std("Study download size limit exceeded");
				rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
				caller->Stop();
				ASC_releaseAssociation(caller->assoc);
				return;
			}
		}
		cbdata->lastTick = std::clock();
	}
	else if (progress->state == DIMSE_StoreProgressing) {
		std::clock_t curTick = std::clock();
		if ( (curTick - cbdata->lastTick) > (CLOCKS_PER_SEC >> 1) ) {
			cbdata->lastTick = curTick;
			std::stringstream ostr;
			ostr << caller->m_mensaje;
			ostr.setf(std::ios::floatfield, std::ios::fixed );
			ostr.precision(2);
			ostr <<  " (" << caller->TasaTransferencia(progress->progressBytes) << " kb/s)";
			std::string msg(ostr.str());
			if(!caller->NotificarProgreso( partial + step * ((float)progress->progressBytes/(float)progress->totalBytes), msg) )
			{
				rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
				OFCondition cond = ASC_releaseAssociation(cbdata->assoc);
				return;
			}
		}
	}
	else if (progress->state == DIMSE_StoreEnd) {
		LOG_TRACE(caller->ambitolog, "storeSCPCallback(). DIMSE_StoreEnd");
		caller->m_numeroImagenes++;
		*statusDetail = NULL; /* no status detail */
		caller->ResetearMedida();

		/* could save the image somewhere else, put it in database, etc */
		rsp->DimseStatus = STATUS_Success;

		//we are going to read received file dataset
		DcmFileFormat ff;
		ff.loadFile(imageFileName, EXS_Unknown, EGL_noChange, DCM_TagInfoLength);
		DcmDataset* imageDataSet = ff.getDataset();
		/* should really check the image to make sure it is consistent,
		* that its sopClass and sopInstance correspond with those in
		* the request.
		*/
		if (rsp->DimseStatus == STATUS_Success) {
			/* which SOP class and SOP instance ? */
			if (!DU_findSOPClassAndInstanceInDataSet(imageDataSet, sopClass, sopInstance)) {
				rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
				LOG_ERROR(caller->ambitolog, "No se pudo encontrar SOPClass o SOPInstanceUID en el dataset");
			}
			else if (strcmp(sopClass, req->AffectedSOPClassUID) != 0) {
				rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
				LOG_ERROR(caller->ambitolog, "El SOPClass del dataset(" << sopClass << ") no coincide con el SOPClass requerido (" << req->AffectedSOPClassUID << ")");
			}
			else if (strcmp(sopInstance, req->AffectedSOPInstanceUID) != 0) {
				rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
				LOG_ERROR(caller->ambitolog, "El SOPInstance del dataset(" << sopInstance << ") no coincide con el SOPInstanceUID requerido (" << req->AffectedSOPInstanceUID << ")");
			}
		}
	}
}

OFCondition GetAssociation::echoSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID) {
	OFCondition cond;

	// the echo succeeded !!
	cond = DIMSE_sendEchoResponse(assoc, presID, &msg->msg.CEchoRQ, STATUS_Success, NULL);

	return cond;
}

void GetAssociation::OnFileReceived(const std::string& fileName, DcmDataset* dset) {
	if (m_pModelo != NULL) {
		OFString OFEstudioUId;
		if (dset->findAndGetOFString(DCM_StudyInstanceUID, OFEstudioUId).good()) {
			OFString OFPacienteUID;
			std::string PacienteUID;
			if (dset->findAndGetOFString(DCM_PatientID, OFPacienteUID).good()) {
				PacienteUID.assign(OFPacienteUID.c_str());
			}

			OFString OFPacienteNombre;
			std::string PacienteNombre;
			if (dset->findAndGetOFString(DCM_PatientName, OFPacienteNombre).good()) {
				PacienteNombre.assign(OFPacienteNombre.c_str());
			}

			OFString OFPacienteFechaNacimiento;
			std::string PacienteFechaNacimiento;

			if (dset->findAndGetOFString(DCM_PatientBirthDate, OFPacienteFechaNacimiento).good()) {
				PacienteFechaNacimiento.assign(OFPacienteFechaNacimiento.c_str());
			}


			OFString OFPacienteSexo;
			std::string PacienteSexo;
			if (dset->findAndGetOFString(DCM_PatientSex, OFPacienteSexo).good()) {
				PacienteSexo.assign(OFPacienteSexo.c_str());
			}

			const IModeloPaciente& paciente = m_pModelo->InsertarPaciente(PacienteUID, PacienteNombre, PacienteFechaNacimiento, PacienteSexo);

			OFString OFEstudioUID;
			std::string EstudioUID;
			if (dset->findAndGetOFString(DCM_StudyInstanceUID, OFEstudioUID).good()) {
				EstudioUID.assign(OFEstudioUID.c_str());
			}

			OFString OFEAccNumber;
			std::string AccNumber;
			if (dset->findAndGetOFString(DCM_AccessionNumber, OFEAccNumber).good()) {
				AccNumber.assign(OFEAccNumber.c_str());
			}

			OFString OFEstudioDescripcion;
			std::string EstudioDescripcion;
			if (dset->findAndGetOFString(DCM_StudyDescription, OFEstudioDescripcion).good()) {
				EstudioDescripcion.assign(OFEstudioDescripcion.c_str());
			}

			OFString OFEstudioModalidad;
			std::string EstudioModalidad;
			if (dset->findAndGetOFString(DCM_ModalitiesInStudy, OFEstudioModalidad).good()) {
				EstudioModalidad.assign(OFEstudioModalidad.c_str());
			}

			OFString OFEstudioFecha;
			std::string EstudioFecha;
			if (dset->findAndGetOFString(DCM_StudyDate, OFEstudioFecha).good()) {
				EstudioFecha.assign(OFEstudioFecha.c_str());
			}

			OFString OFEstudioHora;
			std::string EstudioHora;
			if (dset->findAndGetOFString(DCM_StudyTime, OFEstudioHora).good()) {
				EstudioHora.assign(OFEstudioHora.c_str());
			}


			OFString OFEstudioDoctor;
			std::string EstudioDoctor;
			if (dset->findAndGetOFString(DCM_ReferringPhysicianName, OFEstudioDoctor).good()) {
				EstudioDoctor.assign(OFEstudioDoctor.c_str());
			}

			m_pModelo->InsertarEstudio(paciente.GetUID(), EstudioUID, AccNumber, EstudioDescripcion, EstudioModalidad, EstudioFecha, EstudioHora, EstudioDoctor);

			OFString OFSerieUID;
			std::string SerieUID;
			if (dset->findAndGetOFString(DCM_SeriesInstanceUID, OFSerieUID).good()) {
				SerieUID.assign(OFSerieUID.c_str());
			}

			OFString OFSerieTipo;
			std::string SerieTipo;
			if (dset->findAndGetOFString(DCM_SeriesType, OFSerieTipo).good()) {
				SerieTipo.assign(OFSerieTipo.c_str());
			}

			OFString OFSerieFecha;
			std::string SerieFecha;
			if (dset->findAndGetOFString(DCM_SeriesDate, OFSerieFecha).good()) {
				SerieFecha.assign(OFSerieFecha.c_str());
			}

			OFString OFSerieHora;
			std::string SerieHora;
			if (dset->findAndGetOFString(DCM_SeriesTime, OFSerieHora).good()) {
				SerieHora.assign(OFSerieHora.c_str());
			}

			OFString OFSerieDescripcion;
			std::string SerieDescripcion;
			if (dset->findAndGetOFString(DCM_SeriesDescription, OFSerieDescripcion).good()) {
				SerieDescripcion.assign(OFSerieDescripcion.c_str());
			}

			OFString OFSerieNumero;
			std::string SerieNumero;
			if (dset->findAndGetOFString(DCM_NumberOfSeriesRelatedInstances, OFSerieNumero).good()) {
				SerieNumero.assign(OFSerieNumero.c_str());
			}

			OFString OFSerieDoctor;
			std::string SerieDoctor;
			if (dset->findAndGetOFString(DCM_ReferringPhysicianName, OFSerieDoctor).good()) {
				SerieDoctor.assign(OFSerieDoctor.c_str());
			}

			m_pModelo->InsertarSerie(EstudioUID, SerieUID, SerieTipo, SerieFecha, SerieHora, SerieDescripcion,SerieNumero,SerieDoctor);

			OFString OFUIDImagen;
			std::string UIDImagen;
			OFString OFInstanceNumber;
			std::string InstanceNumber;
			if (dset->findAndGetOFString(DCM_SOPInstanceUID, OFUIDImagen).good()) {
				UIDImagen.assign(OFUIDImagen.c_str());
				 if (dset->findAndGetOFString(DCM_InstanceNumber, OFInstanceNumber).good()) {
					InstanceNumber.assign(OFInstanceNumber.c_str());
				 }
				 else {
					 LOG_WARN(ambitolog, "InstanceNumber not found. Setting to default");
					 InstanceNumber.assign("1");
				 }
				m_pModelo->InsertarImagen(SerieUID,UIDImagen, InstanceNumber, fileName);
			} else {
				LOG_ERROR("DCMGET", "InstanceUID not found");
			}
		}
	}
}


float GetAssociation::TasaTransferencia(int bytesDescargados)
{
	time_t nuevoInstante = time(NULL);
	const double tiempo = difftime(nuevoInstante,m_medida.m_instante);
	if(tiempo>0.5){
		if(bytesDescargados > m_medida.bytesDescargados) {
			const double bytesRecibidosPeriodo = bytesDescargados - m_medida.bytesDescargados;
			m_medida.oldTasa = ( ((float)bytesRecibidosPeriodo/1024.0f)/tiempo );
			m_medida.bytesDescargados = bytesDescargados;
			m_medida.m_instante = nuevoInstante;
		}
	}
	return m_medida.oldTasa;
}

void GetAssociation::ResetearMedida(bool clearTasa) {
	m_medida.bytesDescargados = 0;
	m_medida.m_instante = time(NULL);
	if(clearTasa) {
		m_medida.oldTasa=0.0f;
	}
}

