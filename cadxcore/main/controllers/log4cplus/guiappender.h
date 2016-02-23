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

#include <dcmtk/oflog/appender.h>
#include <sstream>
#include <list>
#include <api/ilock.h>
#include "logdefines.h"


namespace gnkLog4cplus {
	
	class  GUIAppender : public dcmtk::log4cplus::Appender {
		
	public:
		
		static GUIAppender* Instance();
		
		static void FreeInstance();
		
		// Methods
		virtual void close();

		virtual void clear();
		
		void Attach(GNC::GCS::Logging::ILogger* pLogger);

	private:
		void Init();
		
    protected:
		
		typedef std::list<GNC::GCS::Logging::LogEvent> ListaLogs;
		
		// Ctors		  
		GUIAppender(bool logToStdErr = false, bool immediateFlush = false);
		GUIAppender(const dcmtk::log4cplus::helpers::Properties properties);
		GUIAppender(const GUIAppender& o);
		
		// Dtor
		~GUIAppender();
		virtual void append(const dcmtk::log4cplus::spi::InternalLoggingEvent& event);
		
		// Data
		bool logToStdErr;
		/**
		 * Immediate flush means that the underlying output stream
		 * will be flushed at the end of each append operation.
		 */
		bool immediateFlush;
		
		GNC::GCS::Logging::ILogger* m_pLogger;
		ListaLogs                   m_Logs;
		
		dcmtk::log4cplus::LogLevelManager& llmCache;
		dcmtk::log4cplus::tstring         datetimeFormat;
		
		
		static GUIAppender* m_pInstancia;

		dcmtk::log4cplus::thread::Mutex m_mutex;
    };
}
