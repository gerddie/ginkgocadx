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
//#define _GINKGO_TRACE
#include <api/globals.h>
#include <api/icommand.h>
#include <api/ihistorytool.h>
#include <api/observers/ieventsobserver.h>

class wxSemaphore;
namespace GNC
{
namespace GCS
{
class IModuleController;
class HangingProtocol;
}
}

namespace GADAPI
{
class SynchronizationCommandParameters : public GNC::GCS::IComandoParams
{

public:
        //download only specifying
        SynchronizationCommandParameters(const std::list<long>& seriesPk, GNC::GCS::IHistoryTool* pTool);
        SynchronizationCommandParameters(const std::list<long>& seriesPk, GNC::GCS::IModuleController* pCtrlMod, int mode, const GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& pHangingProtocol);
        virtual ~SynchronizationCommandParameters();

public:
        std::list<long> m_SeriesPk;
        std::string m_error;
        GNC::GCS::IHistoryTool* m_pTool;
        GNC::GCS::IModuleController* m_pCtrlMod;
        int m_mode;
        GNC::GCS::Ptr<GNC::GCS::HangingProtocol> m_pHangingProtocol;
        wxSemaphore* m_pSemWait;
};



class SynchronizationCommand : public GNC::GCS::IComando, public GNC::GCS::IEventsObserver
{
public:
        SynchronizationCommand(SynchronizationCommandParameters* pParams);

protected:
        virtual void Execute();
        virtual void Update();

public:

        virtual bool NotificarProgreso(float progresoNormalizado, const std::string &texto);

        virtual void LiberarRecursos();
        virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

        SynchronizationCommandParameters* m_pSynchronizeParams;
};
}
