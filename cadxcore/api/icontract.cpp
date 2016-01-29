/*
 *  
 *  $Id: icontract.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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

bool GNC::GCS::IContract::operator<(const GNC::GCS::IContract& other) const {
	if (m_Priority < other.m_Priority) {
		return true;
	}
	return false;
}

bool GNC::GCS::IContract::operator<(const GNC::GCS::IContract* other) const {
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
