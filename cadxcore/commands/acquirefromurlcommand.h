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
#include <api/globals.h>
#include <api/icommand.h>
#include <api/ihistorytool.h>
#include <api/observers/ieventsobserver.h>

class wxSemaphore;


namespace GADAPI {
	class AcquireFromURLCommandParameters : public GNC::GCS::IComandoParams {

	public:
		//download only specifying 
		AcquireFromURLCommandParameters(const std::list<std::string>& urlList);
		virtual ~AcquireFromURLCommandParameters();

	public:
		std::list<std::string> m_urlList;
		std::string m_error;
		wxSemaphore* m_pSemWait;
	};



	class AcquireFromURLCommand : public GNC::GCS::IComando, public GNC::GCS::IEventsObserver {
	public:
		AcquireFromURLCommand(AcquireFromURLCommandParameters* pParams);

    protected:
		virtual void Execute();
		virtual void Update();

    public:

		virtual bool NotificarProgreso(float progresoNormalizado, const std::string &texto);

		virtual void LiberarRecursos();
		virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

		AcquireFromURLCommandParameters* m_pAcquireParams;
	};
}
