/*
*  
*  $Id: cloupuploadcommand.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#pragma once
#include <api/globals.h>
#include <api/icommand.h>
#include <api/dicom/idicom.h>
#include <vector>

namespace GNC {
	namespace GCS {
		class CloudCommandParams : public ::GNC::GCS::IComandoParams {

		public:

			typedef enum ActionType {
				CC_GET,
				CC_FIND
			} ActionType;

		protected:

			CloudCommandParams(ActionType action);
			~CloudCommandParams();

		public:
			const ActionType m_action;
			

		public:
			std::string m_error;
		};

		class CloudGetCommandParams : public CloudCommandParams {
		public:

			CloudGetCommandParams(const std::string& url, bool base64);
			~CloudGetCommandParams();

			std::string m_url;
			std::string m_baseDir;
		};

		class CloudFindCommandParams : public CloudCommandParams {
		public:

			typedef std::map<std::string, std::string> ResultTuple;
			typedef std::list<ResultTuple> Results;

			CloudFindCommandParams(const std::string& bucketId, const std::string& userNameQuery, const std::string& descriptionQuery, const std::string& modalitiesQuery, const std::string& fromDateQuery, const std::string& toDateQuery);
			~CloudFindCommandParams();

			const std::string m_bucketId;
			const std::string m_userNameQuery;
			const std::string m_descriptionQuery;
			const std::string m_modalitiesQuery;
			const std::string m_fromDateQuery;
			const std::string m_toDateQuery;

			Results m_pResults;
			
		};


		class CloudCommand : public GNC::GCS::IComando {
		public:
			CloudCommand(::GNC::GCS::CloudCommandParams* pParams);

		protected:
			virtual void Execute();
			virtual void Update();

		private:
			void doGet(GNC::GCS::CloudGetCommandParams* pParams);
			void doFind(GNC::GCS::CloudFindCommandParams* pParams);

		public:

			::GNC::GCS::CloudCommandParams* m_pCloudParams;
		};
	}
}