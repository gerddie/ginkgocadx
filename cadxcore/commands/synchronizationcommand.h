/*
*  
*  $Id: comandopacs.h $
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
//#define _GINKGO_TRACE
#include <api/globals.h>
#include <api/icommand.h>
#include <api/ihistorytool.h>
#include <api/observers/ieventsobserver.h>

class wxSemaphore;
namespace GNC {
	namespace GCS {
		class IModuleController;
		class HangingProtocol;
	}
}

namespace GADAPI {
	class SynchronizationCommandParameters : public GNC::GCS::IComandoParams {

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



	class SynchronizationCommand : public GNC::GCS::IComando, public GNC::GCS::IEventsObserver {
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
