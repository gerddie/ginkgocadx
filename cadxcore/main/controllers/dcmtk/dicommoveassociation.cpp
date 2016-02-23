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

#define LOGGER "C-MOVE"

#include "dicomnetwork.h"
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/controladorlog.h>
#include <api/dicom/imodelodicom.h>
#include <main/controllers/controladorpermisos.h>
#include <api/controllers/ipacscontroller.h>
#include <main/entorno.h>

#include <api/internationalization/internationalization.h>

#include "istorecallback.h"


#include "dicommoveassociation.h"

#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpeg/djrplol.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <wx/string.h>
#include <wx/intl.h>

#include "tls/tls.h"
#include "tls/gtlslayer.h"
#include <dcmtk/dcmtls/tlstrans.h>

MoveAssociation::MoveAssociation(const std::string& _ambitolog, IModeloDicom* pModelo) : FindAssociation(_ambitolog),
		m_maxReceivePDULength(ASC_DEFAULTMAXPDU),
		m_pModelo(pModelo),
		m_errorMessage("")
{
	m_abstractSyntax = UID_MOVEStudyRootQueryRetrieveInformationModel;

}

MoveAssociation::~MoveAssociation() { }

void MoveAssociation::Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, /*int ourPort,*/ const char *abstractSyntax) {
	Association::Create(title, peer, port, ouraet, abstractSyntax);
}

void MoveAssociation::SetModelo(IModeloDicom* pModelo) {
	m_pModelo = pModelo;
}

OFCondition MoveAssociation::SendObject(DcmDataset *dataset) {
	return moveSCU(dataset);
}

void MoveAssociation::OnAddPresentationContext(T_ASC_Parameters *params) {
	addAllStoragePresentationContexts(params, true, true);
}

OFCondition MoveAssociation::moveSCU(DcmDataset *pdset) {
	OFCondition cond;
	T_ASC_PresentationContextID presId;
	T_DIMSE_C_MoveRQ req;
	T_DIMSE_C_MoveRSP rsp;
	DIC_US msgId = assoc->nextMsgID++;
	DcmDataset* rspIds = NULL;
	const char* sopClass;
	DcmDataset* statusDetail = NULL;

	if (pdset == NULL) {
		return makeOFCondition(OFM_dcmnet, DIMSEC_BADDATA, OF_error, "NULL DIMSE Query");
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
	strcpy(req.MoveDestination, m_ourAET.c_str());

	cond = DIMSE_moveUser(
		assoc,
		presId,
		&req,
		pdset,
		NULL,
		NULL,
		DIMSE_BLOCKING,
		0,
		GetNetwork()->GetDcmtkNet(),
		NULL,
		this,
		&rsp, &statusDetail, &rspIds);

	if( rsp.DimseStatus != STATUS_Success && rsp.DimseStatus != STATUS_Pending) {
		std::ostringstream os;
		os << DU_cmoveStatusString(rsp.DimseStatus);
		if (statusDetail != NULL) {
			OFString errorComment;
			if (statusDetail->findAndGetOFString(DCM_ErrorComment, errorComment).good()) {
				os << ". " << errorComment.c_str();
			}
		}	
		m_errorMessage = os.str();
	}

	if (statusDetail != NULL) {
		LOG_DEBUG(ambitolog, "DIMSE_moveUser(): Estado: " << std::endl << DumpDataset(statusDetail));
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

	return cond;
}

