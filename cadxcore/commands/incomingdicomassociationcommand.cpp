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


// For some reason dcmtk-3.6.1-20150924 has an hiccup in
// dcmth/oflog/  logger.h and logmacro.h 
// since they are not used we define the include guards here
#define DCMTK_LOG4CPLUS_TRACELOGGER_H
#define DCMTK_LOG4CPLUS_LOGGING_MACROS_HEADER_

#include <wx/string.h>

#include <api/imodelointegracion.h>
#include <api/internationalization/internationalization.h>
#include <commands/incomingdicomassociationcommand.h>
#include <commands/comandoincluirhistorial.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/commandcontroller.h>
#include <main/entorno.h>
#include <eventos/mensajes.h>


#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

#include <dcmtk/dcmnet/cond.h>

#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmnet/dimse.h>

#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>

#include <dcmtk/dcmdata/dcdeftag.h>
#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

#include <main/controllers/pacscontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladoreventos.h>
struct StoreCallbackData
{
    //std::string imageFileName;
	DcmFileFormat* dcmff;
	T_ASC_Association* assoc;
};

/**
* This function.is used to indicate progress when storescp receives instance data over the
* network. On the final call to this function (identified by progress->state == DIMSE_StoreEnd)
* this function will store the data set which was received over the network to a file.
* Earlier calls to this function will simply cause some information to be dumped to stdout.
*
* Parameters:
*   callbackData  - [in] data for this callback function
*   progress      - [in] The state of progress. (identifies if this is the initial or final call
*                   to this function, or a call in between these two calls.
*   req           - [in] The original store request message.
*   imageFileName - [in] The path to and name of the file the information shall be written to.
*   imageDataSet  - [in] The data set which shall be stored in the image file
*   rsp           - [inout] the C-STORE-RSP message (will be sent after the call to this function)
*   statusDetail  - [inout] This variable can be used to capture detailed information with regard to
*                   the status information which is captured in the status element (0000,0900). Note
*                   that this function does specify any such information, the pointer will be set to NULL.
*/
static void storeSCPCallback( void * callbackData, T_DIMSE_StoreProgress * progress, T_DIMSE_C_StoreRQ * /*req*/, char * /*imageFileName*/, DcmDataset ** imageDataSet, T_DIMSE_C_StoreRSP * rsp, DcmDataset ** statusDetail)
{
	//DIC_UI sopClass;
	//DIC_UI sopInstance;

	std::string ambitoLog = "storeSCPCallback";

	// if this is the final call of this function, save the data which was received to a file
	// (note that we could also save the image somewhere else, put it in database, etc.)
	if (progress->state == DIMSE_StoreEnd)
	{
		OFString tmpStr;

		// do not send status detail information
		*statusDetail = NULL;

		// remember callback data
		StoreCallbackData *cbdata = OFstatic_cast(StoreCallbackData *, callbackData);

		// Concerning the following line: an appropriate status code is already set in the resp structure,
		// it need not be success. For example, if the caller has already detected an out of resources problem
		// then the status will reflect this.  The callback function is still called to allow cleanup.
		//rsp->DimseStatus = STATUS_Success;

		// we want to write the received information to a file only if this information
		// is present and the options opt_bitPreserving and opt_ignore are not set.
		if ((imageDataSet != NULL) && (*imageDataSet != NULL))
		{
			OFString fileName;

			OFString OFPacienteUId;
			OFString OFEstudioUId;
			OFString OFSerieUId;
			OFString OFImagenUId;

			std::string PacienteUId;
			std::string EstudioUId;
			std::string SerieUId;
			std::string ImagenUId;

			if ((*imageDataSet)->findAndGetOFString(DCM_PatientID, OFPacienteUId).good()) {
				PacienteUId.assign(OFPacienteUId.c_str());
			} else {
				LOG_ERROR(ambitoLog, _Std("element patient id absent in dataset" ));
				rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
				return;
			}
			if ((*imageDataSet)->findAndGetOFString(DCM_StudyInstanceUID, OFEstudioUId).good()) {
				EstudioUId.assign(OFEstudioUId.c_str());
			} else {
				LOG_ERROR(ambitoLog, _Std("element study instance uid absent in dataset" ));
				rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
				return;
			}
			if ((*imageDataSet)->findAndGetOFString(DCM_SeriesInstanceUID, OFSerieUId).good()) {
				SerieUId.assign(OFSerieUId.c_str());
			} else {
				LOG_ERROR(ambitoLog, _Std("element series instance uid absent in dataset" ));
				rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
				return;
			}

			if ((*imageDataSet)->findAndGetOFString(DCM_SOPInstanceUID, OFImagenUId).good()) {
				ImagenUId.assign(OFImagenUId.c_str());
			} else {
				LOG_ERROR(ambitoLog, _Std("element sop instance uid absent in dataset" ));
				rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
				return;
			}

			std::string rutaStd;
			if (!GIL::DICOM::PACSController::Instance()->GetRutaImagenTemp(PacienteUId,EstudioUId,SerieUId,ImagenUId, rutaStd)) {
				LOG_ERROR(ambitoLog, "could not create subdirectory for study: " << rutaStd);
				rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
				return;
			}
			fileName = rutaStd.c_str();

			// determine the transfer syntax which shall be used to write the information to the file
			E_TransferSyntax xfer = (*imageDataSet)->getOriginalXfer();

			// store file either with meta header or as pure dataset
			LOG_DEBUG(ambitoLog, "storing DICOM file: " << fileName);
			/*   if (OFStandard::fileExists(fileName))
			{
			LOG_WARN(ambitoLog, "DICOM file already exists, overwriting: " << fileName);
			}*/
			OFCondition cond = cbdata->dcmff->saveFile(fileName.c_str(), xfer);
			if (cond.bad())
			{
				LOG_ERROR(ambitoLog, "cannot write DICOM file: " << fileName << ": " << cond.text());
				rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
			}

			// check the image to make sure it is consistent, i.e. that its sopClass and sopInstance correspond
			// to those mentioned in the request. If not, set the status in the response message variable.
			if (rsp->DimseStatus == STATUS_Success)
			{
				// which SOP class and SOP instance ?
				/* if (!DU_findSOPClassAndInstanceInDataSet(*imageDataSet, sopClass, sopInstance))
				{
				LOG_ERROR(ambitoLog, "bad DICOM file: " << fileName);
				rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
				}
				else if (strcmp(sopClass, req->AffectedSOPClassUID) != 0)
				{
				rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
				}
				else if (strcmp(sopInstance, req->AffectedSOPInstanceUID) != 0)
				{
				rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
				}*/
			}
		}

	}
}

