/*
*  
*  $Id: comandoincluirhistorial.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "dbmigrationcommand.h"

#include <api/globals.h>
#include <api/controllers/ieventscontroller.h>
#include <api/ientorno.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladorlog.h>
#include <api/internationalization/internationalization.h>
#include <eventos/progresocomando.h>
#include <eventos/mensajes.h>

#include "comandoincluirhistorial.h"

#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/thread.h>
#include <wx/wxsqlite3/wxsqlite3.h>
#include <sqlite/sqlite3.h>

#define IDC_MIGRATION 120


GADAPI::DBMigrationCommandParams::DBMigrationCommandParams(const std::string& oldDicomDirPath,const std::string& newDicomDirPath, bool deleteAfterFinish)
{
	OldDicomDir = oldDicomDirPath;
	NewDicomDir = newDicomDirPath;
	DeleteAfterFinish = deleteAfterFinish;
	SemaphoreWaitIncludeFiles = new wxSemaphore();
}

GADAPI::DBMigrationCommandParams::~DBMigrationCommandParams()
{
	delete SemaphoreWaitIncludeFiles;
	SemaphoreWaitIncludeFiles = NULL;
}


GADAPI::DBMigrationCommand::DBMigrationCommand(DBMigrationCommandParams* pParams) : IComando(pParams)
{
	m_pMigrationParams = pParams;
	SetId(IDC_MIGRATION);
	GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
}

void GADAPI::DBMigrationCommand::Execute()
{
	//trying to read linked files...
	std::list<std::string> linkedFiles;
	{
		std::string pathFicheroDB;
		std::string pathFicheroDBThumbnails;
		{
			std::ostringstream ostr;
			ostr << m_pMigrationParams->OldDicomDir  << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgodatabase_2.1.db";
			pathFicheroDB = ostr.str();
		}
		if (wxFileExists(FROMPATH(pathFicheroDB))) {
			try {
				wxSQLite3Database connection;
				connection.Open(FROMPATH(pathFicheroDB));
				wxSQLite3StatementBuffer bufSQL;
				bufSQL.Format("Select distinct(file_path) from File");
				wxSQLite3ResultSet resultados = connection.ExecuteQuery(bufSQL);
				while (resultados.NextRow()) {
					wxFileName fileName(resultados.GetAsString(0));
					if (fileName.IsRelative()){
						fileName.MakeAbsolute(FROMPATH(m_pMigrationParams->OldDicomDir));
					}
					if (fileName.FileExists()) {
						linkedFiles.push_back(std::string(TOPATH(fileName.GetFullPath())));
					}
				}
			} catch (wxSQLite3Exception& ex) {
				LOG_WARN("BBDD", "error exploring old database, linked file could be lost " << ex.GetMessage());
			}
		}

	}
	if (!linkedFiles.empty()) {
		NotificarProgreso(0.0f,_Std("Migrating old DICOM Dir ..."));
		GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(linkedFiles,false, GNC::GCS::IHistoryController::TAA_LINK);
		GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
	
		GNC::CommandController::Instance()->ProcessAsync("Processing...",pCmd, m_pMigrationParams);
		NotificarProgreso(0.05f,_Std("Including files ..."));
		m_pMigrationParams->SemaphoreWaitIncludeFiles->Wait();
	}
	//
	NotificarProgreso(0.0f,_Std("Migrating old DICOM Dir ..."));
	GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(m_pMigrationParams->OldDicomDir, true, GNC::GCS::IHistoryController::TAA_COPY);
	GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
	
	GNC::CommandController::Instance()->ProcessAsync("Processing...",pCmd, m_pMigrationParams);
	NotificarProgreso(0.05f,_Std("Including files ..."));
	m_pMigrationParams->SemaphoreWaitIncludeFiles->Wait();
}

void GADAPI::DBMigrationCommand::Update()
{
	if (m_pMigrationParams->DeleteAfterFinish) 
	{
		if (wxMessageBox(_("Migration finished successfully, now Ginkgo CADx will delete folder:\n") + FROMPATH(m_pMigrationParams->OldDicomDir) + _T("\n") + _("and all it's sub folders\nDo you confirm deletion?"), _("Info"), wxICON_WARNING|wxOK|wxCANCEL, GNC::GCS::IEntorno::Instance()->GetVentanaRaiz()) == wxOK) 
		{
			GNC::GCS::IEntorno::Instance()->RemoveDirRecursive(m_pMigrationParams->OldDicomDir);
		}		
	} 
	else 
	{
		GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, _Std("Migration finisthed successfully"), GNC::GCS::Events::EventoMensajes::PopUpMessage));
	}	
}

void GADAPI::DBMigrationCommand::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);
	if (pEvt == NULL  || pEvt->GetComando() == NULL || pEvt->GetComando()->GetOwner() != m_pMigrationParams) {
		return;
	}
	if (pEvt->GetTipo() == GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado)
	{
		m_pMigrationParams->SemaphoreWaitIncludeFiles->Post();
	}
}

