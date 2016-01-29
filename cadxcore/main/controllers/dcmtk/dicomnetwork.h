/*
*  
*  $Id: dicomnetwork.h $
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
#include <map>
#include <api/ilock.h>
#include "dicomassociation.h"

namespace GIL {
	namespace DICOM {

		namespace DCMTK {

			class Network {

			protected:
				typedef std::map<void*, Network*> IntanceMapType;

			public:
				/** Keyed Singleton **/
				static Network* Instance(void* key);
				static void FreeInstance(void* key);

				bool Initiallized();

				/**
				Initialize the dicom network
				*/

				OFCondition InitializeNetwork(int timeout,  Association::RoleType role = Association::RT_Requestor, int port = 0);

				/**
				Drop the dicom network
				*/
				OFCondition DropNetwork();

				/**
				Connect an association to the specified host
				*/
				OFCondition ConnectAssociation(Association* assoc, int pdu = ASC_DEFAULTMAXPDU);

				/**
				Send C-Echo request to dicom node
				*/
				OFCondition SendEchoRequest(const std::string& title, const std::string& peer, int port, const std::string& ouraet, int pdu);

				/**
				Get the pointer to the internal dcmtk network variable (sorry)
				*/
				T_ASC_Network* GetDcmtkNet();

				void SetDcmtkNet(T_ASC_Network* n);

			protected:

				/**
				Connect to a host and try to establish an association
				*/
				OFCondition ASC_ConnectAssociation(Association* assoc, const std::string& peerTitle, const std::string& peer, int port, const std::string& ouraet, int pdu = ASC_DEFAULTMAXPDU);

				/**
				THE dicom network
				*/
				T_ASC_Network* m_pDCMTKNetwork;

				/**
				constructor
				*/
				Network();

				virtual ~Network();

			protected:
				bool m_Initiallized;
				/** Attributes **/
				static IntanceMapType m_pInstancesMap;
				static GNC::GCS::ILockable m_sLock;

				friend class Association;
			};
		}
	}
}
