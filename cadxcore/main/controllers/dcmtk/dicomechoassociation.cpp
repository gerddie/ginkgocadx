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

#include <api/controllers/icontroladorlog.h>
#include <api/dicom/idicomconformance.h>
#include <main/controllers/controladorlog.h>
#include "dicomechoassociation.h"


#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif


#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

EchoAssociation::EchoAssociation(const std::string& ambitolog) : Association(ambitolog)
{
        m_abstractSyntax = GIL::DICOM::Conformance::GetScpSOPClasses().GetSOPClassUID("VerificationSOPClass");
}

EchoAssociation::~EchoAssociation()
{
}

void EchoAssociation::OnAddPresentationContext(T_ASC_Parameters* params)
{
        GIL::DICOM::ArrayHelper transfersyntaxes;
        transfersyntaxes.copyFrom(GIL::DICOM::Conformance::GetScuSOPClasses().GetSupportedTransferSyntaxUIDs("VerificationSOPClass"));


        OFCondition cond = ASC_addPresentationContext(params, 1, m_abstractSyntax.c_str(), transfersyntaxes.array, transfersyntaxes.size);
        if (cond.bad()) {
                LOG_ERROR(ambitolog, "Unable to add default presentation context");
        }
}
