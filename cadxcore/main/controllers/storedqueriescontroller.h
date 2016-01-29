/*
 *  
 *  $Id: hangingprotocolcontroller.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#include <string>
#include <map>
#include <list>
#include <api/autoptr.h>
#include <api/dicom/idicom.h>

class wxXmlNode;
namespace GNC {
	namespace GCS {
		class StoredQuery;
		/**
		Defines a specific layout, is recursive to allow splitted cells
		*/
		class Condition {
		public:
			Condition();
			Condition(const std::string& tag, const std::string& value, bool eval);
			/**
			deserialize from xml
			*/
			Condition(wxXmlNode* layoutRoot);
			Condition(const Condition& other);
			~Condition();
			
			wxXmlNode* serialize();

		public:
			/**
			getters and setters
			*/
			std::string getTag();
			void setTag(std::string tag);
			std::string getValue();
			void setValue(std::string value);
			bool hasToEval();
			void setEval(bool eval);

			//evaluate condition and insert in query
			void addConditionToQuery(GIL::DICOM::DicomDataset& query);

		protected:
			//tag of condition
			std::string tag;
			//value of condition
			std::string value;
			//has to eval condition, i.e. "TODAY"
			bool eval;
		};

		/**
		HangingProtocol class, defines a hanging protocol
		*/
		class StoredQuery {
		public:
			typedef enum {
				TSC_TODAY_MORNING,
				TSC_TODAY_AFTERNOON,
				TSC_TODAY,
				TSC_YESTERDAY,
				TSC_LAST_7_DAYS,
				TSC_LAST_30_DAYS,
				TSC_LAST_3_MONTHS
			} TSpecialConditions;

			typedef std::map<std::string, GNC::GCS::Ptr<Condition> > TMapOfConditions;
			StoredQuery();
			/**
			deserialize from xml
			*/
			StoredQuery(wxXmlNode* node);
			StoredQuery(const StoredQuery& other);
			~StoredQuery();
			/**
			serialize to XML
			*/
			wxXmlNode* serialize();
			/**
			inserts conditions into query
			*/
			void buildQuery(GIL::DICOM::DicomDataset& query);

			/**
			Getters and setters
			*/
			const std::string& getName();
			void setName(const std::string& name);
			const std::string& getPacsId();
			void setPacsId(const std::string& pacsId);
			bool isActive();
			void setActive(bool active);

			void addCondition(const GNC::GCS::Ptr<Condition>& condition);
			GNC::GCS::Ptr<Condition> getCondition(const std::string& tagValue);
			bool hasCondition(const std::string& tagValue);
			void clearConditions();

			void addCondition(const std::string& tag, const std::string& value = "");
			void addConditionIfNotExists(const std::string& tag, const std::string& value = "");
			void addSpecialCondition(TSpecialConditions condition);
			bool containsSpecialCondition(TSpecialConditions condition);

			//if there isn't any condition
			bool isEmpty();

		protected:

			/**
			Query name
			*/
			std::string name;
			/**
			PACS id
			*/
			std::string pacsId;
			/**
			conditions of the stored query
			*/
			TMapOfConditions conditions;
			/*
			if it's active
			*/
			bool active;
		};

		/**
		Hanging protocol controller class
		*/
		class StoredQueryController: GNC::GCS::ILockable
		{
		public:
			typedef std::list<GNC::GCS::Ptr<StoredQuery> > TStoredQueriesList;
			/**
			Singleton method
			*/
			static StoredQueryController* Instance();

			/**
			Free singleton
			*/
			static void FreeInstance();

			/**
			stores hanging protocol list
			*/
			void StoreStoredQueriesConfiguration();
			/**
			gets a copy of storedQueries
			*/
			TStoredQueriesList getStoredQueries();

			/**
			get a copy of active queries
			*/
			TStoredQueriesList getActiveQueries();

			/**
			get a copy of the query by id
			*/
			GNC::GCS::Ptr<StoredQuery> getQuery(const std::string& queryId);

			/**
			removes a query from configuration
			*/
			void removeQuery(const std::string& queryId);

			/**
			store query (overwrite existing)..
			*/
			void storeQuery(const GNC::GCS::Ptr<StoredQuery>& pQuery);

			/**
			store query (overwrite existing)..
			*/
			void storeQueries(const TStoredQueriesList& queries);
			/**
			clear stored queries
			*/
			void clearQueries();
		protected:
			void getActiveQueries(TStoredQueriesList& list, bool onlyActive);

			/**
			Creation method
			*/
			StoredQueryController();

			/**
			Destruction method
			*/
			~StoredQueryController();

			/**
			Load from configuration
			*/
			void LoadFromConfiguration();

			/**
			singleton attribute
			*/
			static StoredQueryController* m_pInstance;

			/**
			protocol list
			*/
			typedef std::map<std::string, GNC::GCS::Ptr<StoredQuery> > TStoredQueriesMap;
			TStoredQueriesMap mapOfQueries;
		};
	}
}
