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
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <api/api.h>

namespace GIL {
	namespace DICOM {

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		typedef std::map<std::string, std::string> TransferSyntaxMapImpl;
		typedef std::list<std::string> TransferSyntaxList;
		typedef std::list<std::string> SOPClassList;

		class EXTAPI TransferSyntaxMap : public TransferSyntaxMapImpl {
		public:

			void AddTransferSyntax(const std::string& tsRef, const std::string& tsUID);

			const std::string& GetTransferSyntaxUID(const std::string& tsRef) const;
		};

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		class EXTAPI SOPClassTuple {
		public:
			std::string scUID;
			TransferSyntaxList tsRefs;
			TransferSyntaxList tsUIDs;

			inline SOPClassTuple()
			{
			}

			inline SOPClassTuple(const std::string& scUID) {
				this->scUID = scUID;
			}

			inline SOPClassTuple(const SOPClassTuple& o) {
				*this = o;
			}

			inline SOPClassTuple& operator=(const SOPClassTuple& o) {
				this->scUID  = o.scUID;
				this->tsRefs = o.tsRefs;
				this->tsUIDs = o.tsUIDs;
				return *this;
			}

		};

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		class EXTAPI ArrayHelper {
		public:
			ArrayHelper();

			ArrayHelper(const std::list<std::string>& list);

			ArrayHelper(const ArrayHelper& o);
			
			ArrayHelper& operator=(const ArrayHelper& o);

			ArrayHelper& operator=(const std::list<std::string>& list);

            ~ArrayHelper();

			void copyFrom(const std::list<std::string>& list);

			void free();

            void swap(ArrayHelper& other);

			const char** array;
			unsigned int size;
		};
		
		typedef std::map<std::string, SOPClassTuple > SOPClassMapImpl;

		class EXTAPI SOPClassMap : public SOPClassMapImpl {

		public:

			void AddSOPClass(const std::string& scRef, const std::string& scUID);

			void AddTransferSyntaxForSOP(const std::string& scRef, const std::string& tsRef);

			const std::string& GetSOPClassUID(const std::string& scRef) const;

			/** Gets the transfer syntax names list (refs) supported by this SOP Class **/
			const TransferSyntaxList& GetSupportedTransferSyntaxRefs(const std::string& scRef) const;
			
			/** Gets the transfer syntax oid list supported by this SOP Class **/
			const TransferSyntaxList& GetSupportedTransferSyntaxUIDs(const std::string& scRef) const;
		};

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		class EXTAPI ModalityTuple {
		public:
			std::string descr;
			SOPClassList scRefs;
			SOPClassList scUIDs;

			inline ModalityTuple()
			{
			}

			inline ModalityTuple(const std::string& descr) {
				this->descr = descr;
			}

			inline ModalityTuple(const ModalityTuple& o) {
				*this = o;
			}

			inline ModalityTuple& operator=(const ModalityTuple& o) {
				this->descr  = o.descr;
				this->scRefs = o.scRefs;
				this->scUIDs = o.scUIDs;
				return *this;
			}

		};


		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		typedef std::map<std::string, ModalityTuple > ModalityMapImpl;

		class EXTAPI ModalityMap : public ModalityMapImpl {

		public:

			void AddModality(const std::string& mRef, const std::string& desr);
			
			void AddSOPClassForModality(const std::string& mRef, const std::string& scRef);

			const std::string& GetDescription(const std::string& mRef) const;

			/** Gets the SOP Classes names (refs) list supported by this Modality **/
			const SOPClassList& GetSupportedSOPClassRefs(const std::string& mRef) const;
			
			/** Gets the SOP Classes oid list supported by this Modality **/
			const SOPClassList& GetSupportedSOPClassUIDs(const std::string& mRef) const;
		};

		//-----------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------

		class EXTAPI Conformance {

		public:

			static void Load();

			static const GIL::DICOM::TransferSyntaxMap& GetTransferSyntaxes();

			static const GIL::DICOM::SOPClassMap& GetStorageSOPClasses();

			static const GIL::DICOM::SOPClassMap& GetScpSOPClasses();

			static const GIL::DICOM::SOPClassMap& GetScuSOPClasses();

			static const GIL::DICOM::ModalityMap& GetModalities();

		private:

			static GIL::DICOM::TransferSyntaxMap m_TransferSyntaxes;

			static GIL::DICOM::SOPClassMap m_StorageSOPClasses;

			static GIL::DICOM::SOPClassMap m_ScpSOPClasses;

			static GIL::DICOM::SOPClassMap m_ScuSOPClasses;

			static GIL::DICOM::ModalityMap m_Modalities;

			static bool m_Loaded;
			
		};
	}
}
