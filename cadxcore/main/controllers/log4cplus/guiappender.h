/*
 *  
 *  $Id: guiappender.h $
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
		
    };
}
