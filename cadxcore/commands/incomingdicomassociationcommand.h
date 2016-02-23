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
#include <api/globals.h>
#include <api/icommand.h>

struct T_ASC_Association;
struct T_DIMSE_Message;
class OFCondition;

namespace GADAPI {

	namespace PACS {
		class EXTAPI IncomingDicomAssociationCommandParams : public GNC::GCS::IComandoParams {
		public:
			IncomingDicomAssociationCommandParams(T_ASC_Association* assoc, unsigned long rcvTimeout = 0);
			virtual ~IncomingDicomAssociationCommandParams();

			T_ASC_Association* m_pAssoc;
			long               m_rcvTimeout;
		};

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		class EXTAPI IncomingDicomAssociationCommand : public GNC::GCS::IComando {
		public:
			IncomingDicomAssociationCommand(IncomingDicomAssociationCommandParams* pParams, const std::string& name="SCP-Service");
			virtual ~IncomingDicomAssociationCommand();

		protected:

			virtual void Execute();
			virtual void Update();

			void echoSCP( T_ASC_Association * assoc, T_DIMSE_Message * msg, unsigned char presID, OFCondition* cond);
			void storeSCP( T_ASC_Association * assoc, T_DIMSE_Message * msg, unsigned char presID, OFCondition* cond);

			IncomingDicomAssociationCommandParams* m_pIncommingassociationParams;

			std::string ambitolog;
			
			std::list<std::string> m_DicomFileList;
			std::string m_TempDir;
			std::string m_Error;
		};

	}
}
