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
#include <wx/wxsqlite3/wxsqlite3.h>
#ifndef _WIN32
#include <unistd.h>
#define Sleep sleep
#define strcpy_s strcpy
#define strcat_s strcat
#endif

//version of ginkgo bbdd
#define GINKGO_BBDD_VERSION 7

//#define _GINKGO_TRACE

/** Timeout values on busy or lock conditions   **/
/** if you observe lock errors you might try to **/
/** increase the values.                        **/

int AbrirConexion(wxSQLite3Database& dataBase,const std::string& DBFileName)
{
	if(dataBase.IsOpen()) {
		return (0);
	}

	dataBase.Open(FROMPATH(DBFileName),wxEmptyString, WXSQLITE_OPEN_READWRITE|WXSQLITE_OPEN_CREATE);

	dataBase.EnableForeignKeySupport(true);

	dataBase.ExecuteUpdate(wxT("PRAGMA recursive_triggers = true;"));
	dataBase.ExecuteUpdate(wxT("PRAGMA synchronous=NORMAL;"));

#ifdef _GINKGO_TRACE

	dataBase.ExecuteUpdate(wxT("PRAGMA parser_trace  = true;"));

#endif

	 return 1;
}

int CreateGinkgoDB(wxSQLite3Database& dataBase,const char *DBFileName)
{
	if (!AbrirConexion(dataBase, DBFileName)) {
		return (0);
	}
    /** Create Tables del Historial: **/
	 /** 1. study table **/
	wxString sentencia = wxEmptyString;
   sentencia << wxT("CREATE TABLE Study (");
	sentencia << wxT("pk INTEGER PRIMARY KEY,"); //autoincrement by default (SQLITE)
	sentencia << wxT("pat_id VARCHAR(250) ,");
	sentencia << wxT("pat_id_issuer VARCHAR(250),");
	sentencia << wxT("pat_name VARCHAR(250) ,");
   sentencia << wxT("pat_birthdate DATE,");
   sentencia << wxT("pat_sex	VARCHAR(250),");
	sentencia << wxT("study_iuid VARCHAR(250),");
	sentencia << wxT("study_id VARCHAR(250),");
	sentencia << wxT("study_datetime DATETIME,");
	sentencia << wxT("accession_no VARCHAR(250),");
	sentencia << wxT("ref_physician VARCHAR(250),");
	sentencia << wxT("study_desc VARCHAR(250),");
   sentencia << wxT("mods_in_study VARCHAR(250),");
	sentencia << wxT("cuids_in_study VARCHAR(250),");
	sentencia << wxT("num_series INTEGER,");
	sentencia << wxT("num_instances INTEGER,");
	sentencia << wxT("location CHAR DEFAULT 'B',"); //B implies fully local L implies some files linked W implies some files wado linked
	sentencia << wxT("created_time DATETIME DEFAULT CURRENT_TIMESTAMP,");
	sentencia << wxT("updated_time DATETIME);");
	dataBase.ExecuteUpdate(sentencia);

	//study indexes
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index pat_id ");
	sentencia << wxT("ON Study(pat_id, pat_id_issuer);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index pat_name ");
	sentencia << wxT("ON Study(pat_name);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index pat_birthdate ");
	sentencia << wxT("ON Study(pat_birthdate);");
	dataBase.ExecuteUpdate(sentencia);
		
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index pat_sex ");
	sentencia << wxT("ON Study(pat_sex);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index study_iuid ");
	sentencia << wxT("ON Study(study_iuid);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index study_datetime ");
	sentencia << wxT("ON Study(study_datetime);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index accession_no ");
	sentencia << wxT("ON Study(accession_no);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index ref_physician ");
	sentencia << wxT("ON Study(ref_physician);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index study_desc ");
	sentencia << wxT("ON Study(study_desc);");
	dataBase.ExecuteUpdate(sentencia);

	 /** 1. Tabla de series **/
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE TABLE Series (");
	sentencia << wxT("pk INTEGER PRIMARY KEY,"); //autoincrement by default (SQLITE)
	sentencia << wxT("study_fk INTEGER REFERENCES Study(pk) ON UPDATE CASCADE ON DELETE CASCADE,");
	sentencia << wxT("series_iuid VARCHAR(250),");
	sentencia << wxT("series_datetime DATETIME,");
	sentencia << wxT("series_no VARCHAR(250),");
	sentencia << wxT("modality VARCHAR(250),");
	sentencia << wxT("uid_importer VARCHAR(250),");
	sentencia << wxT("body_part VARCHAR(250),");
	sentencia << wxT("laterality VARCHAR(250),");
	sentencia << wxT("series_desc VARCHAR(250),");
	sentencia << wxT("institution VARCHAR(250),");
	sentencia << wxT("station_name VARCHAR(250),");
	sentencia << wxT("department VARCHAR(250),");
	sentencia << wxT("perf_physician VARCHAR(250),");
	sentencia << wxT("num_instances INTEGER,");
	sentencia << wxT("frame_of_reference_uid VARCHAR(250),");
	sentencia << wxT("tuids_in_series VARCHAR(250),");
	sentencia << wxT("location CHAR DEFAULT 'B',"); //B implies fully local L implies some files linked W implies some files wado linked
	sentencia << wxT("created_time DATETIME DEFAULT CURRENT_TIMESTAMP,");
	sentencia << wxT("updated_time DATETIME);");
	dataBase.ExecuteUpdate(sentencia);


	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index series_iuid ");
	sentencia << wxT("ON Series(series_iuid);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index series_datetime ");
	sentencia << wxT("ON Series(series_datetime);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index modality ");
	sentencia << wxT("ON Series(modality);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index body_part ");
	sentencia << wxT("ON Series(body_part);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index laterality ");
	sentencia << wxT("ON Series(laterality);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index series_desc ");
	sentencia << wxT("ON Series(series_desc);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index institution ");
	sentencia << wxT("ON Series(institution);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index station_name ");
	sentencia << wxT("ON Series(station_name);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index department ");
	sentencia << wxT("ON Series(department);");
	dataBase.ExecuteUpdate(sentencia);
	
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index perf_physician ");
	sentencia << wxT("ON Series(perf_physician);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
	sentencia << wxT("CREATE Index series_study ");
	sentencia << wxT("ON Series(pk, study_fk);");
	dataBase.ExecuteUpdate(sentencia);


	sentencia = wxEmptyString;
	sentencia << wxT("CREATE Index series_location ");
	sentencia << wxT("ON Series(study_fk, location);");
	dataBase.ExecuteUpdate(sentencia);

	 /** 1. Tabla de ficheros **/
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE TABLE File (");
	sentencia << wxT("pk INTEGER PRIMARY KEY,"); //autoincrement by default (SQLITE)
	sentencia << wxT("series_fk INTEGER REFERENCES Series(pk) ON UPDATE CASCADE ON DELETE CASCADE,");
#ifdef _WIN32
	sentencia << wxT("file_path VARCHAR(1024) COLLATE NOCASE,");
#else
	sentencia << wxT("file_path VARCHAR(1024),");
#endif
	sentencia << wxT("file_sopiuid VARCHAR(240),");
	sentencia << wxT("file_tsuid VARCHAR(240),");
	sentencia << wxT("content_datetime DATETIME,");
	sentencia << wxT("sopcuid VARCHAR(240),");
	sentencia << wxT("orientation CHAR,");//A,C,S, O
	sentencia << wxT("image_positionx DOUBLE,");//patients position en la coordenada que interesa
	sentencia << wxT("image_positiony DOUBLE,");//patients position en la coordenada que interesa
	sentencia << wxT("image_positionz DOUBLE,");//patients position en la coordenada que interesa
	sentencia << wxT("slice_location DOUBLE,");//slice location
	sentencia << wxT("slice_thickness DOUBLE,");//slice thickness
	sentencia << wxT("height INTEGER,");	//altura imagen
	sentencia << wxT("width INTEGER,");		//anchura imagen
	sentencia << wxT("number_of_frames INTEGER,"); //image number of frames
	sentencia << wxT("instance_number INTEGER,"); //instance number
	sentencia << wxT("file_desc VARCHAR(240),");
	sentencia << wxT("spacing VARCHAR(240),");
	sentencia << wxT("direction_cosines VARCHAR(240),");
	sentencia << wxT("location CHAR DEFAULT 'B',"); //B implies fully local L implies some files linked W implies some files wado linked
	sentencia << wxT("created_time DATETIME DEFAULT CURRENT_TIMESTAMP,");
	sentencia << wxT("updated_time DATETIME);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index file_tsuid ");
	sentencia << wxT("ON File(file_tsuid);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index orderBy1 ");
	sentencia << wxT("ON File(instance_number, content_datetime, image_positionz);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index orderBy2 ");
	sentencia << wxT("ON File(instance_number, content_datetime, image_positiony);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index orderBy3 ");
	sentencia << wxT("ON File(instance_number, content_datetime, image_positionx);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index orderBy4 ");
	sentencia << wxT("ON File(instance_number, content_datetime, slice_location);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index instance_number ");
	sentencia << wxT("ON File(instance_number);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index content_datetime ");
	sentencia << wxT("ON File(content_datetime);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index sopcuid ");
	sentencia << wxT("ON File(sopcuid);");
	dataBase.ExecuteUpdate(sentencia);
	sentencia = wxEmptyString;

	sentencia = wxEmptyString;
	sentencia << wxT("CREATE Index file_series ");
	sentencia << wxT("ON File(pk, series_fk);");
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
	sentencia << wxT("CREATE Index file_location ");
	sentencia << wxT("ON File(series_fk, location);");
	dataBase.ExecuteUpdate(sentencia);

	//version table
	sentencia = wxEmptyString;
	sentencia << wxT("CREATE TABLE MetadataBBDD (");
	sentencia << wxT("Property VARCHAR(70) PRIMARY KEY, ");
	sentencia << wxT("Value    VARCHAR(70));");
	dataBase.ExecuteUpdate(sentencia);

	//insert version value
	sentencia = wxEmptyString;
	sentencia << wxT("INSERT INTO MetadataBBDD (Property, Value) VALUES ('Version',") << GINKGO_BBDD_VERSION << wxT(");");
	dataBase.ExecuteUpdate(sentencia);

	 /** tasks table **/
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE TABLE Tasks (");
	sentencia << wxT("pk INTEGER PRIMARY KEY,"); //autoincrement by default (SQLITE)
	sentencia << wxT("created_time DATETIME DEFAULT CURRENT_TIMESTAMP,");
	sentencia << wxT("uid VARCHAR(40),");//class uid
	sentencia << wxT("status INGEGER(1) DEFAULT '0',");//status
	sentencia << wxT("error_description VARCHAR(240) DEFAULT '',");//message
	sentencia << wxT("description VARCHAR(250),");//description
	sentencia << wxT("data TEXT);");//task data
	dataBase.ExecuteUpdate(sentencia);

	sentencia = wxEmptyString;
   sentencia << wxT("CREATE Index task_created_time ");
	sentencia << wxT("ON Tasks(created_time);");
	dataBase.ExecuteUpdate(sentencia);

	/** tasks table **/
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE TABLE TasksSeries (");
	sentencia << wxT("task_fk INTEGER REFERENCES Tasks(pk) ON UPDATE CASCADE ON DELETE CASCADE,"); //task
	sentencia << wxT("series_fk INTEGER REFERENCES Series(pk) ON UPDATE CASCADE ON DELETE CASCADE,");
	sentencia << wxT("PRIMARY KEY (task_fk, series_fk));");//series
	dataBase.ExecuteUpdate(sentencia);

	 /*Triggers de mantenimiento*/

	 //si se borran todos los ficheros de una serie se borra la serie
	sentencia = wxEmptyString;
	sentencia << wxT("CREATE TRIGGER delete_fichero1 AFTER DELETE ON File ");
	sentencia << wxT("FOR EACH ROW ");
		sentencia << wxT("WHEN ((SELECT series_fk FROM File WHERE series_fk = OLD.series_fk) IS NULL) ");
		sentencia << wxT("BEGIN ");
		sentencia << wxT("DELETE FROM Series WHERE pk = OLD.series_fk; ");
	sentencia << wxT("END;");
	dataBase.ExecuteUpdate(sentencia);


	 //si se borran todas las series de un estudio se borra el estudio
	sentencia = wxEmptyString;
	sentencia << wxT("CREATE TRIGGER delete_serie AFTER DELETE ON Series ");
	sentencia << wxT("FOR EACH ROW ");
		sentencia << wxT("WHEN ((SELECT study_fk FROM Series WHERE study_fk = OLD.study_fk) IS NULL) ");
		sentencia << wxT("BEGIN ");
		sentencia << wxT("DELETE FROM Study WHERE pk = OLD.study_fk; ");
	sentencia << wxT("END;");
	dataBase.ExecuteUpdate(sentencia);

	 //si se borran todas las tareas que no apunten a ninguna serie
	sentencia = wxEmptyString;
	sentencia << wxT("CREATE TRIGGER delete_task AFTER DELETE ON TasksSeries ");
	sentencia << wxT("FOR EACH ROW ");
		sentencia << wxT("WHEN ((SELECT task_fk FROM TasksSeries WHERE task_fk = OLD.task_fk) IS NULL) ");
		sentencia << wxT("BEGIN ");
		sentencia << wxT("DELETE FROM Tasks WHERE pk = OLD.task_fk; ");
	sentencia << wxT("END;");
	dataBase.ExecuteUpdate(sentencia);

    return(1);

}

int AbrirConexionThumbnails(wxSQLite3Database& dataBase,const std::string& DBFileName)
{
	if(dataBase.IsOpen()) {
		return (0);
	}

	dataBase.Open(FROMPATH(DBFileName),wxEmptyString, WXSQLITE_OPEN_READWRITE|WXSQLITE_OPEN_CREATE);

	 return 1;
}

int CreateGinkgoDBThumbnails(wxSQLite3Database& dataBase,const std::string& DBFileName)
{
	if (!AbrirConexionThumbnails(dataBase, DBFileName)) {
		return (0);
	}
    /** Create Tables del Historial: **/

	wxString sentencia(wxT(""));
    /** 1. Tabla de thumbnails **/
	sentencia = wxEmptyString;
   sentencia << wxT("CREATE TABLE Thumbnails (");
	sentencia << wxT("file_pk INTEGER,");
	sentencia << wxT("width INTEGER,");
	sentencia << wxT("height INTEGER,");
	sentencia << wxT("thumb BLOB ");
	sentencia << wxT(", PRIMARY KEY (file_pk, width, height))");
	dataBase.ExecuteUpdate(sentencia);
   return(1);

}




