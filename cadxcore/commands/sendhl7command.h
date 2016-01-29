/*
*  
*  $Id: sendhl7command.h $
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
#include <api/icommand.h>
#include <main/controllers/controladorbbddhl7.h>

namespace GADAPI {
	class SendHL7CommandParams : public GNC::GCS::IComandoParams {

	public:
		SendHL7CommandParams(const std::list<GIL::HL7::MensajeHL7>& messages);

		virtual ~SendHL7CommandParams();

	public:
		std::list<GIL::HL7::MensajeHL7> messages;
		wxSemaphore* m_pSemaphore;
		int m_idLastSock;		
	};

	
	class SendHL7Command : public GNC::GCS::IComando {
	public:
		SendHL7Command(SendHL7CommandParams* pParams);

    protected:
		virtual void Execute();
		virtual void Update();
		virtual void OnAbort();

    protected:
		/* Envia el mensaje por MLLP */
		void EnviarMensajeMLLP(const std::string& msg, const std::string& url, bool procesarACK=true, const std::string& msgControlId="") ;

		SendHL7CommandParams* m_pSendParams;
	};
}
