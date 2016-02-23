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
#include <api/autoptr.h>
#include "dicomassociation.h"
#include <api/dicom/imodelodicom.h>
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/dcmtk/dicomnetwork.h>

#include <dcmtk/ofstd/ofstream.h>
#include <dcmtk/ofstd/ofstring.h>      /* for class OFString */

namespace GIL {
        namespace DICOM {
                class DicomDataset;
        }
}

class DicomImage;


class PrintEventHandler
{
public:

  /// destructor
  virtual ~PrintEventHandler() {}

  /** handles an N-EVENT-REPORT-RQ message. This method is called
   *  from PrintAssociation whenever an event report is received.
   *  The event message, information and status detail elements must be copied
   *  if they should remain valid after the end of this method call.
   *  The event handler can react on the event message in any implementation
   *  dependent way, e.g. display the event on-screen.
   *  @param eventMessage the N-EVENT-REPORT-RQ message
   *  @param eventInformation event information dataset. May be NULL if no event information available.
   *  @param statusDetail status detail dataset. ay be NULL if no status detail available.
   *  @return the status code to be sent back as part of the N-EVENT-REPORT-RSP message, usually STATUS_Success.
   */
  virtual DIC_US handleEvent(
    T_DIMSE_N_EventReportRQ& eventMessage,
    DcmDataset *eventInformation,
    DcmDataset *statusDetail)=0;
};

class PrintAssociation : public Association {
public:
	
	/**
	 Constructors
	 */
	PrintAssociation(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificadorProgreso = NULL);
	
	virtual ~PrintAssociation();
	
	/**
	 Send a query object (C-Find) through association
	 */
	OFCondition Print(const GNC::GCS::Ptr<DicomServer>& server, const std::string& local_aet, const GIL::DICOM::DicomDataset& film, const GIL::DICOM::DicomDataset& layout, const GIL::DICOM::DicomDataset& job, const std::list<std::string>& files);
	
private:
	
	private:
	GIL::DICOM::DCMTK::Network* Net;
	std::string                 ambitolog;

protected:
	GNC::IProxyNotificadorProgreso* m_pNotificadorProgreso;
	
	virtual void OnAddPresentationContext(T_ASC_Parameters* params);
	
	T_ASC_PresentationContextID presId;
private:
	
	void dumpNMessage(T_DIMSE_Message &msg, DcmItem *dataset, OFBool outgoing);
	OFCondition sendNRequest( T_ASC_PresentationContextID presId, T_DIMSE_Message &request, DcmDataset *rqDataSet, T_DIMSE_Message &response, DcmDataset* &statusDetail, DcmDataset* &rspDataset);
	OFCondition createRQ(const char *sopclassUID, OFString& sopinstanceUID, DcmDataset *attributeListIn, Uint16& status, DcmDataset* &attributeListOut);
	OFCondition setRQ( const char *sopclassUID, const char *sopinstanceUID, DcmDataset *modificationList, Uint16& status, DcmDataset* &attributeListOut);
	OFCondition getRQ( const char *sopclassUID,  const char *sopinstanceUID,  const Uint16 *attributeIdentifierList, ssize_t numShorts, Uint16& status,  DcmDataset* &attributeListOut);
	OFCondition actionRQ( const char *sopclassUID,  const char *sopinstanceUID,  Uint16 actionTypeID,  DcmDataset *actionInformation, Uint16& status,  DcmDataset* &actionReply);
	OFCondition deleteRQ( const char *sopclassUID,  const char *sopinstanceUID,  Uint16& status);
	OFCondition releaseAssociation();
	OFCondition abortAssociation();
	T_ASC_PresentationContextID findAcceptedPC(const char *sopclassuid);
	OFCondition negotiateAssociation(OFBool negotiatePresentationLUT, OFBool negotiateAnnotationBox, OFBool implicitOnly);
	OFBool printerSupportsPresentationLUT();
	OFBool printerSupportsAnnotationBox();
	
	T_DIMSE_BlockingMode blockMode;
	
	/// the current event handler. Can be NULL.
	PrintEventHandler *eventHandler;
};
