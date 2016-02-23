#pragma once
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