GADAPI::PACS::IncomingDicomAssociationCommandParams::IncomingDicomAssociationCommandParams(T_ASC_Association* assoc, unsigned long rcvTimeout) {
	m_pAssoc = assoc;
	m_rcvTimeout = rcvTimeout;
}

GADAPI::PACS::IncomingDicomAssociationCommandParams::~IncomingDicomAssociationCommandParams()
{
	if (m_pAssoc != NULL) {
		OFCondition cond = ASC_dropSCPAssociation(m_pAssoc);
		OFString temp_str;

		if (cond.bad())
		{
			LOG_FATAL("IncomingDicomAssociationCommandParams", DimseCondition::dump(temp_str, cond).c_str());
		}

		cond = ASC_destroyAssociation(&m_pAssoc);
		if (cond.bad())
		{
			LOG_FATAL("IncomingDicomAssociationCommandParams", DimseCondition::dump(temp_str, cond).c_str());
		}
		m_pAssoc = NULL;
	}
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

GADAPI::PACS::IncomingDicomAssociationCommand::IncomingDicomAssociationCommand(IncomingDicomAssociationCommandParams* pParams, const std::string& name) : IComando(pParams, name){
	m_pIncommingassociationParams = pParams;
	ambitolog = name;
}


GADAPI::PACS::IncomingDicomAssociationCommand::~IncomingDicomAssociationCommand() {
	
}

void GADAPI::PACS::IncomingDicomAssociationCommand::Execute() {
	OFCondition cond = EC_Normal;
	OFString temp_str;

	T_ASC_Association* assoc = m_pIncommingassociationParams->m_pAssoc;

	T_DIMSE_Message msg;
	T_ASC_PresentationContextID presID = 0;
	DcmDataset *statusDetail = NULL;

	std::string cmdname;

	// start a loop to be able to receive more than one DIMSE command
	while( cond == EC_Normal || cond == DIMSE_NODATAAVAILABLE || cond == DIMSE_OUTOFRESOURCES )
	{
		// receive a DIMSE command over the network
		cond = DIMSE_receiveCommand(assoc, DIMSE_BLOCKING, m_pIncommingassociationParams->m_rcvTimeout, &presID, &msg, &statusDetail);

		// if the command which was received has extra status
		// detail information, dump this information
		if (statusDetail != NULL)
		{
			LOG_WARN(ambitolog, "Status Detail:" << OFendl << DcmObject::PrintHelper(*statusDetail));
			delete statusDetail;
		}

		// check if peer did release or abort, or if we have a valid message
		if (cond == EC_Normal)
		{

			switch (msg.CommandField)
			{
			case DIMSE_C_ECHO_RQ:

				echoSCP(assoc, &msg, presID, &cond);

				break;
			case DIMSE_C_STORE_RQ:
				cmdname = "C-STORE-RQ";

				storeSCP(assoc, &msg, presID, &cond);
				break;

			case DIMSE_C_GET_RQ:
				cmdname = "C-GET-RQ";
#if 0
				getSCP(assoc, &msg, presID, &cond);
#else
				{
					LOG_ERROR(ambitolog, _Std("Ignoring incoming operation ") << cmdname << ":" << _Std("Not implemented"));

					DIC_UL x = 0, y = 0;
					DIMSE_ignoreDataSet(assoc, DIMSE_BLOCKING, 0, &x, &y);

					T_DIMSE_Message rsp;
					bzero((char*)&rsp, sizeof(rsp));
					rsp.CommandField = DIMSE_C_GET_RSP;
					rsp.msg.CGetRSP.DimseStatus = STATUS_GET_Failed_SOPClassNotSupported;
					rsp.msg.CGetRSP.MessageIDBeingRespondedTo = msg.msg.CGetRQ.MessageID;
					strcpy(rsp.msg.CGetRSP.AffectedSOPClassUID, msg.msg.CGetRQ.AffectedSOPClassUID);
					rsp.msg.CGetRSP.opts = O_GET_AFFECTEDSOPCLASSUID
						| O_GET_NUMBEROFREMAININGSUBOPERATIONS
						| O_GET_NUMBEROFCOMPLETEDSUBOPERATIONS
						| O_GET_NUMBEROFFAILEDSUBOPERATIONS
						| O_GET_NUMBEROFWARNINGSUBOPERATIONS;
					rsp.msg.CGetRSP.DataSetType = DIMSE_DATASET_NULL;
					rsp.msg.CGetRSP.NumberOfCompletedSubOperations = 0;
					rsp.msg.CGetRSP.NumberOfFailedSubOperations = 1;
					rsp.msg.CGetRSP.NumberOfRemainingSubOperations = 0;
					rsp.msg.CGetRSP.NumberOfWarningSubOperations = 0;

					DcmDataset ds_detail;
					ds_detail.putAndInsertString(DCM_SpecificCharacterSet, "ISO_IR 192");
					ds_detail.putAndInsertString(DCM_ErrorComment, _Std("Not implemented").c_str());
					cond = DIMSE_sendMessageUsingMemoryData(assoc, presID, &rsp, &ds_detail, NULL, NULL, NULL);					
				}
#endif
				break;
			case DIMSE_C_FIND_RQ:
				cmdname = "C-FIND-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_C_MOVE_RQ:
				cmdname = "C-MOVE-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_C_CANCEL_RQ:
				cmdname = "C-CANCEL-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_N_EVENT_REPORT_RQ:
				cmdname = "N-EVENT-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_N_GET_RQ:
				cmdname = "N-GET-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_N_SET_RQ:
				cmdname = "N-SET-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_N_ACTION_RQ:
				cmdname = "N-ACTION-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_N_CREATE_RQ:
				cmdname = "N-CREATE-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_N_DELETE_RQ:
				cmdname = "N-DELETE-RQ";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			case DIMSE_NOTHING:
				cmdname = "NOTHING";
				cond = DIMSE_BADCOMMANDTYPE;
				break;
			default:
				// we cannot handle this kind of message
				cond = DIMSE_BADCOMMANDTYPE;				
				break;
			}
		}
	}

	if (cond == DIMSE_BADCOMMANDTYPE) {
		std::ostringstream os;
				os << "0x" << std::hex << OFstatic_cast(unsigned, msg.CommandField);
				cmdname = os.str();
				LOG_ERROR(ambitolog, _Std("Ignoring incoming operation ") << cmdname << ":" << _Std("Not implemented"));
		ASC_abortAssociation(assoc);
	}
	else {
		if (cond == DUL_PEERREQUESTEDRELEASE) {
			LOG_DEBUG(ambitolog, "Association released by remote endpoint");
			cond = ASC_acknowledgeRelease(assoc);
		}
		else if (cond == DUL_PEERABORTEDASSOCIATION) {
			LOG_WARN(ambitolog, "Association Aborted by remote endpoint");
		}
		else {
			LOG_ERROR(ambitolog, "DIMSE failure (aborting association): " << DimseCondition::dump(temp_str, cond).c_str());
			// some kind of error so abort the association
			cond = ASC_abortAssociation(assoc);
		}
		if (cond.bad()) {
			LOG_WARN(ambitolog, "DIMSE failure (closing association): " << DimseCondition::dump(temp_str, cond).c_str());
		}
	}
}


void GADAPI::PACS::IncomingDicomAssociationCommand::echoSCP( T_ASC_Association * assoc, T_DIMSE_Message * msg, unsigned char presID, OFCondition* rcond)
{
	OFString temp_str;
	LOG_INFO(ambitolog, "Received Echo Request");
	LOG_DEBUG(ambitolog, DIMSE_dumpMessage(temp_str, msg->msg.CEchoRQ, DIMSE_INCOMING, NULL, presID));

	// the echo succeeded !! 
	OFCondition cond = DIMSE_sendEchoResponse(assoc, presID, &msg->msg.CEchoRQ, STATUS_Success, NULL);
	if (cond.bad())
	{
		LOG_ERROR(ambitolog, "Echo SCP Failed: " << DimseCondition::dump(temp_str, cond));
	}
	*rcond = cond;
}

/**
* This function processes a DIMSE C-STORE-RQ commmand that was
* received over the network connection.
*
* Parameters:
*   assoc  - [in] The association (network connection to another DICOM application).
*   msg    - [in] The DIMSE C-STORE-RQ message that was received.
*   presID - [in] The ID of the presentation context which was specified in the PDV which contained
*                 the DIMSE command.
*/
void GADAPI::PACS::IncomingDicomAssociationCommand::storeSCP( T_ASC_Association *assoc, T_DIMSE_Message *msg, unsigned char presID, OFCondition* rcond) {

	OFCondition cond = EC_Normal;
	T_DIMSE_C_StoreRQ *req;

	// assign the actual information of the C-STORE-RQ command to a local variable
	req = &msg->msg.CStoreRQ;

	if (m_TempDir.size() == 0) {
		 m_TempDir = GNC::Entorno::Instance()->CreateGinkgoTempFile();
    }

    // GW: still not secure, but no longer a resource leak, see also #16
    // since right now I don't know how to properly work around this, I'll just tag the code.
    // coverity[SECURE_TEMP]
    char *fileName = tempnam(m_TempDir.c_str(), "sscpfile_");
    if (!fileName) {
        LOG_ERROR(ambitolog, "Unable to create unique file name:"<< strerror(errno));
        *rcond = makeOFCondition(OFM_dcmnet, 20, OF_error, "IncomingDicomAssociationCommand::storeSCP: unable to create unique file name");
        return;
    }

	// dump some information if required
	OFString str;
	LOG_DEBUG(ambitolog, "Received Store Request: MsgID " << req->MessageID << ", (" << dcmSOPClassUIDToModality(req->AffectedSOPClassUID, "OT") << ")");
	LOG_DEBUG(ambitolog, DIMSE_dumpMessage(str, *req, DIMSE_INCOMING, NULL, presID));

	// intialize some variables
	StoreCallbackData callbackData;
	callbackData.assoc = assoc;
	DcmFileFormat dcmff;
	callbackData.dcmff = &dcmff;

	// store SourceApplicationEntityTitle in metaheader
	if (assoc && assoc->params)
	{
		const char *aet = assoc->params->DULparams.callingAPTitle;
		if (aet) dcmff.getMetaInfo()->putAndInsertString(DCM_SourceApplicationEntityTitle, aet);
	}

	// define an address where the information which will be received over the network will be stored

	// if opt_bitPreserving is set, the user requires that the data shall be
	// written exactly as it was received. Depending on this option, function
	// DIMSE_storeProvider must be called with certain parameters.
	bool opt_useMetaheader = true;
	int opt_dimse_timeout = 60;
    cond = DIMSE_storeProvider(assoc, presID, req, fileName, opt_useMetaheader, NULL, storeSCPCallback, &callbackData, DIMSE_NONBLOCKING, opt_dimse_timeout);



	// if some error occured, dump corresponding information and remove the outfile if necessary
	if (cond.bad())
	{
		OFString temp_str;
		LOG_ERROR(ambitolog, "Store SCP Failed: " << DimseCondition::dump(temp_str, cond));
		// remove file
        if (strcmp(fileName, NULL_DEVICE_NAME) != 0)
            OFStandard::deleteFile(fileName);
	}
	else {
        m_DicomFileList.push_back(std::string(fileName));
	}
    free(fileName);
	*rcond = cond;
}

void GADAPI::PACS::IncomingDicomAssociationCommand::Update() {


	if(m_Error != ""){
		LOG_WARN(ambitolog, m_Error);
		GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, m_Error, GNC::GCS::Events::EventoMensajes::PopUpMessage, false, GNC::GCS::Events::EventoMensajes::Error));
	}
	
	if (m_DicomFileList.size() > 0) {
		GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(m_DicomFileList, false, GNC::GCS::HistoryController::TAA_MOVE);
		GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial * pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
		GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Including files..."),pCmd,NULL);
	}
}
