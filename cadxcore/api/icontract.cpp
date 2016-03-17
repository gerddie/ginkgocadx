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

#include "icontract.h"

GNC::GCS::ContractException::ContractException(const std::string& msg) throw() : GNC::GCS::IException(msg, "GNC", false)
{
}

GNC::GCS::IContract::IContract(ContractType type, int prio)
{
        m_Type = type;
        m_Priority = prio;
}

GNC::GCS::IContract::~IContract()
{

}
void GNC::GCS::IContract::Ping()
{
}

bool GNC::GCS::IContract::operator<(const GNC::GCS::IContract& other) const
{
        if (m_Priority < other.m_Priority) {
                return true;
        }
        return false;
}

bool GNC::GCS::IContract::operator<(const GNC::GCS::IContract* other) const
{
        if (m_Priority < other->m_Priority) {
                return true;
        }
        return false;
}
//------------------------------------------------------------------------
GNC::GCS::IContractable::IContractable()
{
        m_pContract = NULL;
}

GNC::GCS::IContractable::~IContractable()
{
        m_pContract = NULL;
}

void GNC::GCS::IContractable::Register(IContract* contract)
{
        if (IsValidContract(contract)) {
                m_pContract = contract;
        }
}
