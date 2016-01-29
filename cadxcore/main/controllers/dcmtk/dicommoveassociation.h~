/*
*  
*  $Id: dicommoveassociation.h $
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
#include <ctime>

#include "dicomfindassociation.h"
#include "istorecallback.h"

class MoveAssociation : public FindAssociation {
public:

	MoveAssociation(const std::string& ambitolog, IModeloDicom* pModelo=NULL);
	virtual ~MoveAssociation();

	void SetModelo(IModeloDicom* pModelo);

	void Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, /*int ourPort,*/ const char *abstractSyntax = UID_MOVEStudyRootQueryRetrieveInformationModel);

	CONDITION SendObject(DcmDataset *dataset);


protected:

	void OnAddPresentationContext(T_ASC_Parameters *params);

private:

	CONDITION moveSCU(DcmDataset *pdset);

	int m_maxReceivePDULength;
	IModeloDicom* m_pModelo;
	std::string m_errorMessage;
};
