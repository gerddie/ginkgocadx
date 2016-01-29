#pragma once
/*
 *  
 *  $Id: icontroladorlog.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef ILOGCONTROLLER_H
#define ILOGCONTROLLER_H
#include <map>
#include <sstream>

#include <api/api.h>

#define LOG_FATAL(logger, mensaje) \
{ \
	GNC::GCS::IControladorLog* cL = GNC::GCS::IControladorLog::Instance(); \
	if (cL != NULL && cL->IsEnabledFor(GNC::GCS::IControladorLog::FatalLog) ) { \
		std::ostringstream  _buf;\
		_buf << mensaje;\
		cL->Log(logger,_buf.str(),GNC::GCS::IControladorLog::FatalLog);\
	} \
}

#define LOG_ERROR(logger, mensaje) \
{ \
	GNC::GCS::IControladorLog* cL = GNC::GCS::IControladorLog::Instance(); \
	if (cL != NULL && cL->IsEnabledFor(GNC::GCS::IControladorLog::ErrorLog) ) { \
		std::ostringstream  _buf;\
		_buf << mensaje;\
		cL->Log(logger,_buf.str(),GNC::GCS::IControladorLog::ErrorLog);\
	} \
}

#define LOG_WARN(logger, mensaje) \
{ \
	GNC::GCS::IControladorLog* cL = GNC::GCS::IControladorLog::Instance(); \
	if (cL != NULL && cL->IsEnabledFor(GNC::GCS::IControladorLog::WarnLog) ) { \
		std::ostringstream  _buf;\
		_buf << mensaje;\
		cL->Log(logger,_buf.str(),GNC::GCS::IControladorLog::WarnLog);\
	} \
}

#define LOG_INFO(logger, mensaje) \
{ \
	GNC::GCS::IControladorLog* cL = GNC::GCS::IControladorLog::Instance(); \
	if (cL != NULL && cL->IsEnabledFor(GNC::GCS::IControladorLog::InfoLog) ) { \
		std::ostringstream  _buf;\
		_buf << mensaje;\
		cL->Log(logger,_buf.str(),GNC::GCS::IControladorLog::InfoLog);\
	} \
}

#define LOG_DEBUG(logger, mensaje) \
{ \
	GNC::GCS::IControladorLog* cL = GNC::GCS::IControladorLog::Instance(); \
	if (cL != NULL && cL->IsEnabledFor(GNC::GCS::IControladorLog::DebugLog) ) { \
		std::ostringstream  _buf;\
		_buf << mensaje;\
		cL->Log(logger,_buf.str(),GNC::GCS::IControladorLog::DebugLog);\
	} \
}

#define LOG_TRACE(logger, mensaje) \
{ \
	GNC::GCS::IControladorLog* cL = GNC::GCS::IControladorLog::Instance(); \
	if (cL != NULL && cL->IsEnabledFor(GNC::GCS::IControladorLog::TraceLog) ) { \
		std::ostringstream  _buf;\
		_buf << mensaje;\
		cL->Log(logger,_buf.str(),GNC::GCS::IControladorLog::TraceLog);\
	} \
}

namespace GNC {
	namespace GCS {
		class EXTAPI IControladorLog {
		public:
			typedef std::map<int, std::string> MapaLogLevels;
			
			public:
			typedef enum {
				NoLog = 0,
				FatalLog,
				ErrorLog,
				WarnLog,
				InfoLog,
				DebugLog,
				TraceLog
			} LogLevel;

			static IControladorLog* Instance();
			static void FreeInstance();
		
		protected:
			//--- Constructores
			IControladorLog();
			~IControladorLog();
		public:			
			//log en el logger por defecto
			virtual void Log(const std::string& mensaje, LogLevel logLevel = DebugLog) = 0;
			//log en otro logger
			virtual void Log(const std::string& logger, const std::string& mensaje,LogLevel logLevel = DebugLog) = 0;
			//logs con contexto
			virtual void LogConContexto(const std::string& contexto, const std::string& mensaje,LogLevel logLevel = DebugLog) = 0;
			//logs en logger especifico con contexto
			virtual void LogConContexto(const std::string& logger, const std::string& contexto, const std::string& mensaje,LogLevel logLevel = DebugLog) = 0;
			
			virtual bool IsEnabledFor(LogLevel logLevel) = 0;
			
			virtual void GetLogLevelsMap(GNC::GCS::IControladorLog::MapaLogLevels& levels) = 0;
			virtual int GetLogLevelCode(const std::string& level) = 0;
			
			virtual int GetActiveLogLevel() = 0;

			virtual void SetActiveLogLevel(int logLevel) = 0;
			
		};
	}
}
#endif
