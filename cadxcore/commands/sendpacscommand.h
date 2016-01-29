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
#include <api/ivista.h>
#include <api/controllers/ipacscontroller.h>

namespace GIL{
	class IModeloIntegracion;
	namespace DICOM{
		class IPACSController;
	}
}

namespace GADAPI {
	class EXTAPI SendPACSCommand : public GNC::GCS::IComando {
	public:
		SendPACSCommand(const std::string& selectedServer, const std::string& hl7Message, const std::list<long> seriesId);
		
		SendPACSCommand( const SendPACSCommand& o);
		SendPACSCommand& operator = (const SendPACSCommand& o);
	protected:
		void checkDependencies();
	public:

		virtual ~SendPACSCommand();		
		virtual std::string getClassUID();

		virtual std::string serialize();
		virtual void init(const std::string& serializedValue);

    protected:
		virtual void Execute();
		virtual void Update();

    public:
		virtual bool NotificarProgreso(float progresoNormalizado, const std::string &texto);

		void LiberarRecursos();

		std::string m_selectedServer;
		std::list<long> m_seriesId;
		std::string m_hl7Message;
		std::string m_error;
	};
}
