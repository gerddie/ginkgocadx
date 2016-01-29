/*
*  
*  $Id: comandoincluirhistorial.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#pragma once
#include <api/api.h>
#include <api/icommand.h>
#include <api/observers/ieventsobserver.h>

class wxSemaphore;

namespace GADAPI {
	//lee un directorio en profundidad y lo incluye en el dicomdir
	class EXTAPI DBMigrationCommandParams : public GNC::GCS::IComandoParams {
	public:
		DBMigrationCommandParams(const std::string& oldDicomDirPath,const std::string& newDicomDirPath, bool deleteAfterFinish);
		virtual ~DBMigrationCommandParams();

		std::string OldDicomDir;
		std::string NewDicomDir;
		bool DeleteAfterFinish;
		wxSemaphore* SemaphoreWaitIncludeFiles;
	};

	class EXTAPI DBMigrationCommand : public GNC::GCS::IComando, public GNC::GCS::IEventsObserver {
	public:

		DBMigrationCommand(DBMigrationCommandParams* pParams);
     protected:
		virtual void Execute();
		virtual void Update();

		virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt) ;
	protected:

		DBMigrationCommandParams* m_pMigrationParams;
	};
}
