/*
 *  Ginkgo CADx Project
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
#include <wx/wxsqlite3/wxsqlite3.h>
#include <api/globals.h>
#ifndef _WIN32
#include <unistd.h>
#define Sleep sleep
#define strcpy_s strcpy
#define strcat_s strcat
#endif

//#define _GINKGO_TRACE

/** Timeout values on busy or lock conditions   **/
/** if you observe lock errors you might try to **/
/** increase the values.                        **/
#define SQLTM_COUNT       200  /** -> SQLTM_COUNT*SQLTM_TIME ms timeout **/
#define SQLTM_TIME        50

inline int AbrirConexionBBDDMensajes(wxSQLite3Database& dataBase,const std::string& DBFileName)
{
        if(dataBase.IsOpen()) {
                return (0);
        }

        dataBase.Open(FROMPATH(DBFileName),wxEmptyString, WXSQLITE_OPEN_READWRITE|WXSQLITE_OPEN_CREATE);

        dataBase.EnableForeignKeySupport(false);

        return 1;
}

inline int CreateMensajesHl7DB(wxSQLite3Database& dataBase,const std::string& DBFileName)
{
        if (!AbrirConexionBBDDMensajes(dataBase, DBFileName)) {
                return (0);
        }
        /** Create Tables del Historial: **/

        /** 1. Tabla de mensajes **/
        wxString sentencia(wxT(""));
        sentencia << wxT("CREATE TABLE MensajesHL7 (");
        sentencia << wxT("IDMensaje INTEGER PRIMARY KEY,");
        sentencia << wxT("FechaEnvio DATETIME,");
        sentencia << wxT("Mensaje TEXT,");
        sentencia << wxT("URLEnvio VARCHAR(1000),");
        sentencia << wxT("Protocolo integer,");
        sentencia << wxT("ProcesarACK CHAR,");
        sentencia << wxT("MsgControlId VARCHAR(100) Default '',");
        sentencia << wxT("MensajeError VARCHAR(500) Default NULL,");
        sentencia << wxT("Enviado CHAR Default 'N');");
        dataBase.ExecuteUpdate(sentencia);

        return(1);

}




