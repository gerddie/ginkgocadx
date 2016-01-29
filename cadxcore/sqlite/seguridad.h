/*
 *  $Id: hl7sqlite.h $
 *  Proyecto Ginkgo
 *
 *  Copyright 2008 MetaEmotion S.L. All rights reserved.
 *
 */
#pragma once
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

inline int AbrirConexionBBDDSeguridad(wxSQLite3Database& dataBase,const std::string& DBFileName)
{
	if(dataBase.IsOpen()) {
		return (0);
	}

	dataBase.Open(FROMPATH(DBFileName),wxEmptyString, WXSQLITE_OPEN_READWRITE|WXSQLITE_OPEN_CREATE);

	dataBase.EnableForeignKeySupport(false);

	 return 1;
}

inline int CreateSeguridadDB(wxSQLite3Database& dataBase,const std::string& DBFileName)
{
	if (!AbrirConexionBBDDSeguridad(dataBase, DBFileName)) {
		return (0);
	}
    /** Create Tables del Seguridad: **/

    /** 1. Tabla de usuario/password **/
	wxString sentencia(wxT(""));
	sentencia << wxT("CREATE TABLE UserPass (");
	sentencia << wxT("User VARCHAR(20) PRIMARY KEY,");
   sentencia << wxT("Password VARCHAR(64));");
	dataBase.ExecuteUpdate(sentencia);

	return(1);
}




