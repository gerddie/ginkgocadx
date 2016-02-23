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
#include <api/ilock.h>

namespace GNC {
	namespace GCS {
		namespace Logging {

			class LogEvent {

			public:

				LogEvent() {}

				LogEvent(const std::string& lg, const std::string& ll,
					 const std::string& ts, const std::string& m, const std::string& fm, const std::string& th)
					: logger(lg),
					level(ll),
					timestamp(ts),
					msg(m),
					thread(th), 
					formattedmsg(fm)
				{					
				}

				LogEvent(const LogEvent& o)
				{
					(*this) = o;
				}

				LogEvent& operator = (const LogEvent& o)
				{
					logger = o.logger;
					level = o.level;
					timestamp = o.timestamp;
					msg = o.msg;
					formattedmsg = o.formattedmsg;
					thread = o.thread;
					return *this;
				}

				~LogEvent(){}

				std::string logger;
				std::string level;
				std::string timestamp;
				std::string msg;
				std::string thread;
				std::string formattedmsg;


				
		
			};


			class ILogger : public GNC::GCS::ILockable
			{
			public:
				ILogger() {}
				virtual ~ILogger(){}

				virtual void Append(const LogEvent& le) = 0;

			};
		}
	}
}
