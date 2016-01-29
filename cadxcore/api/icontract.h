#pragma once
/*
 *  
 *  $Id: icontract.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef ICONTRACT_H
#define ICONTRACT_H

#include <map>
#include <api/api.h>
#include <api/iexception.h>

namespace GNC {
	namespace GCS {

		class EXTAPI ContractException : public GNC::GCS::IException {
		public:

			ContractException(const std::string& msg) throw();
		};

		//------------------------------------------------------------------------
		/* Contract generic interface.
		 */
		class EXTAPI IContract {
		public:

			typedef enum {
				TC_Mandatory,
				TC_Optional
			} ContractType;

			IContract(ContractType type = TC_Mandatory, int prio = 0);

			virtual ~IContract();

			void Ping();

			bool operator<(const IContract& other) const;

			bool operator<(const IContract* other) const;

		public:
			ContractType m_Type;
			int          m_Priority;
		};

		//------------------------------------------------------------------------
		/* Contract management class */
		class EXTAPI IContractable {

		//region "Public interface for contract management"

		protected:
			GNC::GCS::IContract* m_pContract;

		public:

			IContractable();

			virtual ~IContractable();

			virtual bool IsValidContract(IContract* contract) = 0;
			virtual void Register(IContract* contract);
		};
	}
}
#endif
