#pragma once
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
