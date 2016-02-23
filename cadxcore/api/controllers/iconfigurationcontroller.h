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

#include <string>
#include <map>
#include <list>
#include <sstream>

#include <api/api.h>

namespace GNC {
	namespace GCS {
		class EXTAPI  IConfigurationController {
		public:
			class TMapValues: public std::map<std::string, std::string> 
			{
			public:
				bool readStringValue(const std::string& key, std::string& value, const std::string& defaultValue = "") {
					if (find(key) != end()) {
						value = (*this)[key];
						return true;
					} else {
						value = defaultValue;
						return false;
					}
				}
				bool readBoolValue(const std::string& key, bool& value, bool defaultValue = true) {
					if (find(key) != end()) {
						std::istringstream istr((*this)[key]);
						istr >> value;
						return true;
					} else {
						value = defaultValue;
						return false;
					}
				}

				bool readIntValue(const std::string& key, int& value, int defaultValue = 0) {
					if (find(key) != end()) {
						std::istringstream istr((*this)[key]);
						istr >> value;
						return true;
					} else {
						value = defaultValue;
						return false;
					}
				}

				void insertInt(const std::string& key, int value) {
					std::ostringstream ostr;
					ostr << value;
					(*this)[key] = ostr.str();
				}

				void insertBool(const std::string& key, bool value) {
					std::ostringstream ostr;
					ostr << value;
					(*this)[key] = ostr.str();
				}
			};
			typedef std::list<TMapValues> TListGroups;

			virtual void Flush() = 0;

			virtual bool deleteEntryGeneral(const std::string& scope, const std::string&key = "") = 0;
			virtual bool writeStringGeneral(const std::string& scope, const std::string& key, const std::string& value) = 0;
			virtual bool readStringGeneral(const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue = "") = 0;
			virtual bool writeBoolGeneral(const std::string& scope, const std::string& key, bool value) = 0;
			virtual bool readBoolGeneral(const std::string& scope, const std::string& key, bool& value, bool defaultValue = true) = 0;
			virtual bool readIntGeneral(const std::string& scope, const std::string& key, int& value, int defaultValue = 0) = 0;
			virtual bool writeIntGeneral(const std::string& scope, const std::string& key, int value) = 0;
			virtual bool writeGroupGeneral(const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix = "") = 0;
			virtual bool readGroupGeneral(const std::string& scope, TListGroups& listOfGroups) = 0;

			virtual bool deleteEntryUser(const std::string& scope, const std::string&key = "") = 0;
			virtual bool writeStringUser(const std::string& scope, const std::string& key, const std::string& value) = 0;
			virtual bool readStringUser(const std::string& scope, const std::string& key, std::string& value, const std::string& defaultValue = "") = 0;
			virtual bool readBoolUser(const std::string& scope, const std::string& key, bool& value, bool defaultValue = true) = 0;
			virtual bool writeBoolUser(const std::string& scope, const std::string& key, bool value) = 0;
			virtual bool readIntUser(const std::string& scope, const std::string& key, int& value, int defaultValue = 0) = 0;
			virtual bool writeIntUser(const std::string& scope, const std::string& key, int value) = 0;
			virtual bool writeGroupUser(const std::string& scope, const TListGroups& listOfGroups, const std::string& prefix = "") = 0;
			virtual bool readGroupUser(const std::string& scope, TListGroups& listOfGroups) = 0;

			static IConfigurationController* Instance();
			static void FreeInstance();
		protected:
			IConfigurationController();
			virtual ~IConfigurationController();
		};
	}
}
