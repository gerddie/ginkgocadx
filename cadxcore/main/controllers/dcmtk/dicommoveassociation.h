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
#include <ctime>

#include "dicomfindassociation.h"
#include "istorecallback.h"

class MoveAssociation : public FindAssociation
{
public:

        MoveAssociation(const std::string& ambitolog, IModeloDicom* pModelo=NULL);
        virtual ~MoveAssociation();

        void SetModelo(IModeloDicom* pModelo);

        void Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, /*int ourPort,*/ const char *abstractSyntax = UID_MOVEStudyRootQueryRetrieveInformationModel);

        OFCondition SendObject(DcmDataset *dataset);


protected:

        void OnAddPresentationContext(T_ASC_Parameters *params);

private:

        OFCondition moveSCU(DcmDataset *pdset);

        int m_maxReceivePDULength;
        IModeloDicom* m_pModelo;
        std::string m_errorMessage;
};
