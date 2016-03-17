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
#include <api/api.h>
#include <api/icommand.h>
#include <api/observers/ieventsobserver.h>

class wxSemaphore;

namespace GADAPI
{
//lee un directorio en profundidad y lo incluye en el dicomdir
class EXTAPI DBMigrationCommandParams : public GNC::GCS::IComandoParams
{
public:
        DBMigrationCommandParams(const std::string& oldDicomDirPath,const std::string& newDicomDirPath, bool deleteAfterFinish);
        virtual ~DBMigrationCommandParams();

        std::string OldDicomDir;
        std::string NewDicomDir;
        bool DeleteAfterFinish;
        wxSemaphore* SemaphoreWaitIncludeFiles;
};

class EXTAPI DBMigrationCommand : public GNC::GCS::IComando, public GNC::GCS::IEventsObserver
{
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
