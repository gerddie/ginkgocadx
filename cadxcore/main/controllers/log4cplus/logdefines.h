/*
 *  
 *  $Id: logdefines.h $
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
