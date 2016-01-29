/*
 *  
 *  $Id: dicomcustomassociation.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
#include <string>
#include <vector>
#include <list>
#include "dicomassociation.h"
#include <api/dicom/icustomassociation.h>

#include <api/dicom/imodelodicom.h>
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/dcmtk/dicomnetwork.h>
#include <api/controllers/ipacscontroller.h>

#include <dcmtk/ofstd/ofstream.h>
#include <dcmtk/ofstd/ofstring.h>      /* for class OFString */

class DicomImage;


class CustomEventHandler
{
public:

  /// destructor
  virtual ~CustomEventHandler() {}

  /** handles an N-EVENT-REPORT-RQ message. This method is called
   *  from CustomAssociation whenever an event report is received.
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

class CustomAssociation : public Association, public GIL::DICOM::ICustomAssociation {
public:
	
	/**
	 Constructors
	 */
	CustomAssociation(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificadorProgreso = NULL);
	
	virtual ~CustomAssociation();

	virtual bool Connect(const std::string& serverId, const std::string& local_aet);
	
	virtual bool createRQ( const std::string& sopclassUID, const GIL::DICOM::DicomDataset& attributeListIn,  int& status, std::string& affectedSopinstanceUID);
	virtual bool setRQ(    const std::string& sopclassUID, const std::string& sopinstanceUID, const GIL::DICOM::DicomDataset& modificationList, int& status);
	virtual bool getRQ(    const std::string& sopclassUID, const std::string& sopinstanceUID, const int *attributeIdentifierList, unsigned long numShorts, int& status,  GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& attributeListOut);
	virtual bool actionRQ( const std::string& sopclassUID, const std::string& sopinstanceUID, int actionTypeID, const GIL::DICOM::DicomDataset& actionInformation, int& status,  GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& attributeListOut);
	virtual bool deleteRQ( const std::string& sopclassUID, const std::string& sopinstanceUID, int& status);

	virtual bool releaseAssociation();
	virtual bool abortAssociation();


	void dumpNMessage(T_DIMSE_Message &msg, DcmItem *dataset, OFBool outgoing);
	OFCondition sendNRequest( T_ASC_PresentationContextID presId, T_DIMSE_Message &request, DcmDataset *rqDataSet, T_DIMSE_Message &response, DcmDataset* &statusDetail, DcmDataset* &rspDataset);
	T_ASC_PresentationContextID findAcceptedPC(const char *sopclassuid);
	
private:

	void Gnk2Dcmtk(const GIL::DICOM::DicomDataset& base, DcmDataset* query);
	
private:
	GIL::DICOM::DCMTK::Network* Net;
	std::string                 ambitolog;

protected:
	GNC::IProxyNotificadorProgreso* m_pNotificadorProgreso;
	
	virtual void OnAddPresentationContext(T_ASC_Parameters* params);
	
	T_ASC_PresentationContextID presId;

private:
	
	T_DIMSE_BlockingMode blockMode;
	/// the current event handler. Can be NULL.
	CustomEventHandler *eventHandler;
	GNC::GCS::Ptr<DicomServer> m_server;
};
