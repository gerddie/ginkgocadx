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
#include "dicomassociation.h"
#include <api/dicom/imodelodicom.h>
#include <list>
#include <api/autoptr.h>

// This was defined in dcmtk3.5.4 but has been removed since
// Redefing it here may blow right in your face
// 
#define UID_FINDGeneralPurposeWorklistInformationModel   "1.2.840.10008.5.1.4.32.1"

class DicomServer;
namespace GIL {
	namespace DICOM
	{
		class DicomDataset;
	}
}

class FindAssociation : public Association {
public:
	
	typedef enum FindRootQueryRetrieveInformationModel
	{
		FRQRIM_Patient = 0,
		FRQRIM_Study,
		FRQRIM_PatientStudyOnly,
		FRQRIM_WorkListInformation,
		FRQRIM_GeneralPurposeWorklist,
		FRQRIM_ProtocolInformation,
		FRQRIM_ModalityWorklist
	} FindRootQueryRetrieveInformationModel;
	
	/**
	 Constructors
	 */
	FindAssociation(const std::string& ambitolog);
	
	virtual ~FindAssociation();
	
	/* Find Root Query Retrieve InformationModel */
	void SetFindRootQueryRetrieveInformationModel(FindRootQueryRetrieveInformationModel frqi)
	{
		switch (frqi) {
			case FRQRIM_Patient:
				Association::m_abstractSyntax = (char *) UID_FINDStudyRootQueryRetrieveInformationModel;
				break;
			case FRQRIM_Study:
				Association::m_abstractSyntax = (char *) UID_FINDStudyRootQueryRetrieveInformationModel;
				break;
			case FRQRIM_WorkListInformation:
				Association::m_abstractSyntax = (char *) UID_FINDModalityWorklistInformationModel;
				break;
			case FRQRIM_GeneralPurposeWorklist:
				Association::m_abstractSyntax = (char *) UID_FINDGeneralPurposeWorklistInformationModel;
				break;
			case FRQRIM_ProtocolInformation:
				Association::m_abstractSyntax = (char *) UID_FINDHangingProtocolInformationModel;
				break;
			case FRQRIM_ModalityWorklist:
				Association::m_abstractSyntax = (char *) UID_FINDModalityWorklistInformationModel;
				break;
			default:
				throw std::exception();
		}
	}

	void SetAbstractSyntax(const std::string& abstractSyntax)
	{
		Association::m_abstractSyntax = abstractSyntax.c_str();
	}
	
	
	/**
	 Send a query object (C-Find) through association
	 */
	OFCondition SendObject(DcmDataset *dataset);
	
	/**
	 Get the result stack of the last query
	 */
	virtual DcmStack* GetResultStack();
	
	DcmStack CopyResultStack();
	
	/**
	 Delete all objects from the result stack
	 */
	virtual void DeleteResultStack();
	
	/**
	 Set the maximum number of results
	 */
	void SetMaxResults(int max);

	/**
	Set results wrapper to do incremental finds
	*/
	void SetCallbackInfo(std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >* pRwrapper, const GNC::GCS::Ptr<DicomServer>& dicomServer);
	
	/**
	 Get the maximum number of results
	 */
	int GetMaxResults();
	
protected:
	
	/**
	 Response handler
	 */
	virtual void OnResponseReceived(DcmDataset* response);
	
	/**
	 Protected data
	 */
	DcmStack result;
	
	bool bPushResults;
	
private:
	
	typedef struct _FindCallbackInfo {
		FindAssociation* pCaller;
		T_ASC_Association* assoc;
    } FindCallbackInfo;
	
	/**
	 C-Find service class user
	 */
	OFCondition findSCU(T_ASC_Association * assoc, DcmDataset * query);
	
	/**
	 Callback function for C-Find service class user
	 */
	static void findCallback(void*, T_DIMSE_C_FindRQ*, int responseCount, T_DIMSE_C_FindRSP*, DcmDataset *responseIdentifiers);
	int maxResults;
	std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >* pResultsWrapper;
	GNC::GCS::Ptr<DicomServer> pDicomServer;
	
	virtual void OnAddPresentationContext(T_ASC_Parameters* params);
	
};
