#pragma once
/*
 *  
 *  $Id: controladorlog.h $
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
#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <string>
#include <map>

#include <api/api.h>
#include <api/controllers/icontroladorlog.h>

//-------------------------------------------------------------------------------------------------
namespace GNC {
	class ControladorLog : public GNC::GCS::IControladorLog
	{
		//region "Patron singleton"
	public:
		
		static ControladorLog* Instance();
		static void FreeInstance();

		static ControladorLog* m_pInstance;
		//endregion

	protected:
		//--- Constructores
		ControladorLog();
		~ControladorLog();
	public:			
		//log en el logger por defecto
		virtual void Log(const std::string& mensaje,LogLevel logLevel = DebugLog);
		//log en otro logger
		virtual void Log(const std::string& logger, const std::string& mensaje,LogLevel logLevel = DebugLog);
		//logs con contexto
		virtual void LogConContexto(const std::string& contexto, const std::string& mensaje,LogLevel logLevel = DebugLog);
		//logs en logger especifico con contexto
		virtual void LogConContexto(const std::string& logger, const std::string& contexto, const std::string& mensaje,LogLevel logLevel = DebugLog);
		
		virtual bool IsEnabledFor(GNC::GCS::IControladorLog::LogLevel logLevel);
		
		void GetLogLevelsMap(GNC::GCS::IControladorLog::MapaLogLevels& levels);
		virtual int GetLogLevelCode(const std::string& level);
		
		virtual int GetActiveLogLevel();
		
		void SetActiveLogLevel(int ll);
		
	};

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#endif